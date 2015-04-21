
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
    
   #if 0/*如果要用StartReceiving，不能调用Start*/
    _messenger->Start();
    #endif
    
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

    return 0;
}

/**********************************************************
	Interfaces
***********************************************************/
int RequestSendAction::Set(ActionId_t action,Card_t card) {
    SetRequestCode(REQ_GAME_SEND_ACTION);
    
    INT32U actionId = _htonl(action);
    INT8U  cardKind = card;

    AddSeatInfo();
    _add_item( new Item((Item_t)134,4,(INT8U *)&actionId) );
    _add_item( new Item((Item_t)135,1,(INT8U *)&cardKind) );

    return 0;
}

int RequestSendAction::Set(ActionId_t action,int kindNum,Card_t card[]) {
    SetRequestCode(REQ_GAME_SEND_ACTION);
    
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

int RequestGameStart::Set() {
    SetRequestCode(REQ_GAME_SEND_START);
    AddSeatInfo();

    return 0;
}

int RequestShowCard::Set(Card_t card) {
    SetRequestCode(REQ_GAME_SEND_SHOWCARD);
    AddSeatInfo();
    _add_item( new Item((Item_t)65,card) );

    return 0;
}

int RequestTingInfo::Set() {
    SetRequestCode(REQ_GAME_GET_TINGINFO);
    AddSeatInfo();

    return 0;
}

int RequestEnterRoom::Set(int id) {
    SetRequestCode(REQ_GAME_SEND_ENTER);

    Key_t key = EnvVariable::getInstance()->GetKey();
    RoomPath_t roomPath = EnvVariable::getInstance()->GetRoomPath(id);

    INT32U keyid = _htonl((INT32U)key);
    INT32U roomId = _htonl((INT32U)roomPath);
    
    _add_item( new Item((Item_t)132,4,(INT8U *)&keyid) );
    _add_item( new Item((Item_t)131,4,(INT8U *)&roomId) );

    return 0;
}

/*************************************************
    Heart beat
*************************************************/
#include "./../network/CSockets.h"
#include <thread>

KwxHeart::KwxHeart(int second) {
    _socket = new ClientSocket;
    _socket->Start();

    _rate = second;
    _running = true;

    std::thread t1(&KwxHeart::_Beats,this);
    t1.detach();
}

KwxHeart::~KwxHeart() {
    _running = false;
    _delay(_rate*2);/* to make sure the last send executed successfully */
    
    _socket->Stop();
    delete _socket;
}

void KwxHeart::SetRate(int second) {
    _rate = second;

    if(!_running) {
        _running = true;
        std::thread t1(&KwxHeart::_Beats,this);
        t1.detach();
    }
}

void KwxHeart::_Beats() {
    INT8U buf[MSG_MAX_LEN] = {0};
    int   len = 0;
    
    RequestHeartBeat aBeat;
    aBeat.Set();
    len = aBeat.Serialize(buf);

    while(_running) {
        _socket->Send((char *)buf,len);
        _delay(_rate);
    }
}

KwxHeart* KwxHeart::_instance = NULL;

KwxHeart *KwxHeart::getInstance(int second) {
    if (_instance==NULL) {
        _instance = new KwxHeart(second);
    }

    return _instance;
}

void KwxHeart::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

int RequestHeartBeat::Set() {
    SetRequestCode(HEART_BEAT);

    INT32U userId = _htonl(EnvVariable::getInstance()->GetUserId());
    _add_item( new Item((Item_t)131,4,(INT8U *)&userId) );
    
    return 0;
}
