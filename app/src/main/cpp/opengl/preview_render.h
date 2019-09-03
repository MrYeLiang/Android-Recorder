//
// Created by 叶亮 on 2019/7/16.
//

#ifndef ANDROIDRECORDER_PREVIEW_RENDER_H
#define ANDROIDRECORDER_PREVIEW_RENDER_H


#include <GLES2/gl2.h>
#include "gpu_texture_frame.h"
#include "../texture_copier/gpu_texture_frame_copier.h"
#include "opengl_render.h"

static GLfloat CAMERA_TRIANGLE_VERTICES[8] = {
        -1.0f, -1.0f,	// 0 top left
        1.0f, -1.0f,	// 1 bottom left
        -1.0f, 1.0f,  // 2 bottom right
        1.0f, 1.0f,	// 3 top right
};

static GLfloat CAMERA_TEXTURE_NO_ROTATION[8] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
};

static GLfloat CAMERA_TEXTURE_ROTATED_90[8] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f
};

static GLfloat CAMERA_TEXTURE_ROTATED_180[8] = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
};

static GLfloat CAMERA_TEXTURE_ROTATED_270[8] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
};

class PreviewRender {
public:
    PreviewRender();
    virtual ~PreviewRender();

    void init(int degress, bool isVFlip, int textureWidth, int textureHeight, int cameraWidth, int cameraHeight);
    void setDegress(int degress, bool isVFlip);
    void processFrame(float position);
    void drawToViewWithAutofit(int videoWidth, int videoHeight, int texWidth, int texHeight);
    void dealloc();

    int getCameraTexId();
    GLuint getInputTexId() {
        return inputTexId;
    };

protected:
    //在copy以及processor中的FBO
    GLuint FBO;
    //camera捕捉到的TextureFrame 但是是SamplerOES格式的
    GPUTextureFrame* cameraTexFrame;
    //利用mCopier转换为Sampler2D格式的inputTexId
    GLuint inputTexId;
    //用于旋转的纹理id
    GLuint rotateTexId;
    /** 1:把Camera的纹理拷贝到inputTexId **/
    GPUTextureFrameCopier* mCopier;
    /** 3:把outputTexId渲染到View上 **/
    Render* mRenderer;

    /** Camera Info From Android Camera **/
    int degress;
    bool isVFlip;
    GLfloat* textureCoords;
    int textureCoordsSize;
    int textureWidth;
    int textureHeight;
    int cameraWidth;
    int cameraHeight;

    void fillTextureCoords();
    float flip(float i);
    GLfloat* getVertexCoords();
};

#endif //ANDROIDRECORDER_PREVIEW_RENDER_H
