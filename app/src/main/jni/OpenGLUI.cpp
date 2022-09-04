//
// Created by 陈祖杰 on 9/3/22_
//
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h> // ANativeWindow_fromSurfacec的头文件
#include <EGL/egl.h>
#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "auto_encrypt", __VA_ARGS__))

extern "C" void Java_com_jc_openglapplication_nativeFunc_OpenGLClass_onSurfaceCreateNative(JNIEnv *env, jobject surface) {
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
    EGLDisplay eglDisplay; /**< 系统显示 ID 或句柄。 */
    EGLSurface eglSurface; /**< 系统窗口或 frame buffer 句柄。 */
    EGLContext eglContext; /**< OpenGL ES 图形上下文。 */
    EGLConfig eglConfig;
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint eglConfigNum = 0;
    EGLint configSpec[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE
    };
    if (EGL_TRUE != eglChooseConfig(eglDisplay, configSpec, &eglConfig, 1, &eglConfigNum))
    {
        LOGD("eglChooseConfig failed!");
        return;
    }

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWindow, NULL);

    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };
    eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, ctxAttr);
    if (EGL_TRUE != eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext))
    {
        LOGD("eglMakeCurrent failed!");
        return;
    }
}