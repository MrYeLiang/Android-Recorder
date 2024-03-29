//
// Created by 叶亮 on 2019/7/16.
//

#ifndef ANDROIDRECORDER_EGL_CORE_H
#define ANDROIDRECORDER_EGL_CORE_H

#include <EGL/egl.h>

typedef EGLBoolean (EGLAPIENTRYP PFNEGLPRESENTATIONTIMEANDROIDPROC)(EGLDisplay display, EGLSurface surface, khronos_stime_nanoseconds_t time);

class EGLCore{
public:
    EGLCore();
    virtual ~EGLCore();

    bool init();

    bool init(EGLContext shareContext);

    EGLSurface createWindowSurface(ANativeWindow *_window);

    bool makeCurrent(EGLSurface eglSurface);

    bool swapBuffers(EGLSurface eglSurface);

    void release();


private:
    EGLDisplay  display;
    EGLConfig config;
    EGLContext context;

    PFNEGLPRESENTATIONTIMEANDROIDPROC pfneglPresentationTimeANDROID;
};
#endif //ANDROIDRECORDER_EGL_CORE_H
