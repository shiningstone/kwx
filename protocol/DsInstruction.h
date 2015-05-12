
#ifndef _DS_INSTRUCTION_
#define _DS_INSTRUCTION_

#include "./../utils/LogManager.h"

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
    virtual int Dispatch();

    RequestId_t      request;
protected:
    DsInstruction();
    static SeatInfo *_seatInfo;

    PlayerDir_t      _GetPlayer(INT8U seat);

#ifndef __UNIT_TEST__
    NetRoundManager   *_roundManager;
#endif
    static Logger   *_logger;

#ifdef WIN32
public:
    char            *Desc(RequestId_t id)const;
#endif
};

class GameStartResponse : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);

    INT32U           score;
};

class GameStartNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    
    INT8U            seat;
    INT32U           score;
};

class ShowCardResponse : public DsInstruction {
public:
    ~ShowCardResponse();
    
    virtual int Construct(const DsMsg &msg);
    
    Status_t        status;
    TingInfo_t      ting;
};

class ShowCardNotif : public DsInstruction {
public:
    ~ShowCardNotif();
    
    virtual int Construct(const DsMsg &msg);
    
    INT8U            seat;
    Card_t           kind;
    TingInfo_t       ting;
};

class ActionResponse : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    
    INT8U            seat;
    INT8U            waitSeat;
};

class ActionNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    
    INT8U            seat;
    INT8U            whoGive;
    INT8U            next;
    ActionMask_t     actions;
    INT8U            cardNum;
    CardNode_t       card[18];
};

class DecisionNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    
    INT8U            seat;
    INT8U            whoGive;
    INT8U            next;
    ActionMask_t     actions;
    INT8U            cardNum;
    CardNode_t       card[18];
};

class DistCardInfo : public DsInstruction {
public:
    ~DistCardInfo();
    
    virtual int Construct(const DsMsg &msg);
    
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
    
    INT8U            seat;
    INT8U            timer;
    INT8U            remain;
    Card_t           cards[14];
    Reminds_t        remind;
};

class FirstDistNonZhuang : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    
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
    
    INT8U            seat;
    INT8U            timer;
    Card_t           kind;
    INT8U            whoGive;
    Reminds_t        remind;
    INT8U            wait;
};

class DistCardNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    
    INT8U            seat;
    INT8U            remain;
    INT8U            timer;
    Card_t           kind;
};

class ScoreNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    
    INT8U            seat[3];
    INT32U           val[3];
};

class TingInfoResponse : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
    
    TingInfo_t       info;
};

typedef enum {
    HU_PAI = 1,
    ZI_MO,
    DIAN_PAO,
}HuKind_t;

typedef struct {
    INT8U   winType;
    INT32U  fan;
    INT32U  score;
    INT32U  bet;
    INT8U   name[128];
    INT8U   image[128];
    int     cardNum;
    Card_t  card[18];
}HuInfo_t;

class HuInfoNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);

    INT8U            seat;
    INT8U            zhuang;
    HuInfo_t         hu[3];
};

class EndInfoNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);

    INT8U            seat;
    INT8U            cardNum[3];
    Card_t           card[3][18];
};

class EnterRoomResponse : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);
private:    
    RoomPath_t       roomPath;
    RoomId_t         roomId;
    TableId_t        tableId;
    SeatId_t         seat;
    
    int loadFromUtf16(INT8U *buf,const INT8U *input);
public:    
    INT32U           baseScore;

    int              playerNum;
    bool             status[3];
    INT32U           score[3];
    INT8U            name[3][128];
    INT8U            image[3][128];    
};

class EnterRoomNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);

    int              seat;
    INT32U           baseScore;
    bool             status;
    INT32U           score;
    INT8U            name[128];
    INT8U            image[128];    
};

class CounterNotif : public DsInstruction {
public:
    virtual int Construct(const DsMsg &msg);

    int              count;
    INT32U           seat;
};

#endif

