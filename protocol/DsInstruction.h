
#ifndef _DS_INSTRUCTION_
#define _DS_INSTRUCTION_

#include "./../utils/LogManager.h"

#include "RequestStructs.h"
#include "CommonMsg.h"
#include "SeatInfo.h"

class NetRoundManager;
/****************************************************
    DOWNSTREAM : Instruction structure
****************************************************/
class DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    virtual int  Dispatch();
    virtual void Show() const;

    RequestId_t      request;
    FailureCode_t    failure;
    SeatInfo_t       reconnectInfo;
protected:
    DsInstruction();
    static SeatInfo *_seatInfo;

    bool             _IsRejection(const DsMsg &msg);
    PlayerDir_t      _GetPlayer(INT8U seat);

    static int       _recvCnt;
#ifndef __UNIT_TEST__
    NetRoundManager   *_roundManager;
#endif
    static Logger   *_logger;
};

class GameStartResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    INT32U           score;
};

class GameStartNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    
    INT8U            seat;
    INT32U           score;
};

class ShowCardResponse : public DsInstruction {
public:
    ~ShowCardResponse();
    
    virtual int  Construct(const DsMsg &msg);
    
    Status_t        status;
    TingInfo_t      ting;
};

class ShowCardNotif : public DsInstruction {
public:
    ~ShowCardNotif();
    
    virtual int  Construct(const DsMsg &msg);
    virtual void Show() const;
    
    INT8U            seat;
    Card_t           kind;
    TingInfo_t       ting;
};

class ActionResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    
    INT8U            seat;
    INT8U            waitSeat;
};

class ActionNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    virtual void Show() const;
    
    INT8U            seat;
    INT8U            whoGive;
    INT8U            next;
    ActionMask_t     actions;
    INT8U            cardNum;
    CardNode_t       card[18];
    INT8U            huCardsNum[3];
    CardNode_t       huCards[3][18];
};

typedef class ActionNotif DecisionNotif;

class DistCardInfo : public DsInstruction {
public:
    ~DistCardInfo();
    
    virtual int  Construct(const DsMsg &msg);
    
    INT8U            seat;
    INT8U            timer;
    INT8U            remain;
    Card_t           kind;
    Reminds_t        remind;
};

class FirstDistZhuang : public DsInstruction {
public:
    ~FirstDistZhuang();
    
    virtual int  Construct(const DsMsg &msg);
    
    INT8U            seat;
    INT8U            timer;
    INT8U            remain;
    Card_t           cards[14];
    Reminds_t        remind;
};

class FirstDistNonZhuang : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    
    INT8U            seat;
    INT8U            remain;
    Card_t           cards[13];
    INT8U            zhuang;
    INT8U            timer;
};

class RemindInfo : public DsInstruction {
public:
    ~RemindInfo();
    
    virtual int  Construct(const DsMsg &msg);
    
    INT8U            seat;
    INT8U            timer;
    Card_t           kind;
    INT8U            whoGive;
    Reminds_t        remind;
    INT8U            wait;
};

class DistCardNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    virtual void Show() const;
    
    INT8U            seat;
    INT8U            remain;
    INT8U            timer;
    Card_t           kind;
};

class ScoreNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    
    INT8U            seat[3];
    INT32S           val[3];
};

class TingInfoResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);
    
    TingInfo_t       info;
};

typedef enum {
    HU_PAI = 1,
    ZI_MO,
    DIAN_PAO,
}HuKind_t;

typedef struct {
    HuKind_t   status;
    INT32U     fan;
    INT32U     score;
    int        cardNum;
    CardNode_t card[18];
}HuInfo_t;

class HuInfoNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    INT8U            zhuang;
    HuInfo_t         hu[3];
};

class TuoGuanResponse : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    INT8U            status;
};

class TuoGuanNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    PlayerDir_t      dir;
    bool             status;
};

class CounterNotif : public DsInstruction {
public:
    virtual int  Construct(const DsMsg &msg);

    int              count;
    INT32U           seat;
};

#endif

