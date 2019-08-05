//
// Created by 叶亮 on 2019/7/11.
//

#include "controller.h"
#include "log_utils.h"

Controller::Controller() {
    facingId = CAMERA_FACING_FRONT;

    startTime = -1;
    eglCore= NULL;
    _window = NULL;
    isThreadCreateSucceed = false;
    previewSurface = EGL_NO_SURFACE;
    isEncoding = false;
    queue = new MessageQueue("controller message queue");
    handler = new PreviewHandler(this,queue);
}

Controller::~Controller() {

}

bool Controller::init() {
    LOGI("Controller::init start!");

    eglCore = new EGLCore();
    eglCore->init();
    createPreviewSurface();
    buildRenderInstance();
    configCamera();
    render->init(degress, facingId == CAMERA_FACING_FRONT, textureWidth, textureHeight, cameraWidth, cameraHeight);
    startCameraPreview();

    isInSwitchingCamera = false;

    LOGI("Controller::init finish!");
    return true;
}

void Controller::destroy(){

}

void Controller::createPreviewSurface() {
    previewSurface = eglCore->createWindowSurface(_window);
    if(previewSurface != NULL){
        eglCore->makeCurrent(previewSurface);
    }
}

void Controller::destroyPreviewSurface(){

}

void Controller::switchCamera(){
    isInSwitchingCamera = true;
    if(facingId == CAMERA_FACING_BACK){
        facingId = CAMERA_FACING_FRONT;
    } else {
        facingId = CAMERA_FACING_BACK;
    }

    if(handler){
        handler->postMessage(new Message(MSG_SWITCH_CAMERA));
    }
}

void Controller::switchCameraFromMessage()
{
    releaseCamera();
    configCamera();
    render->setDegress(degress, facingId == CAMERA_FACING_FRONT);
    startCameraPreview();
    isInSwitchingCamera = false;
}

void Controller::releaseCamera(){
    JNIEnv *env;

    if(jvm->AttachCurrentThread(&env, NULL) != JNI_OK){
        return;
    }

    if(env == NULL){
        return;
    }

    jclass jcls = env->GetObjectClass(obj);
    if(NULL != jcls){
        jmethodID  releaseCameraCallback = env->GetMethodID(jcls, "releaseCameraFromNative","()V");
        if(NULL != releaseCameraCallback){
            env->CallVoidMethod(obj, releaseCameraCallback);
        }
    }

    jvm->DetachCurrentThread();
}

void Controller::startRecording(){

}

void Controller::stopRecording(){

}

void Controller::prepareEGLContext(ANativeWindow *window, JavaVM *jvm, jobject obj, int screenWidth,
                                   int screenHeight, int cameraFacingId) {

    LOGI("Controller::prepare start!");
    this->jvm = jvm;
    this->obj = obj;
    this->_window = window;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->facingId = cameraFacingId;

    handler->postMessage(new Message(MSG_EGL_THREAD_CREATE));
    int res_code = pthread_create(&_threadId, 0, threadStartCallBack, this);
    if(res_code == 0){
        isThreadCreateSucceed = true;
    }

    LOGI("Controller::prepare finish!");
}

void *Controller::threadStartCallBack(void *myself) {
    LOGI("Controller::threadStartCallBack start");
    Controller *controller = (Controller *) myself;
    controller->processMessage();
    pthread_exit(0);
    LOGI("Controller::threadStartCallBack finish");
}

void Controller::processMessage() {
    bool renderingEnabled = true;
    while(renderingEnabled){
        Message *msg = NULL;
        if(queue->dequeueMessage(&msg, true) > 0){
            if(MESSAGE_QUEUE_LOOP_QUIT_FLAG == msg->execute()){
                renderingEnabled = false;
            }
            delete msg;
        }
    }
}

void Controller::configCamera() {
    LOGI("Controller::configCamera enter");
    JNIEnv *env;

    if(env == NULL){
        LOGE("getJNIEnv failed");
        return;
    }

    if(jvm->AttachCurrentThread(&env, NULL) != JNI_OK){
        LOGE("AttachCurrentThread failed");
        return;
    }

    jclass jcls = env->GetObjectClass(obj);
    if(NULL == jcls){
        return;
    }

    //(I)Lcom/yeliang/recorder/CameraConfigInfo") ---> (I)是形参，后面的是返回值
    jmethodID configCameraCallback = env->GetMethodID(jcls, "configCameraFromNative", "(I)Lcom/yeliang/recorder/CameraConfigInfo;");
    if(configCameraCallback == NULL){
        return;
    }

    //1 调用PreviewScheduler类中的configCameraFromNative方法 返回CameraConfigInfo对象
    jobject  cameraConfigInfo = env->CallObjectMethod(obj, configCameraCallback, facingId);

    //2 根据CameraConfigInfo对象，获取CameraConfigInfo类
    jclass CameraConfigInfo_cls = env->GetObjectClass(cameraConfigInfo);

    //3 通过CameraConfigInfo_cls 获取 CameraConfigInfo中的getDegress方法
    jmethodID getDegress_method = env->GetMethodID(CameraConfigInfo_cls, "getDegress","()I");
    degress = env->CallIntMethod(cameraConfigInfo, getDegress_method);

    jmethodID getCamereFacingId_method = env->GetMethodID(CameraConfigInfo_cls, "getCameraFacingId","()I");
    facingId = env->CallIntMethod(cameraConfigInfo, getCamereFacingId_method);
    
    jmethodID getTextureWidth_method = env->GetMethodID(CameraConfigInfo_cls, "getTextureWidth","()I");
    int previewWidth = env->CallIntMethod(cameraConfigInfo, getTextureWidth_method);
    cameraWidth = previewWidth;

    jmethodID getTextureHeight_method = env->GetMethodID(CameraConfigInfo_cls, "getTextureHeight","()I");
    int previewHeight = env->CallIntMethod(cameraConfigInfo, getTextureHeight_method);
    cameraHeight = previewHeight;

    cameraWidth = 1280;
    cameraHeight = 720;

    textureWidth = 720;
    textureHeight = 1280;

    LOGI("camera:{previewWidth = %d, previewHeight = %d}", cameraWidth, cameraHeight);
    LOGI("Texture: {textureWidth = %d, textureHeight = %d}",textureWidth, textureHeight);

    jvm->DetachCurrentThread();
    LOGI("Controller::configCamera leave");
}

void Controller::startCameraPreview() {
    JNIEnv *env;
    if(jvm->AttachCurrentThread(&env, NULL) != JNI_OK){
        return;
    }

    if(env == NULL){
        return;
    }

    jclass jcls = env->GetObjectClass(obj);

    if(NULL != jcls){
        jmethodID  startPreviewMethod = env->GetMethodID(jcls, "startPreviewFromNative", "(I)V");
        if(NULL != startPreviewMethod){
            env->CallVoidMethod(obj, startPreviewMethod, render->getCameraTexId());
        }
    }

    if(jvm->DetachCurrentThread() != JNI_OK){
        LOGE("DetachCurrentThread Error!");
        return;
    }
}

void Controller::createWindowSurface(ANativeWindow *window) {
    if(_window == NULL){
        _window = window;
        if(handler){
            handler->postMessage(new Message(MSG_EGL_CREATE_PREVIEW_SURFACE));
        }
    }
}

void Controller::notifyFrameAvaliable()
{
        if(handler && !isInSwitchingCamera){
            handler->postMessage(new Message(MSG_RENDER_FRAME));
        }
}

void Controller::renderFrame()
{
        if(NULL != eglCore && !isInSwitchingCamera){
                if(startTime == -1){
                    startTime = getCurrentTime();
                }

            float position = ((float)(getCurrentTime() - startTime)) / 1000.0f;
            processVideoFrame(position);
            if(previewSurface != EGL_NO_SURFACE){
                draw();
            }

            if(isEncoding){
            }
        }
}

void Controller::processVideoFrame(float position)
{
    updateTexImage();
    render->processFrame(position);
}

void Controller::updateTexImage()
{
    JNIEnv *env;
    if(jvm->AttachCurrentThread(&env, NULL) != JNI_OK){
        return;
    }

    if(env == NULL){
        return;
    }

    jclass jcls = env->GetObjectClass(obj);

    printf("Controller::updateTexImage &jcls = %p", &jcls);
    if(NULL != jcls){
        jmethodID updateTexImageCallback = env->GetMethodID(jcls, "updateTexImageFromNative", "()V");
        if(NULL != updateTexImageCallback){
            env->CallVoidMethod(obj, updateTexImageCallback);
        }
    }

    if(jvm->DetachCurrentThread() != JNI_OK){
        LOGE("%s: DetachCurrentThread failed", __FUNCTION__);
    }
}

void Controller::draw()
{
    eglCore->makeCurrent(previewSurface);
    render->drawToViewWithAutofit(screenWidth, screenHeight, textureWidth, textureHeight);
    if(!eglCore -> swapBuffers(previewSurface)){
        LOGE("(eglCore -> swapBuffers(previewSurface) Error!");
    }
}

void Controller::buildRenderInstance() {
    render = new PreviewRender();
}


