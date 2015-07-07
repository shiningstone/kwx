
#include "./../utils/UtilPlatform.h"
#include "./../game/DataBase.h"
#include "./../network/KwxMessenger.h"
#include "VersionManager.h"

#include "./../utils/DebugCtrl.h"

#define RETURN_IF_FAIL(x) do { \
    if(x!=0) { \
        LOGGER_WRITE("%s fail : %s = %d",__FUNCTION__, #x, x); \
        return x; \
    } \
}while(0)

string VersionManager::getCurrentVerName() const {
    int  code = 0;     
    char name[32]      = {0};
    
    Database *aDatabase = Database::getInstance();
    aDatabase->GetVersion(code,name);

    return string(name);
}

VersionCode_t VersionManager::getNewVerCode() const {
    return _newVer.code;
}

string VersionManager::getNewVerName() const {
    return _newVer.name;
}

float VersionManager::getNewVerSize() const {
    return atof(_newVer.size.c_str());
}

int VersionManager::_requestUpdate() {
#if (DEBUG_ENTRANCE==3)
    return 0;
#else
    KwxMessenger *messenger = KwxMessenger::getInstance(MSG_LOGIN);
    messenger->Send(REQ_VER_UPDATE);

    if(messenger->_response==REQUEST_ACCEPTED) {

    }
    
    return messenger->_response;
#endif
}

int VersionManager::_download() {
    _curFileSize = 0;
    _curTime = _get_system_time();

    LOGGER_WRITE("%s (%d)",__FUNCTION__,_curTime);
    
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
        LOGGER_WRITE("%s (%d)",__FUNCTION__,_curTime);

        if(getNewVerSize()==response->getResponseData()->size()) {
            _write_file(_newVer.name.c_str(),response->getResponseData());
            _update();
            _recordVersion();
        } else {
            LOGGER_WRITE("%s error image size(Exp %d,Act %d)",
                __FUNCTION__, getNewVerSize(), response->getResponseData()->size());
        }
    }                             
    
    if (strcmp(response->getHttpRequest()->getTag(), "update_mobile") == 0) {                           
        LOGGER_WRITE("%s","update_mobile");
    }
}

int VersionManager::_recordVersion() {
    Database *aDatabase = Database::getInstance();
    aDatabase->SetVersion(_newVer.code,_newVer.name.c_str());
    return 0;
}

int VersionManager::upgrade() {
    int retcode = 0;
    
    retcode = _requestUpdate();
    RETURN_IF_FAIL(retcode);

    retcode = _download();
    RETURN_IF_FAIL(retcode);

    return retcode;
}

bool VersionManager::get_download_status(float &Kbps,float &percentage) {
    bool finish = false;

    float totalSize = getNewVerSize();
    int   newTime = _get_system_time();

    #if (DEBUG_ENTRANCE==3)
    float newSize = _curFileSize + totalSize/30;
    #else
    float newSize = _get_file_size(_newVer.name.c_str())/float(1024*1024);
    #endif
    
    Kbps = (newSize-_curFileSize)*(1024) / ((newTime-_curTime)/(float)1000);

    _curTime     = newTime;
    _curFileSize = newSize;

    if(_curFileSize>=totalSize) {
        percentage = 1.0;
        finish = true;
    } else {
        percentage = _curFileSize / totalSize;
        finish = false;
    }

    return finish;
}

VersionManager::VersionManager() {
    _logger = LOGGER_REGISTER("VersionManager");

    /*TEST DATA*/
    _newVer.code = 1;
    _newVer.name = "1.0.0.0";
    _newVer.size = "100MB";
	_newVer.url  = "http://120.25.169.221:8080/kwx.apk";
    _newVer.name = "kwx.apk";
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

