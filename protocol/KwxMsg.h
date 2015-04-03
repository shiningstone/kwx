
#ifndef _KWX_MSG_
#define _KWX_MSG_

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

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

    static Logger  *_logger;
};

/****************************************************
    DOWNSTREAM : Basic Message structure
****************************************************/
class KwxDsInstruction;
class KwxDsMsg : public KwxMsg, public DsMsgIntf {
public:
	static KwxDsMsg *getInstance();
	static void      destroyInstance();

    int Dispatch(const INT8U *inMsg);
    virtual int Deserialize(const INT8U *inMsg);
    KwxDsInstruction *_GenerateInstruction();    

    RequestId_t GetRequestCode();
    int         GetLevel();
    INT32U      GetItemValue(int idx) const;

    int _load(Card_t *cards,INT8U &num,int itemIdx) const;
    int _load(ActionId_t *actions,INT8U &num,int itemIdx) const;
    int _load(MsgTingInfo_t &info,const INT8U *inMsg) const;
    int _load(_MingInfo_t &info,const Item *item) const;
    int _load(_Reminds_t &remind,int itemIdx) const;

private:
	static KwxDsMsg *_instance;
    KwxDsMsg();
};

/****************************************************
    UPSTREAM
****************************************************/
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

class RequestTingInfo : public KwxUsMsg {
public:
    int Set();
};

#define KWX_INVALID_PCHC          -1
#define KWX_UNSUPPORTED_PARAM     -2
#define KWX_DISMATCH_LENGTH       -3

#endif
