
#include "MsgFormats.h"
#include "CommonMsg.h"
#include "DsInstruction.h"

#include "KwxMsgLogin.h"

int RequestLogin::Set() {
    SetRequestCode(REQ_LOGIN);

    _add_item( new Item(60,_userType) );
    
    _add_utf16_string(131,(const INT8U *)_account);
    _add_utf16_string(132,(const INT8U *)_mac);
    _add_utf16_string(133,(const INT8U *)_imsi);
    _add_utf16_string(134,(const INT8U *)_resolution);
    _add_utf16_string(135,(const INT8U *)_product);
    _add_utf16_string(136,(const INT8U *)_osVer);
    _add_utf16_string(137,(const INT8U *)_session);

    return 0;
}

int LoginResponse::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    INT8U status = msg.GetItemValue(0);
    if(status!=0) {
        return status;
    }

    _userType          = (UserType_t)msg.GetItemValue(1);
    _userActivated     = msg.GetItemValue(2);
    _reconnectRequired = msg.GetItemValue(3);
    _userId            = msg.GetItemValue(4);
    _key               = msg.GetItemValue(5);
    _roomServerId      = msg.GetItemValue(6);
    Utf16ToUtf8((const Utf16 *)msg._body->_items[7]->_buf,(Utf8 *)_roomServerIp);
    _roomServerPort    = msg.GetItemValue(8);

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

#include <string.h>
int EnterRoomResponse::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    roomPath = msg.GetItemValue(0);
    roomId   = msg.GetItemValue(1);
    tableId  = msg.GetItemValue(2);
    seat     = msg.GetItemValue(3);
    SeatInfo::getInstance()->Set(roomPath,roomId,tableId,seat);
    
    baseScore= msg.GetItemValue(4);
    
    playerNum= msg._body->_items[5]->_bufLen;
    
    for(int i=0;i<playerNum;i++) {
        status[i] = (msg._body->_items[5]->_buf[i]!=0);
        score[i]  = _ntohl( *(INT32U *)(msg._body->_items[6]->_buf + 4*i) );
        memcpy(name[i],msg._body->_items[7]->_buf,msg._body->_items[7]->_bufLen);
        memcpy(image[i],msg._body->_items[8]->_buf,msg._body->_items[8]->_bufLen);
    }
    
    return 0;
}

int EnterRoomNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat     = msg.GetItemValue(0);
    
    return 0;
}

int RequestReconnect::Set() {
    SetRequestCode(REQ_GAME_SEND_RECONNECT);

    INT32U value = _htons(_key);
    _add_item( new Item(130,4,(INT8U *)&value) );
    AddSeatInfo();

    return 0;
}

int ReconnectResponse::Construct(const DsMsg &msg) {
    return 0;
}

int ReconnectNotif::Construct(const DsMsg &msg) {
    _dir   = _GetPlayer(msg.GetItemValue(0));
    return 0;
}

int RequestLeave::Set() {
    SetRequestCode(REQ_GAME_SEND_LEAVE_ROOM);
    AddSeatInfo();
    
    return 0;
}

int LeaveResponse::Construct(const DsMsg &msg) {
    _score = msg.GetItemValue(0);
    return 0;
}

int LeaveNotif::Construct(const DsMsg &msg) {
    _dir    = _GetPlayer(msg.GetItemValue(0));
    _status = (LeaveStatus_t)msg.GetItemValue(1);
    return 0;
}

int LoginConflictNotif::Construct(const DsMsg &msg) {
    Utf16ToUtf8((const Utf16 *)msg._body->_items[0]->_buf,(Utf8 *)_info);
    return 0;
}

int RequestXiaPiao::Set(INT32U score) {
    SetRequestCode(REQ_GAME_SEND_XIA_PIAO);
    AddSeatInfo();

    INT32U value = _htonl(score);
    _add_item( new Item(134,4,(INT8U *)&value) );
    return 0;
}

int XiaPiaoResponse::Construct(const DsMsg &msg) {
    _score = msg.GetItemValue(0);
    return 0;
}

int XiaPiaoNotif::Construct(const DsMsg &msg) {
    _dir   = _GetPlayer(msg.GetItemValue(0));
    _score = msg.GetItemValue(1);
    return 0;
}

int RequestTalk::Set(const INT8U *content, TalkType_t type, PlayerDir_t dir) {
    SetRequestCode(REQ_GAME_SEND_MSG);
    AddSeatInfo();

    _add_item( new Item(61,_seatInfo->GetSeatId(_sendTo)) );
    _add_item( new Item(62,_type) );
    _add_utf16_string(134,(const INT8U *)_content);
    
    return 0;
}

int TalkResponse::Construct(const DsMsg &msg) {
    _status = msg.GetItemValue(0);
    return 0;
}

int TalkNotif::Construct(const DsMsg &msg) {
    _type = (TalkType_t)msg.GetItemValue(0);
    Utf16ToUtf8((const Utf16 *)msg._body->_items[1]->_buf,(Utf8 *)_content);
        
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
    _delay(_rate*2*100);/* to make sure the last send executed successfully */
    
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
    #if 0
    INT8U buf[MSG_MAX_LEN] = {0};
    int   len = 0;
    
    RequestHeartBeat aBeat;
    aBeat.Set();
    len = aBeat.Serialize(buf);

    while(_running) {
        _socket->Send((char *)buf,len);
        _delay(_rate*100);
    }
    #endif
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

