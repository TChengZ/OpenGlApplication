package com.jc.openglapplication;

import android.app.Activity;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

import com.jc.openglapplication.nativeFunc.OpenGLClass;

public class MainActivity extends Activity implements SurfaceHolder.Callback2{

    private SurfaceView mSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mSurfaceView = findViewById(R.id.sv_main);
        if(null != mSurfaceView) {
            mSurfaceView.setZOrderMediaOverlay(true);
            mSurfaceView.getHolder().setFormat(PixelFormat.RGBA_8888);
            mSurfaceView.getHolder().addCallback(MainActivity.this);
        }
    }

    @Override
    public void surfaceRedrawNeeded(@NonNull SurfaceHolder surfaceHolder) {

    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
        OpenGLClass.getInstance().onSurfaceCreateNative(surfaceHolder.getSurface());
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
        OpenGLClass.getInstance().onSurfaceDestroyNative();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(null != mSurfaceView) {
            mSurfaceView.getHolder().removeCallback(MainActivity.this);
        }
    }


}