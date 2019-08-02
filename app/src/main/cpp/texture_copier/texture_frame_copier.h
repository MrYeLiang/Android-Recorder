//
// Created by 叶亮 on 2019/7/17.
//

#ifndef ANDROIDRECORDER_TEXTURE_FRAME_COPIER_H
#define ANDROIDRECORDER_TEXTURE_FRAME_COPIER_H

#include "../opengl/texture_frame.h"

static char* NO_FILTER_VERTEX_SHADER =
        "attribute vec4 vPosition;\n"
                "attribute vec4 vTexCords;\n"
                "varying vec2 yuvTexCoords;\n"
                "uniform highp mat4 texMatrix;\n"
                "uniform highp mat4 trans; \n"
                "void main() {\n"
                "  yuvTexCoords = (texMatrix*vTexCords).xy;\n"
                "  gl_Position = trans * vPosition;\n"
                "}\n";

class TextureFrameCopier {
public:
    TextureFrameCopier();
    virtual ~TextureFrameCopier();

    virtual bool init() = 0;
    virtual void renderWithCoords(TextureFrame* textureFrame, GLuint texId, GLfloat* vertexCoords, GLfloat* textureCoords) = 0;
    virtual void destroy();

protected:
    char* mVertexShader;
    char* mFragmentShader;

    bool mIsInitialized;
    GLuint mGLProgId;
    GLuint mGLVertexCoords;
    GLuint mGLTextureCoords;

    GLint mUniformTexMatrix;
    GLint mUniformTransforms;
};

#endif //ANDROIDRECORDER_TEXTURE_FRAME_COPIER_H
