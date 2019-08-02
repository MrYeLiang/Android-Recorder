//
// Created by 叶亮 on 2019/7/16.
//

#ifndef ANDROIDRECORDER_YUV_TEXTURE_FRAME_H
#define ANDROIDRECORDER_YUV_TEXTURE_FRAME_H

#include "texture_frame.h"

class YUVTextureFrame: public TextureFrame {
private:
    GLuint textures[3];
    int initTexture();

    VideoFrame *frame;
public:
    YUVTextureFrame();
    virtual ~YUVTextureFrame();

    bool createTexture();
    void updateTexImage();
    bool bindTexture(GLint* uniformSamplers);
    void dealloc();
};
#endif //ANDROIDRECORDER_YUV_TEXTURE_FRAME_H
