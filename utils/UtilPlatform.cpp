
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
static int CallJava(const char *jmethod,INT8U *retString) {
    JniMethodInfo method;
    bool IsMethodValid = false;

    jstring str; 
    IsMethodValid = JniHelper::getStaticMethodInfo(method,"org/cocos2dx/cpp/AppActivity",jmethod,"()Ljava/lang/String;");
    if(IsMethodValid) {
        str = (jstring)method.env->CallStaticObjectMethod(method.classID,method.methodID);

        std::string text = JniHelper::jstring2string(str);
        memcpy(retString, text.c_str(), text.size());
    }
}

static int CallJava(const char *jmethod) {
    JniMethodInfo method;
    bool IsMethodValid = false;
    jobject _instance;

    IsMethodValid = JniHelper::getStaticMethodInfo(method,"org/cocos2dx/cpp/AppActivity","getAppActivity","()Ljava/lang/Object;");
    if(IsMethodValid) {
        _instance = method.env->CallStaticObjectMethod(method.classID,method.methodID);
    }

    IsMethodValid = JniHelper::getMethodInfo(method,"org/cocos2dx/cpp/AppActivity",jmethod,"()V");
    if(IsMethodValid) {
        method.env->CallVoidMethod(_instance, method.methodID);
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
    CallJava("getMac", info.mac);
    CallJava("getImsi", info.imsi);
    CallJava("getResolution", info.resolution);
    CallJava("getProtoType", info.protoType);
    CallJava("getOsVer", info.osVer);
#endif
}

#include <IOSTREAM>

#ifdef WIN32
void _write_file(const char *filename,std::vector<char>* buf) {
    printf("write to file %s",filename);
}

void _update_version() {
    printf("%s",__FUNCTION__);
}

#else
#include "cocos2d.h"
#include "../cocos2d/extensions/cocos-ext.h"
USING_NS_CC; 
USING_NS_CC_EXT; 

void _write_file(const char *filename,std::vector<char>* buf) {
    std::string path     = FileUtils::getInstance()->getWritablePath();     
    std::string fullPath = path + filename;     

    FILE* fp = fopen(fullPath.c_str(), "wb");
    log("write to file %s(%x)", fullPath.c_str(), (int)fp);
    if(fp==NULL) {
        return;
    }
    
    for (unsigned int i=0; i<buf->size(); i++) {
        unsigned char bf = buf->at(i);
        fwrite(&bf, 1, 1, fp);
    }
    
    fclose(fp);
}

void _update_version() {
    CallJava("updateVersion");
}

#endif



