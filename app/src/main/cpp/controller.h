//
// Created by 叶亮 on 2019/7/11.
//

#ifndef ANDROIDRECORDER_CONTROLLER_H
#define ANDROIDRECORDER_CONTROLLER_H

#include <jni.h>
#include <android/native_window_jni.h>
#include <android/native_window_jni.h>
#include <sys/types.h>
#include "common/message_handler.h"
#include "egl/egl_core.h"
#include "opengl/opengl_render.h"
#include "common/preview_render.h"
#include "common/coomon_utils.h"

#define CAMERA_FACING_BACK   0
#define CAMERA_FACING_FRONT  1

enum RenderThreadMessage {
    MSG_RENDER_FRAME = 0,
    MSG_EGL_THREAD_CREATE,
    MSG_EGL_CREATE_PREVIEW_SURFACE,
    MSG_SWITCH_CAMERA,
    MSG_START_RECORDING,
    MSG_STOP_RECORDING,
    MSG_EGL_DESTROY_PREVIEW_SURFACE,
    MSG_EGL_THREAD_EXIT
};

class Controller{
public:

    Controller();

    virtual ~Controller();

    void prepareEGLContext(ANativeWindow *window, JavaVM *jvm, jobject obj,
                           int screenWidth, int screenHeight, int cameraFacingId);

    void notifyFrameAvaliable();

    void createWindowSurface(ANativeWindow *window);

    virtual bool init();

    virtual void destroy();

    void createPreviewSurface();

    void destroyEGLContext();

    void destroyPreviewSurface();

    void switchCamera();
    void switchCameraFromMessage();

    void startRecording();

    void stopRecording();

    void renderFrame();

    void draw();


protected:
    ANativeWindow *_window;
    JavaVM *jvm;
    jobject  obj;
    int screenWidth;
    int screenHeight;

    bool isInSwitchingCamera;

    int64_t  startTime;

    int facingId;
    int degress;
    int textureWidth;
    int textureHeight;

    int cameraWidth;
    int cameraHeight;

    Handler *handler;
    MessageQueue *queue;
    pthread_t _threadId;

    static void *threadStartCallBack(void *myself);

    void processMessage();

    EGLCore *eglCore;
    EGLSurface previewSurface;

    PreviewRender *render;

    virtual void buildRenderInstance();

    virtual void processVideoFrame(float position);

    void configCamera();

    void startCameraPreview();

    void updateTexImage();

    void releaseCamera();

    bool isThreadCreateSucceed;
    bool isEncoding;
};

class PreviewHandler:public Handler{

private:
    Controller *controller;

public:
    PreviewHandler(Controller *controller,MessageQueue *queue):Handler(queue){
        this->controller = controller;
    };

    void handleMessage(Message *msg){
        int what = msg->getWhat();
        switch (what) {
            case MSG_EGL_THREAD_CREATE:
                controller->init();
                break;
            case MSG_EGL_CREATE_PREVIEW_SURFACE:
                controller->createPreviewSurface();
                break;
            case MSG_START_RECORDING:
                controller->startRecording();
                break;
            case MSG_STOP_RECORDING:
                controller->stopRecording();
                break;
            case MSG_EGL_DESTROY_PREVIEW_SURFACE:
                controller->destroyPreviewSurface();
                break;
            case MSG_EGL_THREAD_EXIT:
                controller->destroy();
                break;
            case MSG_RENDER_FRAME:
                controller->renderFrame();
                break;
            case MSG_SWITCH_CAMERA:
                controller->switchCameraFromMessage();
                break;
        }
    }


};


#endif //ANDROIDRECORDER_CONTROLLER_H
