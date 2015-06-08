
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
};

#endif

