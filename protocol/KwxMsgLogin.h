
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

    UserType_t  _userType;
    HostString  _account;
    HostString  _mac;
    HostString  _imsi;
    HostString  _resolution;
    HostString  _product;
    HostString  _osVer;
    HostString  _session;
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

class RequestEnterRoom : public UsMsg {
public:
    int Set(int id=0);
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

class LoginConflictNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    
    HostString    _info;
};
#endif

