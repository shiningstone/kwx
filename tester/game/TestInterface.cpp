
#include <IOSTREAM>

#include "cocos2d.h"
#include "../cocos2d/extensions/cocos-ext.h"
USING_NS_CC; 
USING_NS_CC_EXT; 

#include "./../../utils/UtilPlatform.h"
#include "./../../network/KwxHttpManager.h"
#include "./../../hall/VersionManager.h"

void test_write_file() {
    vector<char> *buf = new vector<char>();

    for(int i=0;i<10;i++) {
        char c = '0'+i;
        buf->push_back(c);
    }
    
    _write_file("testfile",buf);

    std::string path     = FileUtils::getInstance()->getWritablePath();     
    std::string fullPath = path + "testfile";     

    FILE *fp = fopen(fullPath.c_str(), "rb");
    if(fp!=NULL) {
        char buf[512]  = {0};
        char bytes[64] = {0};
        
        int len = fread(bytes,1,64,fp);
        for(int i=0;i<len;i++) {
            sprintf(buf+3*i, "%02x ",bytes[i]);
        }
        log("first 64 bytes: %s", buf);
    } else {
        log("open %s fail",fullPath.c_str());
    }
    
    fclose(fp);    
}


class HttpTester : public HTTPManagerDelegate {
public:
    virtual void onHttpManagerRequestCompleted(HttpClient *sender, HttpResponse *response); 
};

void HttpTester::onHttpManagerRequestCompleted(HttpClient *sender, HttpResponse *response) {
    log("tag:%s",response->getHttpRequest()->getTag());
    _write_file("testHttpGetResponse",response->getResponseData());
}

void test_http_get() {
    log("test_http_get");
    HTTPManager *hm = new HTTPManager();
    hm->setHttpDelegate(new HttpTester());     
    hm->sendGetRequest( "http://www.baidu.com","" );
}

void test_upgrade() {
    _update_version("kwx.apk");
}

void test_download_version() {
    VersionManager *vm = VersionManager::getInstance();
    vm->_download();
}

void test_update_version() {
    VersionManager *vm = VersionManager::getInstance();
    vm->_update();
}

void test_interface_upgrade() {
    #if 0
    test_http_get();
    test_write_file();
    test_upgrade();
    #endif
    test_download_version();
    //test_update_version();
}

void test_sdk_version() {
    string version = _ali_get_sdk_version();
    log("ali sdk is %s",version.c_str());
}

void test_check_account() {
    bool isExist = _ali_is_account_exist();
    log("ali account exists %d",isExist);
    
}

void test_pay() {
    _ali_pay("test_product","test_desc","0.01");
}

void test_interface_alipay() {
    test_sdk_version();
    test_check_account();
    test_pay();
}

void test_interface() {
    test_interface_upgrade();
    //test_interface_alipay();
}

