
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
    sprintf((char *)info.mac,"11:22:33:44:55:66");    
    sprintf((char *)info.imsi,"310260000000000");    
    sprintf((char *)info.resolution,"480*800");    
    sprintf((char *)info.protoType,"GT-I9100");    
    sprintf((char *)info.osVer,"2.3.5");
#else
    CallJava(info.mac, "getMac");
    CallJava(info.imsi, "getImsi");
    CallJava(info.resolution, "getResolution");
    CallJava(info.protoType, "getProtoType");
    CallJava(info.osVer, "getOsVer");
#endif
}

