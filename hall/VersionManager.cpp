
#include "./../network/KwxMessenger.h"

#include "VersionManager.h"

#define RETURN_IF_FAIL(x) do { \
    if(x!=0) { \
        LOGGER_WRITE("%s fail : %s = %d",__FUNCTION__, #x, x); \
        return x; \
    } \
}while(0)

void VersionManager::onHttpManagerRequestCompleted(HttpClient *sender, HttpResponse *response) {

}

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
    return 0;
}

int VersionManager::_upgrade() {
    return 0;
}

int VersionManager::update() {
    int retcode = 0;
    
    retcode = _requestUpdate();
    RETURN_IF_FAIL(retcode);

    retcode = _download();
    RETURN_IF_FAIL(retcode);

    retcode = _upgrade();
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

