package com.yeliang.recorder;

/**
 * Author: yeliang
 * Date: 2019/7/9
 * Time: 4:24 PM
 * Description:
 */

public class CameraConfigInfo {
    private int cameraFacingId;
    private int degress;
    private int textureWidth;
    private int textureHeight;

    public CameraConfigInfo(int cameraFacingId, int degress, int textureWidth, int textureHeight) {
        this.cameraFacingId = cameraFacingId;
        this.degress = degress;
        this.textureWidth = textureWidth;
        this.textureHeight = textureHeight;
    }

    public int getCameraFacingId() {
        return cameraFacingId;
    }

    public void setCameraFacingId(int cameraFacingId) {
        this.cameraFacingId = cameraFacingId;
    }

    public int getDegress() {
        return degress;
    }

    public void setDegress(int degress) {
        this.degress = degress;
    }

    public int getTextureWidth() {
        return textureWidth;
    }

    public void setTextureWidth(int textureWidth) {
        this.textureWidth = textureWidth;
    }

    public int getTextureHeight() {
        return textureHeight;
    }

    public void setTextureHeight(int textureHeight) {
        this.textureHeight = textureHeight;
    }
}
