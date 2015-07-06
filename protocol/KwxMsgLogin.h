
#ifndef __KWX_MSG_LOGIN__
#define __KWX_MSG_LOGIN__

#include "KwxMsgBasic.h"
#include "./../network/KwxEnv.h"

class UsMsg;
class DsMsg;
class DsInstruction;

typedef enum {
    VISITOR,
    QQer,
}UserType_t;

typedef Utf8       HostString[MAX_UTF8_BUF];
typedef Utf16      NetString[MAX_CHARS];

class RequestLogin : public UsMsg {
public:
    int Set(UserType_t type=VISITOR);

    UserType_t   _userType;
    HostString   _account;
    HostString   _pwd;
};

class LoginResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    UserType_t  _userType;
    bool        _userActivated;
};

class RequestResourceUpdate : public UsMsg {
public:
    int Set();
};

class ResourceUpdateResponse : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);

    INT32U     maxResId;
    INT8U      url[128];
};

class RequestVersionUpdate : public UsMsg {
public:
    int Set();
};

class VersionUpdateResponse : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
};

class RequestBasicInfo : public UsMsg {
public:
    int Set();
};

class BasicInfoResponse : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
};

class RequestDailyLogin : public UsMsg {
public:
    int Set(Key_t key=INVALID);
};

class DailyLoginResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
};

class RequestEnterRoom : public UsMsg {
public:
    int Set(int id=0);
};

typedef enum {
    ABSENT          = 0,
    WAIT_FOR_START  = 1,
    WAIT_FOR_PIAO   = 2,
    READY           = 5,
}PlayerStatus_t;

class EnterRoomResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
private:

    RoomPath_t       roomPath;
    RoomId_t         roomId;
    TableId_t        tableId;
    SeatId_t         seat;
    
public:    
    INT32U           baseScore;

    int              playerNum;
    PlayerStatus_t   status[3];
    INT32U           score[3];
    INT8U            name[3][128];
    INT8U            image[3][128];    
};

class EnterRoomNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    int              seat;
    INT32U           baseScore;
    bool             status;
    INT32U           score;
    INT8U            name[128];
    INT8U            image[128];    
};

class RequestReconnect : public UsMsg {
public:
    int Set();
};

class ReconnectResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    INT8U            seatId;
    PlayerDir_t      curPlayer;
    PlayerDir_t      zhuang;
    INT32U           remains;
    INT32U           count;
    INT32U           baseScore;
    PlayerStatus_t   status[3];
    INT32U           score[3];
    INT8U            cardsNum[3];
    CardNode_t       cardsInHand[3][18];
    bool             isMing[3];
    INT8U            tingNum[3];
    Card_t           tingCards[3][18];
    INT8U            riverNum[3];
    CardNode_t       river[3][18];
    INT8U            name[3][128];
    INT8U            image[3][128];    
};

class ReconnectNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    PlayerDir_t _dir;
};

class RequestLeave : public UsMsg {
public:
    int Set();
};

class LeaveResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    INT32U     _score;
};

typedef enum {
    LEGAL,     /*游戏结束或未开始离开，踢掉该玩家，空出座位*/
    ILLEGAL,   /*游戏中离开，玩家只提示玩家离开，游戏继续*/
}LeaveStatus_t;

class LeaveNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    PlayerDir_t   _dir;
    LeaveStatus_t _status;
};

class LoginConflictNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    
    HostString    _info;
};

class RequestXiaPiao : public UsMsg {
public:
    int Set(INT32U score);
};

class XiaPiaoResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    
    INT32U _score;
};

class XiaPiaoNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    PlayerDir_t _dir;    
    INT32U      _score;
};

typedef enum {
    TEXT         = 1,
    EXPRESSION,
}TalkType_t;

class RequestTalk : public UsMsg {
public:
    int Set(const INT8U *content, TalkType_t type, PlayerDir_t dir);

    PlayerDir_t   _sendTo;
    TalkType_t    _type;
    HostString    _content;
};

class TalkResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    
    INT8U        _status;
};

class TalkNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    
    TalkType_t    _type;
    HostString    _content;
};

class CSocket;
class KwxHeart {
public:
    static KwxHeart *getInstance(int second=5);
    static void      destroyInstance();

    void SetRate(int second);
private:
    KwxHeart(int second);
    ~KwxHeart();

    void    _Beats();

    CSocket *_socket;
    int      _rate;
    bool     _running;

    static KwxHeart *_instance;
};

class RequestHeartBeat : public UsMsg {
public:
    int Set();
};

#endif

