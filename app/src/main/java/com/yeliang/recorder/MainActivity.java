package com.yeliang.recorder;

import android.Manifest;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.PermissionChecker;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;

import static android.support.v4.content.PermissionChecker.PERMISSION_DENIED;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    static {
        System.loadLibrary("native-lib");
    }

    private RecorderView mSurfaceView;
    private VideoCamera mCamera;
    private PreviewScheduler mScheduler;

    private Button btnStartRecord;
    private Button btnScrollSwitch;

    private boolean hasStart = false;


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //去掉标题栏
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
        //全屏，隐藏状态
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN
        );

        setContentView(R.layout.activity_main);
        initView();
        initData();
    }

    private void initView() {
        mSurfaceView = findViewById(R.id.recorder_view);
        btnStartRecord = findViewById(R.id.btn_start_record);
        btnStartRecord.setOnClickListener(this);

        btnScrollSwitch = findViewById(R.id.btn_camera_switch);
        btnScrollSwitch.setOnClickListener(this);
    }

    private void initData() {
        initPermission();
    }

    /*权限相关*/
    private void initPermission() {
        int result = PermissionChecker.checkSelfPermission(this, Manifest.permission.CAMERA);
        if (result == PERMISSION_DENIED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CAMERA}, 1000);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_start_record:
                hasStart = !hasStart;

                if (hasStart) {
                    btnStartRecord.setText("停止");
                    startRecord();
                } else {
                    btnStartRecord.setText("开始录制");
                    stopRecord();
                }
                break;

            case R.id.btn_camera_switch:
                mScheduler.switchCamera();
                break;
            default:
                break;
        }
    }



    private void stopRecord() {

    }

    private void startRecord() {
        mCamera = new VideoCamera(this);
        mScheduler = new PreviewScheduler(mCamera, mSurfaceView);
    }
}
