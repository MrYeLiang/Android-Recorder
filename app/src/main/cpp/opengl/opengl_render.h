//
// Created by 叶亮 on 2019/7/16.
//

#ifndef ANDROIDRECORDER_RENDER_H
#define ANDROIDRECORDER_RENDER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

static char* OUTPUT_VIEW_VERTEX_SHADER =
        "attribute vec4 position;    \n"
                "attribute vec2 texcoord;   \n"
                "varying vec2 v_texcoord;     \n"
                "void main(void)               \n"
                "{                            \n"
                "   gl_Position = position;  \n"
                "   v_texcoord = texcoord;  \n"
                "}                            \n";

static char* OUTPUT_VIEW_FRAG_SHADER =
        "varying highp vec2 v_texcoord;\n"
                "uniform sampler2D yuvTexSampler;\n"
                "void main() {\n"
                "  gl_FragColor = texture2D(yuvTexSampler, v_texcoord);\n"
                "}\n";

/**
 * Video OpenGL View
 */
class Render {
public:
    Render();
    virtual ~Render();
    bool init(int width, int height);
    void renderToViewWithAutofit(GLuint texID, int screenWidth, int screenHeight, int texWidth, int texHeight);
    void renderToAutoFitTexture(GLuint inputTexId, int width, int height, GLuint outputTexId);
    void dealloc();

protected:
    GLint _backingLeft;
    GLint _backingTop;
    GLint _backingWidth;
    GLint _backingHeight;

    char* mVertexShader;
    char* mFragmentShader;

    bool mIsInitialized;

    GLuint mGLProgId;
    GLuint mGLVertexCoords;
    GLuint mGLTextureCoords;
    GLint mGLUniformTexture;
    void checkGlError(const char* op);
    GLuint loadProgram(char* pVertexSource, char* pFragmentSource);
    GLuint loadShader(GLenum shaderType, const char* pSource);

    float calcCropRatio(int screenWidth, int screenHeight, int texWidth, int texHeight);
};
#endif //ANDROIDRECORDER_RENDER_H
