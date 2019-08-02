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

//首次预览 初始化数据
JNIEXPORT void JNICALL
Java_com_yeliang_recorder_PreviewScheduler_prepareEGLContext(JNIEnv *env, jobject instance,
                                                             jobject surface, jint width,
                                                             jint height, jint cameraFacingId);

//非首次预览时 开始预览
JNIEXPORT void JNICALL
Java_com_yeliang_recorder_PreviewScheduler_createWindowSurface(JNIEnv *env, jobject instance,
                                                               jobject surface);
//更新每一帧数据
JNIEXPORT void JNICALL
Java_com_yeliang_recorder_PreviewScheduler_notifyFrameAvailable(JNIEnv *env, jobject instance);


JNIEXPORT void JNICALL
Java_com_yeliang_recorder_PreviewScheduler_switchCamera(JNIEnv *env, jobject instance);

#ifdef __cplusplus
}
#endif
#endif
