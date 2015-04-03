
#include "./../network/NetMessenger.h"

#include "MsgFormats.h"
#include "CommonMsg.h"
#include "KwxMessenger.h"

#ifndef NULL
#define NULL 0
#endif

static int _HANDLE_DS_PACKAGES(const INT8U *pkg, int &len);

NetMessenger *KwxMessenger::_messenger = 0;
Logger       *KwxMessenger::_logger = 0;

KwxMessenger::KwxMessenger() {
    _messenger = NetMessenger::getInstance();
    _messenger->Start();

    _logger = LOGGER_REGISTER("KwxMessenger");
}

KwxMessenger::~KwxMessenger() {
    LOGGER_DEREGISTER(_logger);
}

/************************************************************
	auto receiving (Downstream)
************************************************************/
void KwxMessenger::StartReceiving(MsgHandler_t handle) {
    _messenger->SetHandler(handle);
    _messenger->Start();
}

void KwxMessenger::StartReceiving() {
    if(_messenger==0) {
        _messenger = NetMessenger::getInstance();
    }

    if (_logger==0) {
        _logger = LOGGER_REGISTER("KwxMessenger");
    }

    _messenger->SetHandler(_HANDLE_DS_PACKAGES);
    _messenger->Start();
}

void KwxMessenger::StopReceiving() {
    _messenger->SetHandler(0);
    LOGGER_DEREGISTER(_logger);
}

int _HANDLE_DS_PACKAGES(const INT8U *pkg, int &len) {
    KwxDsMsg *aMsg = KwxDsMsg::getInstance();
    aMsg->Dispatch(pkg,len);
	return 0;
}

/************************************************************
	Request (Upstream)
************************************************************/
int KwxMessenger::Send(KwxUsMsg &aMsg) {
    INT8U buf[MSG_MAX_LEN] = {0};
    int   len = 0;
    
    len = aMsg.Serialize(buf);
    _messenger->Send(buf,len);

    return 0;
}

/**********************************************************
	Interfaces
***********************************************************/
int RequestSendAction::Set(ActionId_t action,Card_t card) {
    SetRequestCode(REQ_GAME_SEND_ACTION);
    
    INT32U actionId = _htonl(action);
    INT8U  cardKind = card;

    SetSeatInfo();
    _add_item( new Item((Item_t)134,4,(INT8U *)&actionId) );
    _add_item( new Item((Item_t)135,1,(INT8U *)&cardKind) );

    return 0;
}

int RequestGameStart::Set() {
    SetRequestCode(REQ_GAME_SEND_START);
    SetSeatInfo();

    return 0;
}

int RequestHandout::Set(Card_t card) {
    SetRequestCode(REQ_GAME_SEND_SHOWCARD);
    SetSeatInfo();
    _add_item( new Item((Item_t)65,card) );

    return 0;
}

int RequestTingInfo::Set() {
    SetRequestCode(REQ_GAME_GET_TINGINFO);
    SetSeatInfo();

    return 0;
}

