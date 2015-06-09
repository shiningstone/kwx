
#include "./../network/NetMessenger.h"
#include "./../utils/DebugCtrl.h"

#include "MsgFormats.h"
#include "CommonMsg.h"
#include "KwxMessenger.h"

#include "DbgRequestDesc.h"

#ifndef NULL
#define NULL 0
#endif

static int _HANDLE_DS_PACKAGES(const INT8U *pkg, int &len);

NetMessenger *KwxMessenger::_messenger = 0;
Logger       *KwxMessenger::_logger = 0;

KwxMessenger::KwxMessenger() {
    _messenger = NetMessenger::getInstance();
    
    #if 0/*如果要用StartReceiving，不能调用Start*/
    _messenger->Start();
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
    DsMsg *aMsg = DsMsg::getInstance();
    aMsg->Dispatch(pkg,len);
	return 0;
}

/************************************************************
	Request (Upstream)
************************************************************/
int KwxMessenger::Send(UsMsg &aMsg) {
    INT8U buf[MSG_MAX_LEN] = {0};
    int   len = 0;
    
    len = aMsg.Serialize(buf);
    _messenger->Send(buf,len);

    char desc[512] = {0};
    aMsg.Desc(desc);
    LOGGER_WRITE("----------------> #%d: %s",++_sendCnt,desc);
    
    return 0;
}

/**********************************************************
	Interfaces
***********************************************************/
#include <stdio.h>
#include <string.h>

int RequestSendAction::Set(ActionId_t action,Card_t card) {
    SetRequestCode(REQ_GAME_SEND_ACTION);

    _act  = action;
    _card = card;
    
    INT32U actionId = _htonl(action);
    INT8U  cardKind = card;

    AddSeatInfo();
    _add_item( new Item((Item_t)134,4,(INT8U *)&actionId) );
    _add_item( new Item((Item_t)135,1,(INT8U *)&cardKind) );

    return 0;
}

int RequestSendAction::Set(ActionId_t action,int kindNum,Card_t card[]) {
    SetRequestCode(REQ_GAME_SEND_ACTION);

    _act  = action;
    
    INT32U actionId = _htonl(action);
    INT8U  cardKind[4] = {0};

    if(kindNum==1 && card==NULL) {
        cardKind[0] = CARD_UNKNOWN;
    } else {
        for(int i=0;i<kindNum;i++) {
            cardKind[i] = (INT8U)card[i];
        }
    }

    AddSeatInfo();
    _add_item( new Item((Item_t)134,4,(INT8U *)&actionId) );
    _add_item( new Item((Item_t)135,kindNum,(INT8U *)cardKind) );

    return 0;
}

void RequestSendAction::Desc(char *buf) const {
    UsMsg::Desc(buf);
    sprintf(buf+strlen(buf),"%s %s",DescAct(_act),DescCard(_card));
}

int RequestGameStart::Set() {
    SetRequestCode(REQ_GAME_SEND_START);
    AddSeatInfo();

    return 0;
}

int RequestShowCard::Set(Card_t card) {
    SetRequestCode(REQ_GAME_SEND_SHOWCARD);
    AddSeatInfo();
    _add_item( new Item((Item_t)65,card) );

    _card = card;
    
    return 0;
}

void RequestShowCard::Desc(char *buf) const {
    UsMsg::Desc(buf);
    sprintf(buf+strlen(buf),"handout %s",DescCard(_card));
}

int RequestTingInfo::Set() {
    SetRequestCode(REQ_GAME_GET_TINGINFO);
    AddSeatInfo();

    return 0;
}

int RequestTuoGuan::Set(bool status) {
    SetRequestCode(REQ_GAME_SEND_AGENT);
    AddSeatInfo();
    _add_item( new Item(61,(status?1:0)) );

    return 0;
}

