package com.yeliang.recorder;

import android.content.Context;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Author: yeliang
 * Date: 2019/7/9
 * Time: 3:30 PM
 * Description:
 */

public class RecorderView extends SurfaceView implements SurfaceHolder.Callback {

    private RecorderViewCallBack mCallBack;

    private int mHeight;
    private int mWidth;
    private Surface mSurface;

    public RecorderView(Context context, AttributeSet attrs) {
        super(context, attrs);

        SurfaceHolder surfaceHolder = getHolder();
        surfaceHolder.addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        mSurface = getHolder().getSurface();
        mHeight = getWidth();
        mWidth = getHeight();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        if (null != mCallBack) {
            mCallBack.resetRenderSize(width, height);
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        if (null != mCallBack) {
            mCallBack.destroySurface();
        }
    }

    public void setCallBack(RecorderViewCallBack callBack){
        mCallBack = callBack;
        if (null != mCallBack) {
            mCallBack.createSurface(mSurface, mWidth, mHeight);
        }

    }

    public interface RecorderViewCallBack {
        void createSurface(Surface surface, int width, int height);

        void resetRenderSize(int width, int height);

        void destroySurface();
    }
}
