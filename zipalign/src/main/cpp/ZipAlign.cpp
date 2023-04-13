#include <cstdlib>
#include <jni.h>
#include <zipalign/ZipAlign.h>

using namespace android;

extern "C"
JNIEXPORT jboolean
JNICALL Java_io_github_muntashirakon_zipalign_ZipAlign_doZipAlign
(JNIEnv *env, jclass clazz, jstring inZipFile, jstring outZipFile, jint alignment, jboolean force) {
    const char *inFileName = env->GetStringUTFChars(inZipFile, nullptr);
    if (!inFileName) {
        return JNI_FALSE;
    }
    const char *outFileName = env->GetStringUTFChars(outZipFile, nullptr);
    if(!outFileName) {
        env->ReleaseStringUTFChars(inZipFile, inFileName);
        return JNI_FALSE;
    }
    bool aligned = process(inFileName, outFileName, alignment, force) == 0;
    env->ReleaseStringUTFChars(inZipFile, inFileName);
    env->ReleaseStringUTFChars(outZipFile, outFileName);
    return aligned ? JNI_TRUE : JNI_FALSE;
}

extern "C"
JNIEXPORT jboolean
JNICALL Java_io_github_muntashirakon_zipalign_ZipAlign_isZipAligned
(JNIEnv *env, jclass clazz, jstring zipFile, jint alignment, jboolean pageAlignSharedLibs) {
    const char *fileName = env->GetStringUTFChars(zipFile, nullptr);
    if(!fileName) {
        return JNI_FALSE;
    }
    bool verified = verify(fileName, alignment, pageAlignSharedLibs, false) == 0;
    env->ReleaseStringUTFChars(zipFile, fileName);
    return verified ? JNI_TRUE : JNI_FALSE;
}
