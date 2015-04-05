
#ifndef _DS_INSTRUCTION_
#define _DS_INSTRUCTION_

#include "./../utils/LogManager.h"
#include "./../game/DiStructs.h"

#include "RequestStructs.h"
#include "CommonMsg.h"

class SeatInfo;
class NetRoundManager;
/****************************************************
    DOWNSTREAM : Instruction structure
****************************************************/
class DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch() = 0;
protected:
    DsInstruction();
    static SeatInfo *_seatInfo;
    RequestId_t      request;

    PlayerDir_t      _GetPlayer(INT8U seat);
    int _sendToManager(void *info);
    
#ifndef __UNIT_TEST__
    NetRoundManager   *_roundManager;
#endif
    static Logger   *_logger;
};

class GameStartResponse : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();

    INT32U           score;
};

class GameStartNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT32U           score;
};

class HandoutResponse : public DsInstruction {
public:
    ~HandoutResponse();
    
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    Status_t        status;
    MsgTingInfo_t   ting;
};

class HandoutNotif : public DsInstruction {
public:
    ~HandoutNotif();
    
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    Card_t           kind;
    MsgTingInfo_t    ting;
};

class ActionResponse : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            waitSeat;
};

class ActionNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    bool             isFromServer;
    INT8U            next;
    INT8U            actionNum;
    ActionId_t       actions[MAX_AVAIL_ACTIONS];
    INT8U            cardNum;
    Card_t           card[18];
};

class DistCardInfo : public DsInstruction {
public:
    ~DistCardInfo();
    
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            timer;
    INT8U            remain;
    Card_t           kind;
    Reminds_t        remind;
};

class FirstDistZhuang : public DsInstruction {
public:
    ~FirstDistZhuang();
    
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            timer;
    INT8U            remain;
    Card_t           cards[14];
    Reminds_t        remind;
};

class FirstDistNonZhuang : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            remain;
    Card_t           cards[13];
    INT8U            zhuang;
    INT8U            timer;
};

class RemindInfo : public DsInstruction {
public:
    ~RemindInfo();
    
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            timer;
    Reminds_t        remind;
    INT8U            wait;
};

class DistCardNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            remain;
    INT8U            timer;
    Card_t           kind;
};

class ScoreNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat[3];
    INT32U           val[3];
};

class DecisionNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    INT8U            seat;
    INT8U            whoGive;
    INT8U            next;
    INT8U            actionNum;
    ActionId_t       actions[MAX_AVAIL_ACTIONS];
    Card_t           card;
};

class TingInfoResponse : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    virtual int Dispatch();
    
    MsgTingInfo_t    info;
};

#endif

