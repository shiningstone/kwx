
#include <string.h>

#include "./../utils/UtilPlatform.h"
#include "./../network/KwxEnv.h"
#include "./../network/KwxMessenger.h"
#include "./../hall/VersionManager.h"

#include "MsgFormats.h"
#include "CommonMsg.h"
#include "DsInstruction.h"
#include "DsMsgParser.h"

#include "KwxMsgLogin.h"

#include "./../utils/DebugCtrl.h"

#define CHECK_STATUS(msg) do { \
    int status = msg.GetItemValue(0); \
    if(status!=0) { \
        KwxMessenger::getInstance(MSG_LOGIN)->_response = (FailureCode_t)status; \
        return status; \
    } else { \
        KwxMessenger::getInstance(MSG_LOGIN)->_response = REQUEST_ACCEPTED; \
    } \
} while(0)

/* Example from 9100 without Sim card
    4b 57 58 10 01 02 03 04 05 06 07 08 09 0a 0b 00 01 00 8e 
    08
    3c 00
    83 00 02 fe ff
    84 00 24 
    	fe ff 00 39 00 38 00 3a 00 30 00 43 00 3a 00 39 
    	00 32 00 3a 00 43 00 39 00 3a 00 41 00 36 00 3a 
    	00 31 00 42
    85 00 02 fe ff 
    86 00 10 fe ff 00 34 00 38 00 30 00 2a 00 38 00 30 00 30 
    87 00 12 fe ff 00 47 00 54 00 2d 00 49 00 39 00 31 00 30 00 30 
    88 00 0c fe ff 00 32 00 2e 00 33 00 2e 00 35
    89 00 02 fe ff
*/
int RequestLogin::Set(UserType_t type) {
    memset(_account,0,MAX_UTF8_BUF);
    memset(_pwd,0,MAX_UTF8_BUF);

    _userType = type;
    sprintf((char *)_account,"0");
    sprintf((char *)_pwd,"0");

    SetRequestCode(REQ_LOGIN);

    _add_item( new Item(60,_userType) );

    DeviceInfo_t &device = EnvVariable::getInstance()->_device;

    _add_utf16_string(131,(const INT8U *)_account);
    _add_utf16_string(132,device.mac);
    _add_utf16_string(133,device.imsi);
    _add_utf16_string(134,device.resolution);
    _add_utf16_string(135,device.protoType);
    _add_utf16_string(136,device.osVer);
    _add_utf16_string(137,(const INT8U *)_pwd);

    return 0;
}

int LoginResponse::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);

    CHECK_STATUS(msg);
    
    _userType          = (UserType_t)msg.GetItemValue(1);
    _userActivated     = (msg.GetItemValue(2)==1);

    EnvVariable *env = EnvVariable::getInstance();
    if(msg._body->_items[3]->_bufLen/4 > 1) {/* reconnect required */
        INT32U roomPath = _ntohl(*(INT32U *)(msg._body->_items[3]->_buf+4));
        INT32U roomId   = _ntohl(*(INT32U *)(msg._body->_items[3]->_buf+8));
        INT32U tableId  = _ntohl(*(INT32U *)(msg._body->_items[3]->_buf+12));
        INT32U seatId   = _ntohl(*(INT32U *)(msg._body->_items[3]->_buf+16));

        SeatInfo::getInstance()->Set(roomPath,roomId,tableId,seatId);
        env->SetReconnect(true);
    }

    INT32U roomId      = msg.GetItemValue(6);
    INT8U  roomIp[32] = {0};
    msg.GetString(7,roomIp);
    INT32U roomPort    = msg.GetItemValue(8);
    INT32U voicePort   = msg.GetItemValue(9);
    
    env->SetUserId(msg.GetItemValue(4));
    env->SetKey(msg.GetItemValue(5));
    env->SetRoomServer(roomId,(char *)roomIp,roomPort,voicePort);

    return 0;
}

int RequestResourceUpdate::Set() {
    SetRequestCode(REQ_RES_UPDATE);
    return 0;
}

int ResourceUpdateResponse::Construct(const DsMsg & msg) {
    CHECK_STATUS(msg);
    
    maxResId = msg.GetItemValue(1);
    msg.GetString(2,url);
    return 0;
}

int RequestVersionUpdate::Set() {
    SetRequestCode(REQ_VER_UPDATE);
    return 0;
}

int VersionUpdateResponse::Construct(const DsMsg & msg) {
    CHECK_STATUS(msg);

    VerInfo_t &ver = VersionManager::getInstance()->_newVer;
    
    ver.code = msg.GetItemValue(1);
    msg.GetString(2,ver.name);
    msg.GetString(3,ver.content);
    msg.GetString(4,ver.size);
    msg.GetString(5,ver.url);
    
    return 0;
}

int RequestBasicInfo::Set() {
    SetRequestCode(RRQ_BASIC_INFO);
    return 0;
}

int BasicInfoResponse::Construct(const DsMsg &msg) {
    Personal_Information &info = EnvVariable::getInstance()->personalDetailed;

    info.id          = msg.GetItemValue(0);
    msg.GetString(1,info.account);
    info.accountType = (Account_Type)msg.GetItemValue(2);
    msg.GetString(3,info.Nickname);
    info.PlayerSex   = (Sex_t)msg.GetItemValue(4);
    //info.Winrate
    info.maxTime     = msg.GetItemValue(6);
    msg.GetString(7,info.PhotoName);
    //info.PlayerLv
    info.GoldNum     = msg.GetItemValue(9);
    
    return 0;
}

int RequestDailyLogin::Set(Key_t key) {
    SetRequestCode(REQ_DAILY_LOGIN);

    INT32U keyVal = 0;

    if(key==INVALID) {
        keyVal = _htonl((INT32U)EnvVariable::getInstance()->GetKey());
    } else {
        keyVal = _htonl((INT32U)key);
    }
    
    _add_item( new Item((Item_t)131,4,(INT8U *)&keyVal) );

    return 0;
}

int DailyLoginResponse::Construct(const DsMsg &msg) {
    DataDailyLogin_t *info = &(EnvVariable::getInstance()->_dailyLogin);
    memset(info,0,sizeof(DataDailyLogin_t));
    
    DsInstruction::Construct(msg);

    if(msg._body->_items[0]->_bufLen==4) {
        info->hasReward = false;
    } else {
        info->hasReward = true;
        info->continuousDays = _ntohl(*(INT32U *)(msg._body->_items[0]->_buf+4));
    }
    
    msg.GetString(1, info->name);
    msg.GetString(2, info->image);
    
    info->gold   = msg.GetItemValue(3);
    info->coupon = msg.GetItemValue(4);

    DsMsgParser::_load_friends_info(info->friends,msg,5);
    DsMsgParser::_load_others_info(info->strangers,msg,6);
    DsMsgParser::_load_others_info(info->nearbys,msg,7);

    return 0;
}

int RequestEnterRoom::Set(int id) {
    SetRequestCode(REQ_GAME_SEND_ENTER);

    RoomPath_t roomPath = EnvVariable::getInstance()->GetRoomPath();

    INT32U roomId = _htonl((INT32U)roomPath);
    
    _add_item( new Item((Item_t)131,4,(INT8U *)&roomId) );

    return 0;
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
    _split(name,names);
    
    INT8U images[512] = {0};
    msg.GetString(8,images);
    _split(image,images);

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
    DsInstruction::Construct(msg);
        
    memset(name,0,sizeof(name));
    memset(image,0,sizeof(image));

    int seatId = msg.GetItemValue(0);
    curPlayer  = _GetPlayer(msg.GetItemValue(1));
    zhuang     = _GetPlayer(msg.GetItemValue(2));
    remains    = msg.GetItemValue(3);
    count      = msg.GetItemValue(4);
    
    baseScore  = msg.GetItemValue(5);
    for(int i=0;i<PLAYER_NUM;i++) {
        int dir = (MIDDLE+i)%3;
        
        status[dir] = (PlayerStatus_t)msg._body->_items[6]->_buf[i];
        score[dir]  = _ntohl( *(INT32U *)(msg._body->_items[7]->_buf + 4*i) );
    }

    DsMsgParser::_load(isMing,tingNum,tingCards,msg,9);

    CardNode_t temp[3][18];
    INT8U      tempNum[3];
    DsMsgParser::_load(temp, tempNum, msg, 8);
    for(int i=0;i<PLAYER_NUM;i++) {
        int dir = (MIDDLE+i)%3;
        memcpy(cardsInHand[dir],temp[i],sizeof(CardNode_t)*18);
        cardsNum[dir] = tempNum[i];
    }
    
    DsMsgParser::_load(temp, tempNum, msg, 10);
    for(int i=0;i<PLAYER_NUM;i++) {
        int dir = (MIDDLE+i)%3;
        memcpy(river[dir],temp[i],sizeof(CardNode_t)*18);
        riverNum[dir] = tempNum[i];
    }

    
    INT8U names[512] = {0};
    msg.GetString(11,names);
    _split(name,names);
    
    INT8U images[512] = {0};
    msg.GetString(12,images);
    _split(image,images);

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

