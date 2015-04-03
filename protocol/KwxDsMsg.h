
#ifndef __KWX_DS_MSG__
#define __KWX_DS_MSG__

#include "KwxMsg.h"

class RoundManager;
/****************************************************
    DOWNSTREAM : Instruction structure
****************************************************/
class KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg) = 0;
    virtual int Dispatch() = 0;
protected:
#ifndef __UNIT_TEST__
    KwxDsInstruction();
    RoundManager *_roundManager;
#endif
};

class GameStartResponse : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();

    INT32U           score;
};

class GameStartNotif : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT32U           score;
};

class HandoutResponse : public KwxDsInstruction {
public:
    ~HandoutResponse();
    
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    Status_t        status;
    MsgTingInfo_t   ting;
};

class HandoutNotif : public KwxDsInstruction {
public:
    ~HandoutNotif();
    
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
    
    INT8U            seat;
    INT8U            waitSeat;
};

class ActionNotif : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    bool             isFromServer;
    INT8U            next;
    INT8U            actionNum;
    ActionId_t       actions[MAX_AVAIL_ACTIONS];
    INT8U            cardNum;
    Card_t           card[18];
};

class DistCardInfo : public KwxDsInstruction {
public:
    ~DistCardInfo();
    
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            timer;
    INT8U            remain;
    Card_t           kind;
    Reminds_t        remind;
};

class FirstDistZhuang : public KwxDsInstruction {
public:
    ~FirstDistZhuang();
    
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            timer;
    INT8U            remain;
    Card_t           cards[14];
    Reminds_t        remind;
};

class FirstDistNonZhuang : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            remain;
    Card_t           cards[13];
    INT8U            zhuang;
    INT8U            timer;
};

class RemindInfo : public KwxDsInstruction {
public:
    ~RemindInfo();
    
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            timer;
    Reminds_t        remind;
    INT8U            wait;
};

class DistCardNotif : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            remain;
    INT8U            timer;
    Card_t           kind;
};

class ScoreNotif : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat[3];
    INT32U           val[3];
};

class DecisionNotif : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            whoGive;
    INT8U            next;
    INT8U            actionNum;
    ActionId_t       actions[MAX_AVAIL_ACTIONS];
    Card_t           card;
};

class TingInfoResponse : public KwxDsInstruction {
public:
    virtual int Construct(const KwxDsMsg &msg);
    virtual int Dispatch();
    
    MsgTingInfo_t    info;
};

#endif

