//
// Created by 叶亮 on 2019/7/15.
//

#include "PreviewScheduler.h"

static Controller *controller = 0;
static jobject  g_obj = 0;

JNIEXPORT void JNICALL
Java_com_yeliang_recorder_PreviewScheduler_prepareEGLContext(JNIEnv *env, jobject obj,
                                                             jobject surface, jint screenWidth,
                                                             jint screenHeight, jint cameraFacingId) {
    controller = new Controller();
    JavaVM *jvm = NULL;
    env->GetJavaVM(&jvm);
    g_obj = env->NewGlobalRef(obj);
    if(surface != NULL && NULL != controller){
        ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
        if(window != NULL){
            controller -> prepareEGLContext(window, jvm, g_obj, screenWidth, screenHeight,cameraFacingId);
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yeliang_recorder_PreviewScheduler_createWindowSurface(JNIEnv *env, jobject instance,
                                                               jobject surface) {

    if(surface != 0 && NULL != controller){
        ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
        if(window != NULL){
            controller->createWindowSurface(window);
        }
    }


}extern "C"
JNIEXPORT void JNICALL
Java_com_yeliang_recorder_PreviewScheduler_notifyFrameAvailable(JNIEnv *env, jobject instance) {
    if(NULL != controller){
        controller->notifyFrameAvaliable();
    }

}