//
// Created by 叶亮 on 2019/7/17.
//

#ifndef ANDROIDRECORDER_GPU_TEXTURE_FRAME_COPIER_H
#define ANDROIDRECORDER_GPU_TEXTURE_FRAME_COPIER_H


#include "texture_frame_copier.h"
#include "../egl/gl_tools.h"
#include "../matrix.h"

static char* GPU_FRAME_FRAGMENT_SHADER =
        "#extension GL_OES_EGL_image_external : require\n"
                "precision mediump float;\n"
                "uniform samplerExternalOES yuvTexSampler;\n"
                "varying vec2 yuvTexCoords;\n"
                "void main() {\n"
                "  gl_FragColor = texture2D(yuvTexSampler, yuvTexCoords);\n"
                "}\n";

class GPUTextureFrameCopier : public TextureFrameCopier{
public:
    GPUTextureFrameCopier();
    virtual ~GPUTextureFrameCopier();

    virtual bool init();
    virtual void renderWithCoords(TextureFrame* textureFrame, GLuint texId, GLfloat* vertexCoords, GLfloat* textureCoords);
protected:
    GLint mGLUniformTexture;
};

#endif //ANDROIDRECORDER_GPU_TEXTURE_FRAME_COPIER_H
