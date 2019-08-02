//
// Created by 叶亮 on 2019/7/16.
//

#ifndef ANDROIDRECORDER_COOMON_UTILS_H
#define ANDROIDRECORDER_COOMON_UTILS_H

#include <sys/time.h>

static inline long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec/1000;
}

#endif //ANDROIDRECORDER_COOMON_UTILS_H
