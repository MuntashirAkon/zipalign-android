/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Zip alignment tool
 */
#include "ZipFile.h"

#include <stdlib.h>
#include <stdio.h>

using namespace android;

/*
 * Copy all entries from "pZin" to "pZout", aligning as needed.
 */
static int copyAndAlign(ZipFile *pZin, ZipFile *pZout, int alignment) {
    int numEntries = pZin->getNumEntries();
    ZipEntry *pEntry;
    int bias = 0;
    status_t status;

    for (int i = 0; i < numEntries; i++) {
        ZipEntry *pNewEntry;
        int padding = 0;

        pEntry = pZin->getEntryByIndex(i);
        if (pEntry == nullptr) {
            fprintf(stderr, "ERROR: unable to retrieve entry %d\n", i);
            return 1;
        }

        if (pEntry->isCompressed()) {
            /* copy the entry without padding */
            //printf("--- %s: orig at %ld len=%ld (compressed)\n",
            //    pEntry->getFileName(), (long) pEntry->getFileOffset(),
            //    (long) pEntry->getUncompressedLen());

        } else {
            /*
             * Copy the entry, adjusting as required.  We assume that the
             * file position in the new file will be equal to the file
             * position in the original.
             */
            long newOffset = pEntry->getFileOffset() + bias;
            padding = (alignment - (newOffset % alignment)) % alignment;

            //printf("--- %s: orig at %ld(+%d) len=%ld, adding pad=%d\n",
            //    pEntry->getFileName(), (long) pEntry->getFileOffset(),
            //    bias, (long) pEntry->getUncompressedLen(), padding);
        }

        status = pZout->add(pZin, pEntry, padding, &pNewEntry);
        if (status != NO_ERROR)
            return 1;
        bias += padding;
        //printf(" added '%s' at %ld (pad=%d)\n",
        //    pNewEntry->getFileName(), (long) pNewEntry->getFileOffset(),
        //    padding);
    }

    return 0;
}

/*
 * Process a file.  We open the input and output files, failing if the
 * output file exists and "force" wasn't specified.
 */
extern "C"
int process(const char *inFileName, const char *outFileName, int alignment, bool force) {
    ZipFile zin, zout;

    //printf("PROCESS: align=%d in='%s' out='%s' force=%d\n",
    //    alignment, inFileName, outFileName, force);

    /* this mode isn't supported -- do a trivial check */
    if (strcmp(inFileName, outFileName) == 0) {
        fprintf(stderr, "Input and output can't be same file\n");
        return 1;
    }

    /* don't overwrite existing unless given permission */
    if (!force && access(outFileName, F_OK) == 0) {
        fprintf(stderr, "Output file '%s' exists\n", outFileName);
        return 1;
    }

    if (zin.open(inFileName, ZipFile::kOpenReadOnly) != NO_ERROR) {
        fprintf(stderr, "Unable to open '%s' as zip archive\n", inFileName);
        return 1;
    }
    if (zout.open(outFileName,
                  ZipFile::kOpenReadWrite | ZipFile::kOpenCreate | ZipFile::kOpenTruncate)
            != NO_ERROR) {
        fprintf(stderr, "Unable to open '%s' as zip archive\n", outFileName);
        return 1;
    }

    int result = copyAndAlign(&zin, &zout, alignment);
    if (result != 0) {
        printf("zipalign: failed rewriting '%s' to '%s'\n",
               inFileName, outFileName);
    }
    return result;
}

/*
 * Verify the alignment of a zip archive.
 */
extern "C"
int verify(const char *fileName, int alignment, bool verbose) {
    ZipFile zipFile;
    bool foundBad = false;

    if (verbose)
        printf("Verifying alignment of %s (%d)...\n", fileName, alignment);

    if (zipFile.open(fileName, ZipFile::kOpenReadOnly) != NO_ERROR) {
        fprintf(stderr, "Unable to open '%s' for verification\n", fileName);
        return 1;
    }

    int numEntries = zipFile.getNumEntries();
    ZipEntry *pEntry;

    for (int i = 0; i < numEntries; i++) {
        pEntry = zipFile.getEntryByIndex(i);
        if (pEntry->isCompressed()) {
            if (verbose) {
                printf("%8ld %s (OK - compressed)\n",
                       (long) pEntry->getFileOffset(), pEntry->getFileName());
            }
        } else {
            long offset = pEntry->getFileOffset();
            if ((offset % alignment) != 0) {
                if (verbose) {
                    printf("%8ld %s (BAD - %ld)\n",
                           (long) offset, pEntry->getFileName(),
                           offset % alignment);
                }
                foundBad = true;
            } else {
                if (verbose) {
                    printf("%8ld %s (OK)\n",
                           (long) offset, pEntry->getFileName());
                }
            }
        }
    }

    if (verbose)
        printf("Verification %s\n", foundBad ? "FAILED" : "succesful");

    return foundBad ? 1 : 0;
}
