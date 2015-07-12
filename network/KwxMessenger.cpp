
#include <string.h>

#include "./../network/KwxEnv.h"
#include "./../network/NetMessenger.h"
#include "./../utils/DebugCtrl.h"
#include "./../protocol/MsgFormats.h"
#include "./../protocol/CommonMsg.h"
#include "./../protocol/DbgRequestDesc.h"

#include "KwxMessenger.h"

#ifndef NULL
#define NULL 0
#endif

static int _HANDLE_DS_PACKAGES(const INT8U *pkg, int &len);
std::vector<RequestId_t> KwxMessenger::_waitQueue;

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
    LOGGER_WRITE("server ip:%s, port:%d",_serverIp,_port);
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

int KwxMessenger::WaitQueueFind(RequestId_t req) {
    for(unsigned int i=0;i<_waitQueue.size();i++) {
        if(_waitQueue.at(i)==req) {
            return i;
        }
    }

    return INVALID;
}

bool KwxMessenger::IsWaiting(RequestId_t req) {
    if(req==REQ_INVALID) {
        return (_waitQueue.size()>0);
    }
    
    return (WaitQueueFind(req)!=INVALID);
}

bool KwxMessenger::Wait(RequestId_t req) {
    WaitQueueAdd(req);

    int waitCount = 0;
    while(_waitQueue.size()>0) {/*BUG : resume only all wait req are handled*/
        if(waitCount<30) {
            if((waitCount+1)%10==0) {
                //LOGGER_WRITE("wait %d second",(waitCount+1)/10);
            }
            _delay(1000);
            waitCount++;
        } else {
            LOGGER_WRITE("wait %s timeout",DescReq(req));
            _response = TIMEOUT;
            return false;
        }
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

FailureCode_t KwxMessenger::_response = REQUEST_ACCEPTED;
KwxMessenger *KwxMessenger::_instances[MSG_TYPE_MAX] = {0};

KwxMessenger *KwxMessenger::getInstance(MsgType_t type) {
    if(!_instances[type]) {
        _instances[type] = new KwxMessenger(type);
        _response = REQUEST_ACCEPTED;
    }

    return _instances[type];
}

void KwxMessenger::destroyInstances() {
    for(int i=0;i<MSG_TYPE_MAX;i++) {
        delete _instances[i];
        _instances[i] = NULL;
    }
}

int _HANDLE_DS_PACKAGES(const INT8U *pkg, int &len) {
    DsMsg *aMsg = DsMsg::getInstance();
    aMsg->Dispatch(pkg,len);
	return 0;
}

#include "./../protocol/DsInstruction.h"
#include "./../protocol/UsRequest.h"
#include "./../protocol/KwxMsgLogin.h"
int KwxMessenger::Send(RequestId_t req) {
    switch(req) {
        case REQ_LOGIN:
			{
    			RequestLogin aReq;
    			aReq.Set();
    			return Send(aReq);
			}
            
        case REQ_DAILY_LOGIN:
            {
                RequestDailyLogin aReq;
                aReq.Set();
                return Send(aReq);
            }
                
        case REQ_GET_DAILY_PRIZE:
            {
                RequestGetDailyPrize aReq;
                aReq.Set();
                return Send(aReq);
            }
                        
        case REQ_RES_UPDATE:
            {
                RequestResourceUpdate aReq;
                aReq.Set();
                return Send(aReq);
            }
            
        case REQ_VER_UPDATE:
            {
                RequestVersionUpdate aReq;
                aReq.Set();
                return Send(aReq);
            }
                    
        case REQ_GAME_SEND_RECONNECT:
			{
				RequestReconnect aReq;
				aReq.Set();
				return Send(aReq);
			}

        case REQ_GAME_SEND_ENTER:
            {
                RequestEnterRoom aReq;
                aReq.Set();
                return Send(aReq);
            }

        case REQ_GAME_SEND_START:
            {
                RequestGameStart aReq;
                aReq.Set();
                return Send(aReq);
            }

        case REQ_GAME_SEND_LEAVE_ROOM:
            {
                RequestLeave aReq;
                aReq.Set();
                return Send(aReq);
            }
        
		default:
			LOGGER_WRITE("%s unsupported request id %d",__FUNCTION__,req);
			return INVALID;
    }
}

int KwxMessenger::Send(ActionId_t action,Card_t card) {
    RequestSendAction aReq;
    aReq.Set(action,card);
    return Send(aReq);
}

int KwxMessenger::Send(ActionId_t code,int kindNum,Card_t kinds[]) {
    RequestSendAction aReq;
    aReq.Set(code,kindNum,kinds);
    return Send(aReq);
}


