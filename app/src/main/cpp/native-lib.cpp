#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include "ext.h"

#include<android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "ModuleTesting", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "ModuleTesting", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "ModuleTesting", __VA_ARGS__))


// Global
static JavaVM* g_jvm = NULL;
jobject g_obj = NULL;
jobject g_callback = NULL;


extern "C" {
    // Return simple string to android
    jstring Java_blazers_moduletesting_MainActivity_stringFromJNI(JNIEnv *env, jobject obj) {
        return env->NewStringUTF("Hello from JNI");
    }

    // Return simple object
    jobject Java_blazers_moduletesting_MainActivity_getPeople(JNIEnv *env, jobject instance) {
        // Define Java class path
        const char* peopleName = "blazers/moduletesting/People";
        // Reflect java class
        jclass peopleClazz = env->FindClass(peopleName);
        // Get constructor method
        jmethodID  mid = env->GetMethodID(peopleClazz, "<init>", "(Ljava/lang/String;)V");
        // Call origin C++
        People cPeople;
        // Call java class constructor method
        jobject  ret = env->NewObject(peopleClazz, mid, env->NewStringUTF(cPeople.getName()));
        return ret;
    }

    // Sending data
    void* threadMethod(void* args) {
        LOGI("%s: ======== Entering sub thread ======== ", __FUNCTION__);
        JNIEnv *env;

        if (g_jvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
            LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
            return NULL;
        }

        // Call static method
        jclass clsCaller = env->GetObjectClass(g_obj);
        jmethodID midStatic = env->GetStaticMethodID(clsCaller, "callBack", "(Ljava/lang/String;)V");
        env->CallStaticVoidMethod(clsCaller, midStatic, env->NewStringUTF("HEHEHHEHEHE"));


        // Call callback method
        jclass clsCallback = env->GetObjectClass(g_callback);
        jmethodID midCallback = env->GetMethodID(clsCallback, "callBack", "(Ljava/lang/String;)V");

        int time = 0;
        while (time < 10) {
            sleep(2);
            env->CallVoidMethod(g_callback, midCallback, env->NewStringUTF("HAHAHAHAHAHA"));
            time++;
        }

        // Exit thread
        if(g_jvm->DetachCurrentThread() != JNI_OK) {
            LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
        }
        pthread_exit(0);
    }

    // Subscribe data
    void Java_blazers_moduletesting_MainActivity_subscribe(JNIEnv *env, jobject instance, jobject callback) {
        // Cache the env
        env->GetJavaVM(&g_jvm);
        g_obj = env->NewGlobalRef(instance);
        g_callback = env->NewGlobalRef(callback);

        // Start an Thread
        pthread_t hThread[1];
        pthread_create(&hThread[0], NULL, &threadMethod, NULL);
    }
}