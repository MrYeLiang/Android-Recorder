//
// Created by 叶亮 on 2019/7/16.
//

#include "gpu_texture_frame.h"
#include "../log_utils.h"

GPUTextureFrame::GPUTextureFrame() {
    decodeTexId = 0;
}

GPUTextureFrame::~GPUTextureFrame() {

}

bool GPUTextureFrame::createTexture() {
    LOGI("enter GPUTextureFrame::createTexture");
    decodeTexId = 0;
    int ret = initTexture();
    if (ret < 0) {
        LOGI("init texture failed...");
        this->dealloc();
        return false;
    }
    return true;
}

bool GPUTextureFrame::bindTexture(GLint* uniformSamplers) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, decodeTexId);
    glUniform1i(*uniformSamplers, 0);
    return true;
}

void GPUTextureFrame::dealloc() {
    LOGI("enter GPUTextureFrame::dealloc");
    if (decodeTexId) {
        glDeleteTextures(1, &decodeTexId);
    }
}

int GPUTextureFrame::initTexture() {
    glGenTextures(1, &decodeTexId);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, decodeTexId);
    if (checkGlError("glBindTexture")) {
        return -1;
    }
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (checkGlError("glTexParameteri")) {
        return -1;
    }
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if (checkGlError("glTexParameteri")) {
        return -1;
    }
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    if (checkGlError("glTexParameteri")) {
        return -1;
    }
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if (checkGlError("glTexParameteri")) {
        return -1;
    }
    return 1;
}