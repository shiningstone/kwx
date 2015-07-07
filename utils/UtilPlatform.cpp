
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

#define MAIN_ACTIVITY "org/cocos2dx/cpp/AppActivity"

#ifndef WIN32
static int CallJava(const char *jmethod,INT8U *retString) {
    JniMethodInfo method;
    bool IsMethodValid = false;

    jstring str; 
    IsMethodValid = JniHelper::getStaticMethodInfo(method,MAIN_ACTIVITY,jmethod,"()Ljava/lang/String;");
    if(IsMethodValid) {
        str = (jstring)method.env->CallStaticObjectMethod(method.classID,method.methodID);

        std::string text = JniHelper::jstring2string(str);
        memcpy(retString, text.c_str(), text.size());
    }
}

static int CallJavaNonStatic(const char *jmethod,const char *param) {
    JniMethodInfo method;
    bool IsMethodValid = false;
    jobject _instance;

    IsMethodValid = JniHelper::getStaticMethodInfo(method,MAIN_ACTIVITY,"getAppActivity","()Ljava/lang/Object;");
    if(IsMethodValid) {
        _instance = method.env->CallStaticObjectMethod(method.classID,method.methodID);
    }

    IsMethodValid = JniHelper::getMethodInfo(method,MAIN_ACTIVITY,jmethod,"(Ljava/lang/String;)V");
    if(IsMethodValid) {
        jobject str = method.env->NewStringUTF(param);
        method.env->CallVoidMethod(_instance, method.methodID, str);
    }
}
#endif

/****************************************************************
    设备信息接口
****************************************************************/
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


/****************************************************************
    升级接口
****************************************************************/
#include <IOSTREAM>
#ifndef WIN32
#include "cocos2d.h"
#include "../cocos2d/extensions/cocos-ext.h"
USING_NS_CC; 
USING_NS_CC_EXT;
#endif

void _write_file(const char *filename,std::vector<char>* buf) {
#ifndef WIN32/*ANDROID*/
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
#else/*WIN32*/
    printf("write to file %s",filename);
#endif
}

void _update_version(const char *apk) {
#ifndef WIN32/*ANDROID*/
    std::string path     = FileUtils::getInstance()->getWritablePath();     
    std::string fullPath = path + apk;

    CallJavaNonStatic("updateVersion",fullPath.c_str());
#else/*WIN32*/
    printf("%s",__FUNCTION__);
#endif
}

int _get_file_size(const char *filename) {
#ifndef WIN32
    std::string path     = FileUtils::getInstance()->getWritablePath();     
    std::string fullPath = path + filename;

    FILE* fp = fopen("c:/Test.txt", "rb");
    
    if(fp==NULL) {
        log("open file %s ERROR", fullPath.c_str());
       return 0;
    } else {
        fseek(fp,0,SEEK_END);
        return ftell(fp);
    }
#else
    printf("%s",__FUNCTION__);
	return 0;
#endif
}

#include <time.h>
int _get_system_time() {
#ifndef WIN32
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000 + tv.tv_usec/1000;
#else
    return GetTickCount();
#endif
}
/****************************************************************
    支付宝接口
****************************************************************/
void _ali_pay(const char *product,const char *desc,const char *price) {
#ifndef WIN32
    JniMethodInfo method;
    bool IsMethodValid = false;
    jobject _instance;

    IsMethodValid = JniHelper::getStaticMethodInfo(method,MAIN_ACTIVITY,"getAppActivity","()Ljava/lang/Object;");
    if(IsMethodValid) {
        _instance = method.env->CallStaticObjectMethod(method.classID,method.methodID);
    }

    IsMethodValid = JniHelper::getMethodInfo(method,MAIN_ACTIVITY,"pay","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if(IsMethodValid) {
        jobject p1 = method.env->NewStringUTF(product);
        jobject p2 = method.env->NewStringUTF(desc);
        jobject p3 = method.env->NewStringUTF(price);

        method.env->CallVoidMethod(_instance, method.methodID, p1, p2, p3);
    }
#else
    printf("%s : product %s, desc %s, price %s",__FUNCTION__, product, desc, price);
#endif
}

bool _ali_is_account_exist() {
#ifndef WIN32
    JniMethodInfo method;
    bool IsMethodValid = false;
    jobject _instance;

    IsMethodValid = JniHelper::getStaticMethodInfo(method,MAIN_ACTIVITY,"getAppActivity","()Ljava/lang/Object;");
    if(IsMethodValid) {
        _instance = method.env->CallStaticObjectMethod(method.classID,method.methodID);
    }

    IsMethodValid = JniHelper::getMethodInfo(method,MAIN_ACTIVITY,"check","()V");
    if(IsMethodValid) {
        method.env->CallVoidMethod(_instance, method.methodID);
    }

    _delay(1000);/* 等待ali执行完成，可能有进程同步问题 */

    bool isExist; 
    IsMethodValid = JniHelper::getStaticMethodInfo(method,MAIN_ACTIVITY,"getAccountStatus","()Z");
    if(IsMethodValid) {
        isExist = method.env->CallStaticBooleanMethod(method.classID,method.methodID);
        return isExist;
    } else {
        return false;
    }
#else
    printf("%s",__FUNCTION__);
    return true;
#endif
}

string _ali_get_sdk_version() {
#ifndef WIN32
    JniMethodInfo method;
    bool IsMethodValid = false;

    jstring str; 
    IsMethodValid = JniHelper::getStaticMethodInfo(method,MAIN_ACTIVITY,"getSDKVersion","()Ljava/lang/String;");
    if(IsMethodValid) {
        str = (jstring)method.env->CallStaticObjectMethod(method.classID,method.methodID);
        return JniHelper::jstring2string(str);
    }
#else
    printf("%s",__FUNCTION__);
    return string("win32_mock");
#endif
}

