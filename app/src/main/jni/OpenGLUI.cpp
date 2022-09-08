//
// Created by 陈祖杰 on 9/3/22_
//
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h> // ANativeWindow_fromSurfacec的头文件
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "auto_encrypt", __VA_ARGS__))

#define JNI_CURRENT_VERSION JNI_VERSION_1_4

JavaVM* gGlobalVM = NULL;

extern "C" void onSurfaceCreate(JNIEnv *env, jobject clazz, jobject surface) {
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
    EGLDisplay eglDisplay; /**< 系统显示 ID 或句柄。 */
    EGLSurface eglSurface; /**< 系统窗口或 frame buffer 句柄。 */
    EGLContext eglContext; /**< OpenGL ES 图形上下文。 */
    EGLConfig eglConfig;
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY); // 获取 EGLDisplay 对象，建立与本地窗口系统的连接
    EGLint eglConfigNum = 0;
    EGLint configSpec[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE
    };
    if (EGL_TRUE != eglChooseConfig(eglDisplay, configSpec, &eglConfig, 1, &eglConfigNum)) // 获取 EGLConfig 对象，确定渲染表面的配置信息
    {
        LOGD("eglChooseConfig failed!");
        return;
    }

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWindow, NULL); // 创建渲染表面 EGLSurface

    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };
    eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, ctxAttr); // 创建渲染上下文 EGLContext
    // 通过 eglMakeCurrent 方法将 EGLSurface、EGLContext、EGLDisplay 三者绑定，绑定成功之后 OpenGLES 环境就创建好了，接下来便可以进行渲染
    if (EGL_TRUE != eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext))
    {
        LOGD("eglMakeCurrent failed!");
        return;
    }
    // 绘制功能，这里指简单绘制背景
    glClearColor(1.0, 1.0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    // 交换缓冲
    eglSwapBuffers(eglDisplay, eglSurface);
}
#define METHOD_LEN(x) ((int) (sizeof(x) / sizeof((x)[0])))
#define javaClass "com/jc/openglapplication/nativeFunc/OpenGLClass"

static const JNINativeMethod nativeMethods[] =
{
        { "onSurfaceCreateNative", "(Landroid/view/Surface;)V", (void*) onSurfaceCreate },
};


int registerNativeMethod(JNIEnv* env)
{
    jclass clazz = env->FindClass(javaClass);
    return env->RegisterNatives(clazz, nativeMethods, METHOD_LEN(nativeMethods));
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOGD("JNI_OnLoad");
    JNIEnv *env;
    jint r;
    gGlobalVM = vm;
    r = vm->GetEnv((void **) &env, JNI_CURRENT_VERSION);
    if (r != JNI_OK)
        return r;

    registerNativeMethod(env);

    return JNI_CURRENT_VERSION;
}