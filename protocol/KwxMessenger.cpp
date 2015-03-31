
#include "KwxMessenger.h"
#include "MsgFormats.h"
#include "KwxMsg.h"

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
	auto receiving 
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
    KwxMsg aMsg(DOWN_STREAM);
    Logger *_logger = LOGGER_REGISTER("KwxMessenger");

    aMsg.Deserialize(pkg);

    LOGGER_WRITE("%s : %d\n",__FUNCTION__,aMsg.GetRequestCode());

    switch(aMsg.GetRequestCode()) {
        case REQ_GAME_SEND_DIST:
            return 0;

        case REQ_GAME_RECV_ACTION:
            return 0;

        case REQ_GAME_RECV_SHOWCARD:
            return 0;

        case REQ_GAME_RECV_RESPONSE:
            return 0;

        default:
            return KWX_INVALID_PCHC;
    }

    LOGGER_DEREGISTER(_logger);
}

/************************************************************
	Request (Upstream)
************************************************************/
int KwxMessenger::SetAction(ActionId_t code) {
    return 0;
}

int KwxMessenger::SetShowCard(CardType_t code) {
    return 0;
}

int KwxMessenger::SetReaction(ActionId_t code,CardType_t kind) {
    INT8U buf[MSG_MAX_LEN] = {0};
    int   len = 0;
    
    KwxMsg aMsg(UP_STREAM);
    len = aMsg.SetReaction(buf,code,kind);
    
    _messenger->Send(buf,len);

    return 0;
}

int KwxMessenger::SetReaction(ActionId_t code,int num,Card_t *kind) {
    return 0;
}

int KwxMessenger::SetRequestDistribute() {
    return 0;
}

int KwxMessenger::SetUpdateCardList(CARD *cards,int num) {
    return 0;
}
