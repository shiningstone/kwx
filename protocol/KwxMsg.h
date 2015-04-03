
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

/****************************************************
    DOWNSTREAM
****************************************************/

class KwxDsMsg : public KwxMsg, public DsMsgIntf {
public:
	static KwxDsMsg *getInstance();
	static void          destroyInstance();

    virtual int Deserialize(const INT8U *inMsg);
    
    RequestId_t GetRequestCode();
    int         GetLevel();

    /* these functions are only for test purpose */
    int Construct(GameStartResponse_t &startInfo);
    int Construct(GameStartNotif_t &startInfo);
    int Construct(HandoutResponse_t &handoutResponse);
    int Construct(HandoutNotif_t &handoutInfo);
    int Construct(ActionResponse_t &waitInfo);
    int Construct(ActionNotif_t &action);
    int Construct(DistCardInfo_t &dist);
    int Construct(FirstDistZhuang_t &dist);
    int Construct(FirstDistNonZhuang_t &dist);
    int Construct(DistCardNotif_t &dist);
    int Construct(ScoreNotif_t &score);
    int Construct(RemindInfo_t &remind);
    int Construct(DecisionNotif_t &decision);
    int Construct(MsgTingInfo_t &info);

    /***************************************************************************
    NOTE :
        there are some dynamically allocated memory in several kinds of struct
    ***************************************************************************/
    static void Release(DistCardInfo_t &info);
    static void Release(HandoutNotif_t &info);
    static void Release(HandoutResponse_t &info);
    static void Release(RemindInfo_t &info);
    static void Release(FirstDistZhuang_t &info);
    
private:
	static KwxDsMsg *_instance;
    KwxDsMsg();

    INT32U  _GetItemValue(int idx);

    int _load(Card_t *cards,INT8U &num,int itemIdx);
    int _load(ActionId_t *actions,INT8U &num,int itemIdx);
    int _load(MsgTingInfo_t &info,const INT8U *inMsg);
    int _load(_MingInfo_t &info,const Item *item);
    int _load(_Reminds_t &remind,int itemIdx);
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
