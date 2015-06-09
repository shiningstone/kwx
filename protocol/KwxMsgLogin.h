
#ifndef __KWX_MSG_LOGIN__
#define __KWX_MSG_LOGIN__

#include "KwxMsgBasic.h"

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
    int Set();

    UserType_t   _userType;
    HostString   _account;
    DeviceInfo_t _device;
    HostString   _session;
};

class LoginResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    UserType_t  _userType;
    bool        _userActivated;
    bool        _reconnectRequired;
    INT32U      _userId;
    INT32U      _key;
    INT32U      _roomServerId;
    INT8U       _roomServerIp[32];
    INT32U      _roomServerPort;
};

class RequestEnterRoom : public UsMsg {
public:
    int Set(int id=0);
};

class EnterRoomResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    static void  _LoadStrings(INT8U strings[][128],const INT8U *buf,int bufLen);
private:

    RoomPath_t       roomPath;
    RoomId_t         roomId;
    TableId_t        tableId;
    SeatId_t         seat;
    
public:    
    INT32U           baseScore;

    int              playerNum;
    bool             status[3];
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

    INT32U  _key;
};

class ReconnectResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    
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

