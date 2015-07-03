
#include "./../utils/UtilPlatform.h"
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

int VersionManager::_download() {
    _hm = new HTTPManager();  
    
    _hm->retain();
    _hm->setHttpDelegate(this);     
    _hm->sendGetRequest(_newVer.url.c_str(), VERSION_CODE);     

    return 0;
}

int VersionManager::_update() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _update_version(_newVer.name.c_str());
#endif

    return 0;
}

void VersionManager::onHttpManagerRequestCompleted(HttpClient *sender, HttpResponse *response) {
    if (strcmp(response->getHttpRequest()->getTag(), VERSION_CODE) == 0) {             
        LOGGER_WRITE("%s",VERSION_CODE);

        if(atoi(_newVer.size.c_str())==response->getResponseData()->size()) {
            _write_file(_newVer.name.c_str(),response->getResponseData());
        } else {
            LOGGER_WRITE("%s error image size",__FUNCTION__);
        }
    }                             
    
    if (strcmp(response->getHttpRequest()->getTag(), "update_mobile") == 0) {                           
        LOGGER_WRITE("%s","update_mobile");
    }
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
    _logger = LOGGER_REGISTER("VersionManager");
}

VersionManager::~VersionManager() {
    LOGGER_DEREGISTER(_logger);
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
