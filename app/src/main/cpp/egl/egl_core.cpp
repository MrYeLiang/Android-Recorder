//
// Created by 叶亮 on 2019/7/16.
//

#include <android/native_window.h>
#include "egl_core.h"
#include "../log_utils.h"

EGLCore::EGLCore() {
    pfneglPresentationTimeANDROID = 0;
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
}

EGLCore::~EGLCore() {

}

void EGLCore::release() {
    if(EGL_NO_DISPLAY != display && EGL_NO_CONTEXT != context){
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }

    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
}

void EGLCore::releaseSurface(EGLSurface eglSurface) {
    eglDestroyContext(display, eglSurface);
}

EGLContext EGLCore::getContext() {
    return context;
}

EGLSurface EGLCore::createWindowSurface(ANativeWindow *_window) {
    EGLSurface  surface = NULL;
    EGLint format;

    if(_window == NULL){
        LOGE("createWindowSurface failed! ");
        return NULL;
    }
    bool eglConfig = eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    if(!eglConfig){
        LOGE("eglGetConfigAttrib error %d", eglGetError());
        release();
        return surface;
    }

    ANativeWindow_setBuffersGeometry(_window, 0, 0, format);
    if(!(surface = eglCreateWindowSurface(display, config, _window, 0))){
        LOGE("eglCreateWindowSurface error!");
    }
    return surface;
}

EGLSurface EGLCore::createOffScreenSurface(int width, int height) {
    EGLSurface surface;
    EGLint PbufferAttributes[] = {EGL_WIDTH, width, EGL_HEIGHT, };
    if(!(surface = eglCreatePbufferSurface(display, config, PbufferAttributes))){
        LOGE("eglCreatePbufferSurface error!");
    }
    return surface;
}

int EGLCore::setPresentationTime(EGLSurface surface, khronos_stime_nanoseconds_t nsesc) {
    pfneglPresentationTimeANDROID(display, surface, nsesc);
    return 1;
}

bool EGLCore::swapBuffers(EGLSurface eglSurface) {
    return eglSwapBuffers(display, eglSurface);
}

bool EGLCore::makeCurrent(EGLSurface eglSurface) {
    return eglMakeCurrent(display, eglSurface, eglSurface, context);
}

bool EGLCore::init() {
    return init(NULL);
}

bool EGLCore::init(EGLContext shareContext) {
    EGLint numConfigs;

    const EGLint attribs[] = {
            EGL_BUFFER_SIZE, 32,
            EGL_ALPHA_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE};

    if((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY){
        LOGE("eglGetDisplay error!");
        return false;
    }

    if(!eglInitialize(display, 0, 0)){
        LOGE("eglInitialize error!");
        return false;
    }

    if(!eglChooseConfig(display, attribs, &config, 1, &numConfigs)){
        LOGE("eglChooseConfig error!");
        release();
        return false;
    }

    EGLint eglContextAttributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    context = eglCreateContext(display, config, NULL == shareContext ? EGL_NO_CONTEXT:shareContext,eglContextAttributes);
    if(!(context)){
        LOGE("eglCreateContext error!");
        release();
        return  false;
    }

    pfneglPresentationTimeANDROID = (PFNEGLPRESENTATIONTIMEANDROIDPROC) eglGetProcAddress("eglPresentationTimeANDROID");

    if(!pfneglPresentationTimeANDROID){
        LOGE("pfneglPresentationTimeANDROID error");
    }

    LOGI("EGLCore::init success! ");
    return true;
}