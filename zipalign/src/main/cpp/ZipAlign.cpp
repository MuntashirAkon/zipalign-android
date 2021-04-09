#include <cstdlib>
#include <jni.h>
#include <zipalign/ZipAlign.h>

using namespace android;

extern "C"
JNIEXPORT jboolean
JNICALL Java_io_github_muntashirakon_zipalign_ZipAlign_doZipAlign
(JNIEnv *env, jclass clazz, jstring inZipFile, jstring outZipFile, jint alignment, jboolean force) {
    const char *inFileName = env->GetStringUTFChars(inZipFile, nullptr);
    const char *outFileName = env->GetStringUTFChars(outZipFile, nullptr);
    if(!inFileName || !outFileName) {
        return JNI_FALSE;
    }
    if (process(inFileName, outFileName, alignment, force) == 0) return JNI_TRUE;
    return JNI_FALSE;
}

extern "C"
JNIEXPORT jboolean
JNICALL Java_io_github_muntashirakon_zipalign_ZipAlign_isZipAligned
(JNIEnv *env, jclass clazz, jstring zipFile, jint alignment) {
    const char *fileName = env->GetStringUTFChars(zipFile, nullptr);
    if(!fileName) {
        return JNI_FALSE;
    }
    if(verify(fileName, alignment, false) == 0) return JNI_TRUE;
    return JNI_FALSE;
}
