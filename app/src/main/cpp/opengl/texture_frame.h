//
// Created by 叶亮 on 2019/7/16.
//

#ifndef ANDROIDRECORDER_TEXTURE_FRAME_H
#define ANDROIDRECORDER_TEXTURE_FRAME_H

#include <GLES2/gl2.h>

class TextureFrame {
protected:
    bool checkGlError(const char* op);

public:
    TextureFrame();
    virtual ~TextureFrame();

    virtual bool createTexture() = 0;
    virtual bool bindTexture(GLint* uniformSamplers) = 0;
    virtual void dealloc() = 0;
};

#endif //ANDROIDRECORDER_TEXTURE_FRAME_H
