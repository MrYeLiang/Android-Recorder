//
// Created by 叶亮 on 2019/7/16.
//
#include "opengl_render.h"
#include "../log_utils.h"
#include <stdlib.h>

Render::Render() {
    mVertexShader = OUTPUT_VIEW_VERTEX_SHADER;
    mFragmentShader = OUTPUT_VIEW_FRAG_SHADER;
}

Render::~Render() {
}

bool Render::init(int width, int height) {
    this->_backingLeft = 0;
    this->_backingTop = 0;
    this->_backingWidth = width;
    this->_backingHeight = height;
    mGLProgId = loadProgram(mVertexShader, mFragmentShader);
    if (!mGLProgId) {
        LOGE("Could not create program.");
        return false;
    }
    mGLVertexCoords = glGetAttribLocation(mGLProgId, "position");
    checkGlError("glGetAttribLocation vPosition");
    mGLTextureCoords = glGetAttribLocation(mGLProgId, "texcoord");
    checkGlError("glGetAttribLocation vTexCords");
    mGLUniformTexture = glGetUniformLocation(mGLProgId, "yuvTexSampler");
    checkGlError("glGetAttribLocation yuvTexSampler");

    mIsInitialized = true;
    return true;
}

void Render::renderToView(GLuint texID, int screenWidth, int screenHeight) {
    glViewport(0, 0, screenWidth, screenHeight);

    if (!mIsInitialized) {
        LOGE("ViewRenderEffect::renderEffect effect not initialized!");
        return;
    }

    glUseProgram (mGLProgId);
    static const GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    static const GLfloat texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(mGLTextureCoords);

    /* Binding the input texture */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(mGLUniformTexture, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(mGLVertexCoords);
    glDisableVertexAttribArray(mGLTextureCoords);
    glBindTexture(GL_TEXTURE_2D, 0);
}

float Render::calcCropRatio(int screenWidth, int screenHeight, int texWidth, int texHeight) {
    int fitHeight = (int)((float)texHeight*screenWidth/texWidth+0.5f);

    float ratio = (float)(fitHeight-screenHeight)/(2*fitHeight);

    return ratio;
}

// 肯定是竖屏录制
void Render::renderToViewWithAutofit(GLuint texID, int screenWidth, int screenHeight, int texWidth, int texHeight) {
    glViewport(0, 0, screenWidth, screenHeight);

    if (!mIsInitialized) {
        LOGE("ViewRenderEffect::renderEffect effect not initialized!");
        return;
    }

    float cropRatio = calcCropRatio(screenWidth, screenHeight, texWidth, texHeight);

    if (cropRatio < 0)
        cropRatio = 0.0f;

    glUseProgram(mGLProgId);
    static const GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
                                         1.0f, 1.0f };
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    static const GLfloat texCoords[] = { 0.0f, cropRatio, 1.0f, cropRatio, 0.0f, 1.0f-cropRatio,
                                         1.0f, 1.0f-cropRatio };
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(mGLTextureCoords);

    /* Binding the input texture */
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(mGLUniformTexture, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(mGLVertexCoords);
    glDisableVertexAttribArray(mGLTextureCoords);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Render::renderToAutoFitTexture(GLuint inputTexId, int width, int height, GLuint outputTexId) {
    float textureAspectRatio = (float)height / (float)width;
    float viewAspectRatio = (float)_backingHeight / (float)_backingWidth;
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    if(textureAspectRatio > viewAspectRatio){
        //Update Y Offset
        int expectedHeight = (int)((float)height*_backingWidth/(float)width+0.5f);
        yOffset = (float)(expectedHeight-_backingHeight)/(2*expectedHeight);
//		LOGI("yOffset is %.3f", yOffset);
    } else if(textureAspectRatio < viewAspectRatio){
        //Update X Offset
        int expectedWidth = (int)((float)(height * _backingWidth) / (float)_backingHeight + 0.5);
        xOffset = (float)(width - expectedWidth)/(2*width);
//		LOGI("xOffset is %.3f", xOffset);
    }

    glViewport(_backingLeft, _backingTop, _backingWidth, _backingHeight);

    if (!mIsInitialized) {
        LOGE("ViewRenderEffect::renderEffect effect not initialized!");
        return;
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    checkGlError("PassThroughRender::renderEffect glFramebufferTexture2D");
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGI("failed to make complete framebuffer object %x", status);
    }

    glUseProgram(mGLProgId);
    const GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    static const GLfloat texCoords[] = { xOffset, yOffset, (GLfloat)(1.0 - xOffset), yOffset, xOffset, (GLfloat)1.0 - yOffset,
                                         (GLfloat)(1.0 - xOffset), (GLfloat)(1.0 - yOffset) };
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(mGLTextureCoords);

    /* Binding the input texture */
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    glUniform1i(mGLUniformTexture, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(mGLVertexCoords);
    glDisableVertexAttribArray(mGLTextureCoords);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}



void Render::renderToTexture(GLuint inputTexId, GLuint outputTexId) {
    glViewport(_backingLeft, _backingTop, GLsizei(_backingWidth), GLsizei(_backingHeight));

    if (!mIsInitialized) {
        LOGE("ViewRenderEffect::renderEffect effect not initialized!");
        return;
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    checkGlError("PassThroughRender::renderEffect glFramebufferTexture2D");
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGI("failed to make complete framebuffer object %x", status);
    }

    glUseProgram (mGLProgId);
    static const GLfloat _vertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    static const GLfloat texCoords[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(mGLTextureCoords);

    /* Binding the input texture */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    glUniform1i(mGLUniformTexture, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(mGLVertexCoords);
    glDisableVertexAttribArray(mGLTextureCoords);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

void Render::checkGlError(const char* op){
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGE("after %s() glError (0x%x)\n", op, error);
    }
}

void Render::dealloc() {
    mIsInitialized = false;
    glDeleteProgram(mGLProgId);
}

void Render::renderToVFlipTexture(GLuint inputTexId, GLuint outputTexId) {
    glViewport(_backingLeft, _backingTop, GLsizei(_backingWidth), GLsizei(_backingHeight));

    if (!mIsInitialized) {
        LOGE("ViewRenderEffect::renderEffect effect not initialized!");
        return;
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    checkGlError("PassThroughRender::renderEffect glFramebufferTexture2D");
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGI("failed to make complete framebuffer object %x", status);
    }

    glUseProgram (mGLProgId);
    static const GLfloat _vertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
    glVertexAttribPointer(mGLVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
    glEnableVertexAttribArray(mGLVertexCoords);
    static const GLfloat texCoords[] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
    glVertexAttribPointer(mGLTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(mGLTextureCoords);

    /* Binding the input texture */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    glUniform1i(mGLUniformTexture, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(mGLVertexCoords);
    glDisableVertexAttribArray(mGLTextureCoords);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

GLuint Render::loadProgram(char* pVertexSource, char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }
    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }
    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGI("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint Render::loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGI("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
            } else {
                LOGI( "Guessing at GL_INFO_LOG_LENGTH size\n");
                char* buf = (char*) malloc(0x1000);
                if (buf) {
                    glGetShaderInfoLog(shader, 0x1000, NULL, buf);
                    LOGI("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
            }
            glDeleteShader(shader);
            shader = 0;
        }
    }
    return shader;
}