
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

typedef HostString Account_t;
typedef HostString MacAddr_t;
typedef HostString Imsi_t;
typedef HostString Resolution_t;
typedef HostString Product_t;
typedef HostString OsVersion_t;
typedef HostString SessionKey_t;

class RequestLogin : public UsMsg {
public:
    int Set();

    UserType_t    _userType;
    Account_t     _account;
    MacAddr_t     _mac;
    Imsi_t        _imsi;
    Resolution_t  _resolution;
    Product_t     _product;
    OsVersion_t   _osVer;
    SessionKey_t  _session;
};

class LoginResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
};

#endif

