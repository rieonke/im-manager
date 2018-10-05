//
// Created by Rieon Ke on 2018/10/1.
//

#include "im_manager_jvm.h"
#include <im_manager.h>

/*
 * Class:     cn_rieon_tools_immanager_InputMethodManager
 * Method:    getCurrentInputMethod
 * Signature: ()Lcn/rieon/tools/immanager/InputMethod;
 */
jobject Java_cn_rieon_tools_immanager_InputMethodManager_getCurrentInputMethod(JNIEnv *env, jclass clz) {
    imm_input_source *is = imm_get_current_input_source();


    jclass input_source_class = (*env)->FindClass(env, "cn/rieon/tools/immanager/InputMethod");

    jmethodID constructor = (*env)->GetMethodID(env, input_source_class, "<init>", "()V");
    jmethodID setId = (*env)->GetMethodID(env, input_source_class, "setId", "(Ljava/lang/String;)V");
    jmethodID setName = (*env)->GetMethodID(env, input_source_class, "setName", "(Ljava/lang/String;)V");
    jmethodID setIcon = (*env)->GetMethodID(env, input_source_class, "setIcon", "(Ljava/lang/String;)V");

    jobject object = (*env)->NewObject(env, input_source_class, constructor);

    (*env)->CallVoidMethod(env, object, setId, (*env)->NewStringUTF(env, is->id));
    (*env)->CallVoidMethod(env, object, setName, (*env)->NewStringUTF(env, is->name));
    (*env)->CallVoidMethod(env, object, setIcon, (*env)->NewStringUTF(env, is->icon));

    imm_free_input_source(is);

    return object;
}

/*
 * Class:     cn_rieon_tools_immanager_InputMethodManager
 * Method:    getAvailableInputMethods
 * Signature: ()[Lcn/rieon/tools/immanager/InputMethod;
 */
jobjectArray Java_cn_rieon_tools_immanager_InputMethodManager_getAvailableInputMethods
        (JNIEnv *env, jclass clz) {

    imm_input_source_list *isl = imm_get_input_source_list();

    jobjectArray j_is_arr = NULL;
    jclass j_is_clz = (*env)->FindClass(env, "cn/rieon/tools/immanager/InputMethod");

    j_is_arr = (*env)->NewObjectArray(env, isl->count, j_is_clz, NULL);

    jmethodID constructor = (*env)->GetMethodID(env, j_is_clz, "<init>", "()V");
    jmethodID setId = (*env)->GetMethodID(env, j_is_clz, "setId", "(Ljava/lang/String;)V");
    jmethodID setName = (*env)->GetMethodID(env, j_is_clz, "setName", "(Ljava/lang/String;)V");
    jmethodID setIcon = (*env)->GetMethodID(env, j_is_clz, "setIcon", "(Ljava/lang/String;)V");

    for (size_t i = 0; i < isl->count; i++) {
        imm_input_source *p = isl->iss + i;
        jobject object = (*env)->NewObject(env, j_is_clz, constructor);
        (*env)->CallVoidMethod(env, object, setId, (*env)->NewStringUTF(env, p->id));
        (*env)->CallVoidMethod(env, object, setName, (*env)->NewStringUTF(env, p->name));
        (*env)->CallVoidMethod(env, object, setIcon, (*env)->NewStringUTF(env, p->icon));

        (*env)->SetObjectArrayElement(env, j_is_arr, (jsize) i, object);
    }

    imm_free_input_source_list(isl);

    return j_is_arr;
}

/*
 * Class:     cn_rieon_tools_immanager_InputMethodManager
 * Method:    switchTo
 * Signature: (Ljava/lang/String;)V
 */
jboolean Java_cn_rieon_tools_immanager_InputMethodManager_switchTo(JNIEnv *env, jclass clz, jstring id) {
    const char *chars = (*env)->GetStringUTFChars(env, id, 0);
    imm_bool res = imm_select(chars);
    return (jboolean) res;
}

/*
 * Class:     cn_rieon_tools_immanager_InputMethodManager
 * Method:    getInputMethod
 * Signature: (Ljava/lang/String;)Lcn/rieon/tools/immanager/InputMethod;
 */
jobject Java_cn_rieon_tools_immanager_InputMethodManager_getInputMethod(JNIEnv *env, jclass clz, jstring str) {

    imm_input_source *is = imm_get_input_source_by_id((*env)->GetStringUTFChars(env, str, 0));

    if (is == NULL) {
        return NULL;
    }

    jclass j_is_clz = (*env)->FindClass(env, "cn/rieon/tools/immanager/InputMethod");

    jmethodID constructor = (*env)->GetMethodID(env, j_is_clz, "<init>", "()V");
    jmethodID setId = (*env)->GetMethodID(env, j_is_clz, "setId", "(Ljava/lang/String;)V");
    jmethodID setName = (*env)->GetMethodID(env, j_is_clz, "setName", "(Ljava/lang/String;)V");
    jmethodID setIcon = (*env)->GetMethodID(env, j_is_clz, "setIcon", "(Ljava/lang/String;)V");

    jobject object = (*env)->NewObject(env, j_is_clz, constructor);

    (*env)->CallVoidMethod(env, object, setId, (*env)->NewStringUTF(env, is->id));
    (*env)->CallVoidMethod(env, object, setName, (*env)->NewStringUTF(env, is->name));
    (*env)->CallVoidMethod(env, object, setIcon, (*env)->NewStringUTF(env, is->icon));

    imm_free_input_source(is);

    return object;
}
