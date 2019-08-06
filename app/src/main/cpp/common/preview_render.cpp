//
// Created by 叶亮 on 2019/7/16.
//


#include <string.h>
#include "preview_render.h"
#include "../log_utils.h"
#include "../egl/gl_tools.h"

class VideoGLSurfaceRender;

PreviewRender::PreviewRender() {
    degress = 270;
    cameraTexFrame = NULL;
    mCopier = NULL;
    textureCoords = NULL;
    textureCoordsSize = 8;
    mRenderer = NULL;
}

PreviewRender::~PreviewRender() {
}

void PreviewRender::setDegress(int degress, bool isVFlip) {
    this->degress = degress;
    this->isVFlip = isVFlip;
    this->fillTextureCoords();
}

void PreviewRender::init(int degress, bool isVFlip, int textureWidth, int textureHeight, int cameraWidth, int cameraHeight) {
    LOGI("enter PreviewRender::init() textureWidth is %d, textureHeight is %d", textureWidth, textureHeight);
    LOGI("enter PreviewRender::init() cameraWidth is %d, cameraHeight is %d", cameraWidth, cameraHeight);
    this->degress = degress;
    this->isVFlip = isVFlip;
    textureCoords = new GLfloat[textureCoordsSize];
    this->fillTextureCoords();
    this->textureWidth = textureWidth;
    this->textureHeight = textureHeight;
    this->cameraWidth = cameraWidth;
    this->cameraHeight = cameraHeight;

    mCopier = new GPUTextureFrameCopier();
    mCopier->init();
    mRenderer = new Render();
    mRenderer->init(textureWidth, textureHeight);
    cameraTexFrame = new GPUTextureFrame();
    cameraTexFrame->createTexture();
    glGenTextures(1, &inputTexId);
    checkGlError("glGenTextures inputTexId");
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    checkGlError("glBindTexture inputTexId");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLint internalFormat = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (GLsizei) textureWidth, (GLsizei) textureHeight, 0, internalFormat, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &FBO);
    checkGlError("glGenFramebuffers");

    glGenTextures(1, &rotateTexId);
    checkGlError("glGenTextures rotateTexId");
    glBindTexture(GL_TEXTURE_2D, rotateTexId);
    checkGlError("glBindTexture rotateTexId");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (degress == 90 || degress == 270)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cameraHeight, cameraWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cameraWidth, cameraHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    LOGI("leave PreviewRender::init()");
}

void PreviewRender::processFrame(float position) {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    checkGlError("glBindFramebuffer FBO");

    if (degress == 90 || degress == 270)
        glViewport(0, 0, cameraHeight, cameraWidth);
    else
        glViewport(0, 0, cameraWidth, cameraHeight);

    GLfloat* vertexCoords = this->getVertexCoords();
    mCopier->renderWithCoords(cameraTexFrame, rotateTexId, vertexCoords, textureCoords);

    int rotateTexWidth = cameraWidth;
    int rotateTexHeight = cameraHeight;
    if (degress == 90 || degress == 270){
        rotateTexWidth = cameraHeight;
        rotateTexHeight = cameraWidth;
    }
    mRenderer->renderToAutoFitTexture(rotateTexId, rotateTexWidth, rotateTexHeight, inputTexId);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void PreviewRender::drawToViewWithAutofit(int videoWidth, int videoHeight, int texWidth, int texHeight) {
    mRenderer->renderToViewWithAutofit(inputTexId, videoWidth, videoHeight, texWidth, texHeight);
}

int PreviewRender::getCameraTexId() {
    if (cameraTexFrame) {
        return cameraTexFrame->getDecodeTexId();
    }
    return -1;
}

void PreviewRender::dealloc(){
    LOGI("enter PreviewRender::dealloc()");
    if(mCopier){
        mCopier->destroy();
        delete mCopier;
        mCopier = NULL;
    }
    LOGI("after delete mCopier");
    if(mRenderer) {
        LOGI("delete mRenderer..");
        mRenderer->dealloc();
        delete mRenderer;
        mRenderer = NULL;
    }
    LOGI("after delete mRenderer");
    if (inputTexId) {
        LOGI("delete inputTexId ..");
        glDeleteTextures(1, &inputTexId);
    }
    if(FBO){
        LOGI("delete FBO..");
        glDeleteBuffers(1, &FBO);
    }
    if (cameraTexFrame != NULL){
        cameraTexFrame->dealloc();
        delete cameraTexFrame;
        cameraTexFrame = NULL;
    }

    if(textureCoords){
        delete[] textureCoords;
        textureCoords = NULL;
    }
    LOGI("leave PreviewRender::dealloc()");
}

void PreviewRender::fillTextureCoords() {
    GLfloat* tempTextureCoords;
    switch (degress) {
        case 90:
            tempTextureCoords = CAMERA_TEXTURE_ROTATED_90;
            break;
        case 180:
            tempTextureCoords = CAMERA_TEXTURE_ROTATED_180;
            break;
        case 270:
            tempTextureCoords = CAMERA_TEXTURE_ROTATED_270;
            break;
        case 0:
        default:
            tempTextureCoords = CAMERA_TEXTURE_NO_ROTATION;
            break;
    }
    memcpy(textureCoords, tempTextureCoords, textureCoordsSize * sizeof(GLfloat));
    if(isVFlip){
        textureCoords[1] = flip(textureCoords[1]);
        textureCoords[3] = flip(textureCoords[3]);
        textureCoords[5] = flip(textureCoords[5]);
        textureCoords[7] = flip(textureCoords[7]);
    }
}

float PreviewRender::flip(float i) {
    if (i == 0.0f) {
        return 1.0f;
    }
    return 0.0f;
}

GLfloat* PreviewRender::getVertexCoords() {
    return CAMERA_TRIANGLE_VERTICES;
}