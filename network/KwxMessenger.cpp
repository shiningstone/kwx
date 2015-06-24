
#include <string.h>

#include "./../network/KwxEnv.h"
#include "./../network/NetMessenger.h"
#include "./../utils/DebugCtrl.h"
#include "./../protocol/MsgFormats.h"
#include "./../protocol/CommonMsg.h"

#include "KwxMessenger.h"

#ifndef NULL
#define NULL 0
#endif

static int _HANDLE_DS_PACKAGES(const INT8U *pkg, int &len);

Logger       *KwxMessenger::_logger = 0;

KwxMessenger::KwxMessenger(MsgType_t type) {
    EnvVariable *env = EnvVariable::getInstance();
    
    switch(type) {
        case MSG_LOGIN:
            strcpy((char *)_serverIp,env->_loginServer.ipaddr);
            _port = env->_loginServer.port;
            break;
        case MSG_GAME:
            strcpy((char *)_serverIp,env->_roomServer.ipaddr);
            _port = env->_roomServer.port;
            break;
        case MSG_CHAT:
            break;
    }
    
	_waitQueue.clear();

    #if 0/*如果要用StartReceiving，不能调用Start*/
    Start((char *)_serverIp,_port);
    #endif

    _sendCnt = 0;
    
    _logger = LOGGER_REGISTER("KwxMessenger");
}

KwxMessenger::~KwxMessenger() {
    LOGGER_DEREGISTER(_logger);
}

/************************************************************
	auto receiving (Downstream)
************************************************************/
void KwxMessenger::StartReceiving(MsgHandler_t handle) {
    SetHandler(handle);
    Start((char *)_serverIp,_port);
}

void KwxMessenger::StartReceiving() {
    SetHandler(_HANDLE_DS_PACKAGES);
    Start((char *)_serverIp,_port);
}

void KwxMessenger::StopReceiving() {
    SetHandler(0);
}

int KwxMessenger::Send(UsMsg &aMsg, bool ignoreRsp) {
    INT8U buf[MSG_MAX_LEN] = {0};
    int   len = 0;
    
    len = aMsg.Serialize(buf);
    NetMessenger::Send(buf,len);

    char desc[512] = {0};
    aMsg.Desc(desc);
    LOGGER_WRITE("----------------> #%d: %s",++_sendCnt,desc);

    if(!ignoreRsp) {
        Wait((RequestId_t)aMsg._header->_requestCode);
    }
    
    return 0;
}

void KwxMessenger::WaitQueueAdd(RequestId_t req) {
    _waitQueue.push_back(req);
}

int KwxMessenger::WaitQueueFind(RequestId_t req) const {
    for(int i=0;i<_waitQueue.size();i++) {
        if(_waitQueue.at(i)==req) {
            return i;
        }
    }

    return INVALID;
}

bool KwxMessenger::IsWaiting(RequestId_t req) const {
    if(req==REQ_INVALID) {
        return (_waitQueue.size()>0);
    }
    
    return (WaitQueueFind(req)!=INVALID);
}

bool KwxMessenger::Wait(RequestId_t req) {
    WaitQueueAdd(req);
    
    while(_waitQueue.size()>0) {/*BUG : resume only all wait req are handled*/
        _delay(100);
    }

    return true;
}

void KwxMessenger::Resume(RequestId_t req) {
    int pos = WaitQueueFind(req);
    
    if(pos!=INVALID) {
        _waitQueue.erase(_waitQueue.begin()+pos);
    } else {
        _waitQueue.clear();
    }
}

int _HANDLE_DS_PACKAGES(const INT8U *pkg, int &len) {
    DsMsg *aMsg = DsMsg::getInstance();
    aMsg->Dispatch(pkg,len);
	return 0;
}

