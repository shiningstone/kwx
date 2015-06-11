
#include <string.h>

#include "MsgFormats.h"
#include "CommonMsg.h"
#include "DsInstruction.h"

#include "KwxMsgLogin.h"

int RequestLogin::Set() {
    _get_device_info(_device);

    SetRequestCode(REQ_LOGIN);

    _add_item( new Item(60,_userType) );
    
    _add_utf16_string(131,(const INT8U *)_account);
    _add_utf16_string(132,_device.mac);
    _add_utf16_string(133,_device.imsi);
    _add_utf16_string(134,_device.resolution);
    _add_utf16_string(135,_device.protoType);
    _add_utf16_string(136,_device.osVer);
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
    
    INT32U roomId      = msg.GetItemValue(6);
    INT8U  roomIp[32] = {0};
    msg.GetString(7,roomIp);
    INT32U roomPort    = msg.GetItemValue(8);

    EnvVariable *env = EnvVariable::getInstance();
    env->SetUserId(msg.GetItemValue(4));
    env->SetKey(msg.GetItemValue(5));
    env->SetRoomServer(roomId,(char *)roomIp,roomPort);

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

void EnterRoomResponse::_Split(INT8U strings[3][128],const INT8U *buf) {
    const char *SPLIT = "%@";
    int idx = 0;

    char *token = strtok((char *)buf,SPLIT);
    
    while(token!=NULL) {
        strcpy((char *)strings[(MIDDLE+idx)%3],token);
        idx++;
        
        token = strtok(NULL,SPLIT);
    }
}

int EnterRoomResponse::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    memset(name,0,sizeof(name));
    memset(image,0,sizeof(image));
            
    roomPath = msg.GetItemValue(0);
    roomId   = msg.GetItemValue(1);
    tableId  = msg.GetItemValue(2);
    seat     = msg.GetItemValue(3);
    SeatInfo::getInstance()->Set(roomPath,roomId,tableId,seat);
    
    baseScore= msg.GetItemValue(4);
    
    playerNum= msg._body->_items[5]->_bufLen;

    int nameLen  = 0;
    int imageLen = 0;
    for(int i=0;i<playerNum;i++) {
        int dir = (MIDDLE+i)%3;
        
        status[dir] = (PlayerStatus_t)msg._body->_items[5]->_buf[i];
        score[dir]  = _ntohl( *(INT32U *)(msg._body->_items[6]->_buf + 4*i) );
    }

    INT8U names[512] = {0};
    msg.GetString(7,names);
    _Split(name,names);
    
    INT8U images[512] = {0};
    msg.GetString(8,images);
    _Split(image,images);

    return 0;
}

int EnterRoomNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);

    memset(name,0,sizeof(name));
    memset(image,0,sizeof(image));

    seat      = _GetPlayer(msg.GetItemValue(0));
    baseScore = msg.GetItemValue(1);
    
    score     = msg.GetItemValue(3);

    msg.GetString(4, name);
    msg.GetString(5, image);

    return 0;
}

int RequestReconnect::Set() {
    SetRequestCode(REQ_GAME_SEND_RECONNECT);

    Key_t key = EnvVariable::getInstance()->GetKey();
    INT32U keyid = _htonl((INT32U)key);

    _add_item( new Item(130,4,(INT8U *)&keyid) );
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
    msg.GetString(0,(INT8U *)_info);
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
    msg.GetString(1,(INT8U *)_content);
        
    return 0;
}

/*************************************************
    Heart beat
*************************************************/
#include "./../network/CSockets.h"
#include <thread>

KwxHeart::KwxHeart(int second) {
    EnvVariable *env = EnvVariable::getInstance();

    _socket = new ClientSocket;
    _socket->Start(env->_roomServer.ipaddr,env->_roomServer.port);

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
#ifndef NO_HEART_BEAT
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

