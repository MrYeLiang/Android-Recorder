//
// Created by 叶亮 on 2019/7/16.
//

#ifndef ANDROIDRECORDER_GPU_TEXTURE_FRAME_H
#define ANDROIDRECORDER_GPU_TEXTURE_FRAME_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "texture_frame.h"

class GPUTextureFrame: public TextureFrame {
private:
    GLuint decodeTexId;
    int initTexture();

public:
    GPUTextureFrame();
    virtual ~GPUTextureFrame();

    bool createTexture();
    bool bindTexture(GLint* uniformSamplers);
    void dealloc();

    GLuint getDecodeTexId() {
        return decodeTexId;
    };

};

#endif //ANDROIDRECORDER_GPU_TEXTURE_FRAME_H
