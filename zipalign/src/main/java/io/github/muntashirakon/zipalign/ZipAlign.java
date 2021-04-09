package io.github.muntashirakon.zipalign;

public class ZipAlign {
    static {
        System.loadLibrary("zipalign");
    }

    /**
     * Generate a new, aligned, zip "output" from an "input" zip.
     * <p>
     * <b><em>NOTE:</em></b> If the APK is to be signed with schema v2 or later, the APK must be aligned <em>before</em>
     * signing it, and for v1 schema (AKA jar signing), the APK must be aligned <em>after</em> signing it.
     *
     * @param inZipFile  The zip file to be aligned.
     * @param outZipFile File where the aligned zip file will be saved.
     * @param alignment  Alignment (in bytes) for uncompressed entries.
     * @param force      Overwrite output if it exists, fail otherwise.
     * @return {@code true} on success.
     */
    public static native boolean doZipAlign(String inZipFile, String outZipFile, int alignment, boolean force);

    /**
     * Verify the alignment of a zip archive.
     *
     * @param zipFie    The zip file whose alignment has to be verified
     * @param alignment Alignment (in bytes) for uncompressed entries.
     * @return {@code true} on success.
     */
    public static native boolean isZipAligned(String zipFie, int alignment);
}
