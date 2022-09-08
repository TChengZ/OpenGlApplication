package com.jc.openglapplication.nativeFunc;


import android.view.Surface;

public class OpenGLClass {

    {
        System.loadLibrary("OpenGLUI");
    }
    private static volatile OpenGLClass instance;

    private OpenGLClass() {

    }

    public static OpenGLClass getInstance() {
        if(null == instance) {
            synchronized (OpenGLClass.class) {
                if(null == instance) {
                    instance = new OpenGLClass();
                }
            }
        }
        return instance;
    }

    public native void onSurfaceCreateNative(Surface surface);
}
