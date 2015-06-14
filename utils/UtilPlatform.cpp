
#ifdef WIN32
#include <windows.h>
#else
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
USING_NS_CC;
#endif

#include <stdio.h>
#include <string.h>

#include "UtilPlatform.h"

static int CallJava(INT8U *buf,const char *jmethod) {
    JniMethodInfo method;
    jobject obj;

    bool ret = JniHelper::getStaticMethodInfo(method,"org/cocos2dx/cpp/DeviceInfo",jmethod,"()Ljava/lang/String;");
    if(ret) {
        obj = method.env->CallStaticObjectMethod(method.classID,method.methodID);
    }

    const char *sBuf = method.env->GetStringUTFChars(static_cast<jstring>(obj),NULL);
    memcpy(buf, sBuf, strlen(sBuf));
}

void _get_device_info(DeviceInfo_t &info) {
    memset(&info,0,sizeof(DeviceInfo_t));

#ifdef WIN32
    sprintf((char *)info.mac,"win32_mac");    
    sprintf((char *)info.imsi,"win32_imsi");    
    sprintf((char *)info.resolution,"1024*768");    
    sprintf((char *)info.protoType,"win32_protoType");    
    sprintf((char *)info.osVer,"win32_osVer");
#else
    CallJava(info.mac, "getMac");
#endif
}

