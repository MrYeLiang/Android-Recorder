//
// Created by 叶亮 on 2019/7/15.
//

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "../cpp/controller.h"

#ifndef ANDROIDRECORDER_PREVIEW_SCHEDULER_H
#define ANDROIDRECORDER_PREVIEW_SCHEDULER_H

#ifdef __cplusplus
extern "C" {
#endif

//
JNIEXPORT void JNICALL
Java_com_yeliang_recorder_PreviewScheduler_prepareEGLContext(JNIEnv *env, jobject instance,
                                                             jobject surface, jint width,
                                                             jint height, jint cameraFacingId);

#ifdef __cplusplus
}
#endif

#endif //ANDROIDRECORDER_PREVIEW_SCHEDULER_H
