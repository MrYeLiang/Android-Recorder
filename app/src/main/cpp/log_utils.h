//
// Created by 叶亮 on 2019/7/15.
//

#ifndef ANDROIDRECORDER_LOGUTILS_H
#define ANDROIDRECORDER_LOGUTILS_H

#include <jni.h>--
#include <android/log.h>
#include <stdio.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "--recorder--", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "--recorder--", __VA_ARGS__)

#endif //ANDROIDRECORDER_LOGUTILS_H
