
#ifndef __KXW_REQUEST_TYPES__
#define __KXW_REQUEST_TYPES__

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"

typedef INT32U RoomPath_t;
typedef INT32U RoomId_t;
typedef INT32U TableId_t;
typedef INT8U  SeatId_t;

/*this type could reference to other modules*/
typedef INT8U  CardType_t;

typedef enum {
    SUCCESS,
    FAIL,
}Status_t;

typedef struct _ActionResponse_t {
    INT8U         seat;
    INT8U         waitSeat;
}ActionResponse_t;

typedef struct _ActionNotif_t {
    INT8U         seat;
    bool          isFromServer;
    INT8U         next;
    ActionId_t    action;
    INT8U         cardNum;
    Card_t        card[18];
}ActionNotif_t;

typedef struct _GameStartResponse_t {
    INT32U        score;
}GameStartResponse_t;

typedef struct _GameStartNotif_t {
    INT8U         seat;
    INT32U        score;
}GameStartNotif_t;

/*there is another TingInfo_t, maybe could combine them*/
typedef struct _TingItem_t {
    Card_t        kind;        /* 可以胡的花色 */
    int           remain;      /* 剩余张数     */
    int           fan;         /* 番数         */
}TingItem_t;

typedef struct _MsgTingInfo_t {
    int              cardNum;      /* 可以胡的花色数目 */
    TingItem_t       *cards;
}MsgTingInfo_t;

typedef struct _HandoutResponse_t {
    Status_t         status;
    MsgTingInfo_t   ting;
}HandoutResponse_t;

typedef struct _HandoutNotif_t {
    INT8U            seat;
    Card_t           kind;
    MsgTingInfo_t   ting;
}HandoutNotif_t;

typedef struct __MingChoice_t {
    Card_t           kind;       /*可以出的花色*/
    MsgTingInfo_t   ting;
}_MingChoice_t;

typedef struct __MingInfo_t {
    INT8U            choiceNum;     /*可以出的花色数目*/
    _MingChoice_t    *handouts;    
}_MingInfo_t;

typedef struct __Reminds_t {
    ActionId_t       actions;
    INT8U            gangKindNum;
    Card_t           gangCard[4];
    INT8U            kouKindNum;
    Card_t           kouCard[4];
    _MingInfo_t      ming;
}_Reminds_t;

typedef struct _DistCardInfo_t {
    INT8U            seat;
    INT8U            timer;
    INT8U            remain;
    Card_t           kind;
    _Reminds_t       remind;
}DistCardInfo_t;

typedef struct _FirstDistZhuang_t {
    INT8U            seat;
    INT8U            timer;
    INT8U            remain;
    Card_t           cards[14];
    _Reminds_t       remind;
}FirstDistZhuang_t;

typedef struct _FirstDistNonZhuang_t {
    INT8U            seat;
    INT8U            remain;
    Card_t           cards[13];
    INT8U            zhuang;
    INT8U            timer;
}FirstDistNonZhuang_t;

typedef struct _RemindInfo_t {
    INT8U            seat;
    INT8U            timer;
    _Reminds_t       remind;
    INT8U            wait;
}RemindInfo_t;

typedef struct _DistCardNotif_t {
    INT8U            seat;
    INT8U            remain;
    INT8U            timer;
    Card_t           kind;
}DistCardNotif_t;

typedef struct _ScoreNotif_t {
    INT8U            seat[3];
    INT32U           val[3];
}ScoreNotif_t;

typedef struct _DecisionNotif_t {
    INT8U            seat;
    INT8U            whoGive;
    INT8U            next;
    ActionId_t       actions;
    Card_t           card;
}DecisionNotif_t;

#endif
