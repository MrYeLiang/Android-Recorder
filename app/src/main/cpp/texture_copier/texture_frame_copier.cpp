//
// Created by 叶亮 on 2019/7/17.
//


#include "texture_frame_copier.h"

TextureFrameCopier::TextureFrameCopier() {
}

TextureFrameCopier::~TextureFrameCopier() {
}

void TextureFrameCopier::destroy() {
    mIsInitialized = false;
    if (mGLProgId) {
        glDeleteProgram(mGLProgId);
        mGLProgId = 0;
    }
}