
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
    DOWNSTREAM : Basic Message structure
****************************************************/
class KwxDsMsg : public KwxMsg, public DsMsgIntf {
public:
	static KwxDsMsg *getInstance();
	static void      destroyInstance();

    virtual int Deserialize(const INT8U *inMsg);
    
    RequestId_t GetRequestCode();
    int         GetLevel();

    INT32U      GetItemValue(int idx) const;

    int _load(Card_t *cards,INT8U &num,int itemIdx) const;
    int _load(ActionId_t *actions,INT8U &num,int itemIdx) const;
    int _load(MsgTingInfo_t &info,const INT8U *inMsg) const;
    int _load(_MingInfo_t &info,const Item *item) const;
    int _load(_Reminds_t &remind,int itemIdx) const;

    /* these functions are only for test purpose */
    int Construct(FirstDistZhuang_t &dist);
    int Construct(FirstDistNonZhuang_t &dist);
    int Construct(DistCardNotif_t &dist);
    int Construct(ScoreNotif_t &score);
    int Construct(RemindInfo_t &remind);
    int Construct(DecisionNotif_t &decision);
    int Construct(MsgTingInfo_t &info);

#if 0
    /***************************************************************************
    NOTE :
        there are some dynamically allocated memory in several kinds of struct
    ***************************************************************************/
    static void Release(DistCardInfo_t &info);
    static void Release(HandoutNotif_t &info);
    static void Release(HandoutResponse_t &info);
    static void Release(RemindInfo_t &info);
    static void Release(FirstDistZhuang_t &info);
#endif    
private:
	static KwxDsMsg *_instance;
    KwxDsMsg();
};

/****************************************************
    DOWNSTREAM : Instruction structure
****************************************************/
class KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg) = 0;
    virtual int Dispatch() = 0;
};

class GameStartResponse : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();

    INT32U        score;
};

class GameStartNotif : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U         seat;
    INT32U        score;
};

class HandoutResponse : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    Status_t        status;
    MsgTingInfo_t   ting;
};

class HandoutNotif : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    Card_t           kind;
    MsgTingInfo_t    ting;
};

class ActionResponse : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U         seat;
    INT8U         waitSeat;
};

class ActionNotif : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U         seat;
    bool          isFromServer;
    INT8U         next;
    INT8U         actionNum;
    ActionId_t    actions[MAX_AVAIL_ACTIONS];
    INT8U         cardNum;
    Card_t        card[18];
};

class DistCardInfo : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            timer;
    INT8U            remain;
    Card_t           kind;
    _Reminds_t       remind;
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
