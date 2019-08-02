package com.yeliang.recorder;

import android.app.Activity;
import android.content.Context;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Size;
import android.view.Surface;

import java.io.IOException;
import java.util.List;

/**
 * Author: yeliang
 * Date: 2019/7/9
 * Time: 4:11 PM
 * Description:
 */

public class VideoCamera {

    private int mVideoWidth;
    private int mVideoHeight;
    private int DEFAULT_VIDEO_HEIGHT = 720;
    private int DEFAULT_VIDEO_WIDTH = 480;

    private int mVideoFrameRate;

    private Context mContext;
    private Camera mCamera;
    private SurfaceTexture mTexture;

    public VideoCamera(Context context) {
        mContext = context;
    }

    public Camera getCamera() {
        return mCamera;
    }

    public CameraConfigInfo configCameraFromNative(int cameraFacingId) {
        if (null != mCamera) {
            releaseCamera();
        }

        if (cameraFacingId >= Camera.getNumberOfCameras()) {
            cameraFacingId = 0;
        }

        return setCamera(cameraFacingId);
    }

    public void releaseCamera() {
        if (mTexture != null) {
            mTexture.release();
            mTexture = null;
        }

        if (null != mCamera) {
            mCamera.setPreviewCallback(null);
            mCamera.release();
            mCamera = null;
        }
    }

    private CameraConfigInfo setCamera(int cameraFacingId) {
        setDefaultPreviewSize1280_720();

        //1 打开Camera
        mCamera = Camera.open(cameraFacingId);

        //2 设置预览图片的图像格式
        Camera.Parameters parameters = mCamera.getParameters();
        List<Integer> supportedPreviewFormats = parameters.getSupportedPreviewFormats();
        if (supportedPreviewFormats.contains(ImageFormat.NV21)) {
            parameters.setPreviewFormat(ImageFormat.NV21);
        }

        //3 设置预览图片的尺寸
        List<Size> suppportPreViewSize = parameters.getSupportedPreviewSizes();
        int previewWidth = mVideoWidth;
        int previewHeight = mVideoHeight;
        boolean isSupportPreviewSize = isSupportPreviewSize(suppportPreViewSize, previewWidth, previewHeight);

        if (isSupportPreviewSize) {
            parameters.setPreviewSize(previewWidth, previewHeight);
        } else {
            previewWidth = DEFAULT_VIDEO_WIDTH;
            previewHeight = DEFAULT_VIDEO_HEIGHT;
        }

        //4 设置视频记录的连续自动对焦模式
        if (parameters.getSupportedFocusModes().contains(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO)) {
            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
        }

        mCamera.setParameters(parameters);

        int degress = getCameraDisplayOrientation(cameraFacingId);

        return new CameraConfigInfo(cameraFacingId, degress, previewWidth, previewHeight);
    }

    private void setDefaultPreviewSize1280_720() {
        mVideoWidth = 1280;
        mVideoHeight = 1920;
        mVideoFrameRate = 24;
    }

    private boolean isSupportPreviewSize(List<Size> supportedPreviewSizes, int previewWidth, int previewHeight) {
        boolean isSupport = false;
        for (Size size : supportedPreviewSizes) {
            if (previewHeight == size.height && previewWidth == size.width) {
                isSupport = true;
            }
        }

        return isSupport;
    }

    private int getCameraDisplayOrientation(int cameraId) {
        int rotation = ((Activity) mContext).getWindowManager().getDefaultDisplay().getRotation();
        int degress = 0;

        switch (rotation) {
            case Surface.ROTATION_0:
                degress = 0;
                break;
            case Surface.ROTATION_90:
                degress = 90;
                break;
            case Surface.ROTATION_180:
                degress = 180;
                break;
            case Surface.ROTATION_270:
                degress = 270;
                break;

        }

        int result;
        CameraInfo info = new CameraInfo();
        Camera.getCameraInfo(cameraId, info);
        if (info.facing == CameraInfo.CAMERA_FACING_FRONT) {
            result = (info.orientation + degress) % 360;
        } else {
            result = (info.orientation - degress + 360) % 360;
        }

        return result;
    }

    public void setCameraPreviewTexture(int textureId) {
        mTexture = new SurfaceTexture(textureId);

        try {
            mCamera.setPreviewTexture(mTexture);
            mTexture.setOnFrameAvailableListener(new SurfaceTexture.OnFrameAvailableListener() {
                @Override
                public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                    if (null != mCallBack) {
                        mCallBack.notifyFrameAvailable();
                    }
                }
            });
            mCamera.startPreview();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void updateTexImage() {
        if (mTexture != null) {
            mTexture.updateTexImage();
        }
    }

    private VideoCameraCallBack mCallBack;

    public void setCallBack(VideoCameraCallBack callBack) {
        mCallBack = callBack;
    }

    public interface VideoCameraCallBack {
        void onPermissionDismiss(String tip);

        void notifyFrameAvailable();
    }
}
