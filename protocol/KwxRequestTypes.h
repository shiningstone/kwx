
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

typedef struct _ActionResponse {
    INT8U         seat;
    INT8U         waitSeat;
}ActionResponse_t;

typedef struct _GameStartResponse_t {
    INT32U        score;
}GameStartResponse_t;

typedef struct _GameStartNotif_t {
    INT8U         seat;
    INT32U        score;
}GameStartNotif_t;

/*there is another TingInfo_t, maybe could combine them*/
typedef struct __MsgTingInfo_t {
    Card_t        card;        /* 可以胡的花色 */
    int           remain;      /* 剩余数目     */
    int           fan;         /* 番数         */
}_MsgTingInfo_t;

typedef struct _HandoutResponse_t {
    Status_t         status;
    int              tingNum;
    _MsgTingInfo_t   tingInfo[9];
}HandoutResponse_t;

typedef struct _HandoutNotif_t {
    INT8U            seat;
    Card_t           handout;
    int              tingNum;
    _MsgTingInfo_t   tingInfo[9];
}HandoutNotif_t;

#endif
