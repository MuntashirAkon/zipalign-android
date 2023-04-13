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

#ifndef __LIBS_ZIPALIGN_H
#define __LIBS_ZIPALIGN_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

namespace android {

/*
 * Generate a new, aligned, zip "output" from an "input" zip.
 * - alignTo: Alignment (in bytes) for uncompressed entries.
 * - pageAlignSharedLibs: Align .so files to 4096 and other files to
 *   alignTo, or all files to alignTo if false.
 * - force  : Overwrite output if it exists, fail otherwise.
 *
 * Returns 0 on success.
 */
int process(const char* input, const char* output, int alignTo, bool pageAlignSharedLibs, bool force);

/*
 * Verify the alignment of a zip archive.
 * - alignTo: Alignment (in bytes) for uncompressed entries.
 * - pageAlignSharedLibs: Align .so files to 4096 and other files to
 *   alignTo, or all files to alignTo if false.
 *
 * Returns 0 on success.
 */
int verify(const char* fileName, int alignTo, bool pageAlignSharedLibs, bool verbose);

}; // namespace android

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LIBS_ZIPALIGN_H
