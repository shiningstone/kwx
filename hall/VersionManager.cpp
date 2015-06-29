
#include "./../network/KwxMessenger.h"

#include "VersionManager.h"

#define RETURN_IF_FAIL(x) do { \
    if(x!=0) { \
        LOGGER_WRITE("%s fail : %s = %d",__FUNCTION__, #x, x); \
        return x; \
    } \
}while(0)

VersionCode_t VersionManager::getCurrentVerCode() const {
    return 0;
}

string VersionManager::getCurrentVerName() const {
    return 0;
}

int VersionManager::_requestUpdate() {
    KwxMessenger *messenger = KwxMessenger::getInstance(MSG_LOGIN);
    messenger->Send(REQ_VER_UPDATE);

    if(messenger->_response==REQUEST_ACCEPTED) {

    }
    
    return messenger->_response;
}
    
void VersionManager::onHttpManagerRequestCompleted(HttpClient *sender, HttpResponse *response) {
    if (strcmp(response->getHttpRequest()->getTag(), VERSION_CODE) == 0) {             
        LOGGER_WRITE("%s",VERSION_CODE);

        _hm->writeFileFromRequest(response,"qwe.json");             
    
        std::string version = "version";             
        std::string table   = "version";             
    
        if (!(strcmp(version.c_str(), table.c_str()) == 0)) {
        }                                     
    }                             
    
    if (strcmp(response->getHttpRequest()->getTag(), "update_mobile") == 0) {                           
        LOGGER_WRITE("%s","update_mobile");
    }
}

int VersionManager::_download() {
    _hm = new HTTPManager();  
    
    _hm->retain();
    _hm->setHttpDelegate(this);     
    _hm->sendGetRequest(VERSION_PAGE, VERSION_CODE);     

    return 0;
}

int VersionManager::_update() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
#endif

    return 0;
}

int VersionManager::upgrade() {
    int retcode = 0;
    
    retcode = _requestUpdate();
    RETURN_IF_FAIL(retcode);

    retcode = _download();
    RETURN_IF_FAIL(retcode);

    retcode = _update();
    RETURN_IF_FAIL(retcode);

    return retcode;
}

VersionManager::VersionManager() {
}

VersionManager* VersionManager::_instance = NULL;

VersionManager *VersionManager::getInstance() {
    if (_instance==NULL) {
        _instance = new VersionManager();
    }

    return _instance;
}

void VersionManager::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

