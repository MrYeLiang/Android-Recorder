package com.yeliang.recorder;

import android.hardware.Camera.CameraInfo;
import android.view.Surface;


/**
 * Author: yeliang
 * Date: 2019/7/10
 * Time: 7:16 PM
 * Description:
 */

public class PreviewScheduler implements VideoCamera.VideoCameraCallBack, RecorderView.RecorderViewCallBack {

    private VideoCamera mCamera;
    private RecorderView mPreviewView;

    private boolean isFirst = true;
    private int mDefaultCameraFacingId = CameraInfo.CAMERA_FACING_FRONT;

    public PreviewScheduler(VideoCamera camera, RecorderView previewView) {
        mCamera = camera;
        mPreviewView = previewView;

        mCamera.setCallBack(this);
        mPreviewView.setCallBack(this);
    }

    private void startPreview(Surface surface, int width, int height, final int cameraFacingId) {
        if (isFirst) {
            prepareEGLContext(surface, width, height, cameraFacingId);
            isFirst = false;
        } else {
            createWindowSurface(surface);
        }
    }

    private CameraConfigInfo mConfigIfo;

    public native void prepareEGLContext(Surface surface, int width, int height, int cameraFacingId);

    public native void createWindowSurface(Surface surface);

    public native void switchCamera();

    public native void destroyEGLContext();

    //jni回调 获取CameraConfigInfo
    public CameraConfigInfo configCameraFromNative(int cameraFacingId) {
        mDefaultCameraFacingId = cameraFacingId;
        mConfigIfo = mCamera.configCameraFromNative(cameraFacingId);
        return mConfigIfo;
    }

    //jni回调 传递指定texture开始预览
    public void startPreviewFromNative(int textureId) {
        mCamera.setCameraPreviewTexture(textureId);
    }

    //jni回调
    public void updateTexImageFromNative(){
        mCamera.updateTexImage();
    }

    //jin回调
    public void releaseCameraFromNative(){
        mCamera.releaseCamera();
    }

    @Override
    public void onPermissionDismiss(String tip) {

    }

    @Override
    public native void notifyFrameAvailable();


    @Override
    public void createSurface(Surface surface, int width, int height) {
        startPreview(surface, width, height, mDefaultCameraFacingId);
    }

    @Override
    public void resetRenderSize(int width, int height) {

    }

    @Override
    public void destroySurface() {

    }

}
