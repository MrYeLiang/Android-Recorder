//
// Created by 叶亮 on 2019/7/16.
//


#include "texture_frame.h"
#include "../log_utils.h"

TextureFrame::TextureFrame() {

}

TextureFrame::~TextureFrame() {

}

bool TextureFrame::checkGlError(const char* op) {
    GLint error;
    for (error = glGetError(); error; error = glGetError()) {
        LOGI("error::after %s() glError (0x%x)\n", op, error);
        return true;
    }
    return false;
}