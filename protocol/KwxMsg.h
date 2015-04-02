
#ifndef __KWX_MSG__
#define __KWX_MSG__


#include "./../RaceType.h"

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

#include "KwxRequestConsts.h"
#include "KwxRequestTypes.h"

#define UP_STREAM     0
#define DOWN_STREAM   1

class Header;
class MsgBody;
class Item;

class KwxMsg {
public:
    KwxMsg(int dir);
    ~KwxMsg();

    const int _dir;    
    Header  *_header;
    MsgBody *_body;

    static Logger       *_logger;
};

class KwxDsMsg : public KwxMsg, public DsMsgIntf {
public:
    KwxDsMsg();

    virtual int Deserialize(const INT8U *inMsg);
    
    RequestId_t GetRequestCode();
    int         GetLevel();
    INT32U      GetItemValue(int idx);
    
    int Construct(ActionResponse_t &waitInfo);
    int Construct(GameStartResponse_t &startInfo);
    int Construct(GameStartNotif_t &startInfo);
    int Construct(HandoutResponse_t &handoutResponse);
    int Construct(HandoutNotif_t &handoutInfo);
    int Construct(ActionNotif_t &action);
    int Construct(DistCardInfo_t &dist);
    
private:
    int KwxDsMsg::_load(Card_t *cards,INT8U &num,const Item *item);
    int KwxDsMsg::_load(_MsgTingInfo_t &info,const INT8U *inMsg);
    int KwxDsMsg::_load(_MingInfo_t &info,const Item *item);
};

class KwxUsMsg : public KwxMsg, public UsMsgIntf {
public:
    virtual int Serialize(INT8U *outMsg);
    
protected:
    KwxUsMsg();
    
	void _set_size(INT8U *buf,INT16U len);
	int  _add_item(Item *item);

    int SetRequestCode(RequestId_t code);
    int SetSeatInfo();
    
/* this is for test use */
public:
    friend class TestFullUpMsg;
};

class RequestSendAction : public KwxUsMsg {
public:
    int Set(ActionId_t code,Card_t card);
};

class RequestGameStart : public KwxUsMsg {
public:
    int Set();
};

class RequestHandout : public KwxUsMsg {
public:
    int Set(Card_t card);
};

#define KWX_INVALID_PCHC          -1
#define KWX_UNSUPPORTED_PARAM     -2
#define KWX_DISMATCH_LENGTH       -3

#endif
