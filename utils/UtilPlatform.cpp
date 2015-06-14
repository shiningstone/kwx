
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

#ifndef WIN32
static int CallJava(INT8U *buf,const char *jmethod) {
    JniMethodInfo method;
    bool IsMethodValid = false;

    jstring str; 
    IsMethodValid = JniHelper::getStaticMethodInfo(method,"org/cocos2dx/cpp/AppActivity",jmethod,"()Ljava/lang/String;");
    if(IsMethodValid) {
        str = (jstring)method.env->CallStaticObjectMethod(method.classID,method.methodID);

        std::string text = JniHelper::jstring2string(str);
        memcpy(buf, text.c_str(), text.size());
    }
}
#endif

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
    CallJava(info.imsi, "getImsi");
    CallJava(info.resolution, "getResolution");
    CallJava(info.protoType, "getProtoType");
    CallJava(info.osVer, "getOsVer");
#endif
}

