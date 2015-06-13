
#ifdef WIN32
#include <windows.h>
#else
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
USING_NS_CC;
#endif

#include <stdio.h>

#include "UtilPlatform.h"

void _get_device_info(DeviceInfo_t &info) {
#ifdef WIN32
    memset(&info,0,sizeof(DeviceInfo_t));

    sprintf((char *)info.mac,"win32_mac");    
    sprintf((char *)info.imsi,"win32_imsi");    
    sprintf((char *)info.resolution,"1024*768");    
    sprintf((char *)info.protoType,"win32_protoType");    
    sprintf((char *)info.osVer,"win32_osVer");
#else
    //    typedef struct JniMethodInfo_
    //    {
    //        JNIEnv *    env;
    //        jclass      classID;
    //        jmethodID   methodID;
    //    } JniMethodInfo;
    JniMethodInfo method;

    bool ret = JniHelper::getStaticMethodInfo(method,"org/cocos2dx/cpp/DeviceInfo","test","()V");
    if(ret) {
        method.env->CallStaticVoidMethod(method.classID,method.methodID);
    }
#endif
}

