
#ifndef __KXW_REQUEST_TYPES__
#define __KXW_REQUEST_TYPES__

#include "./../utils/UtilBasic.h"

typedef INT32U RoomPath_t;
typedef INT32U RoomId_t;
typedef INT32U TableId_t;
typedef INT8U  SeatId_t;

typedef INT8U  CardType_t;/*this type could reference to other modules*/

typedef struct _distributeResponse {
    INT8U         seat;
    CardType_t    cardKind;
    INT8U         counter;
    INT32U        reminder;
}DistributeResponse_t;

typedef struct _othersAction {
    INT8U         seat;
    ActionId_t    action;
    CardType_t    cardKind;
    INT8U         seat2;
    INT8U         counter;
    INT32U        reminder;
    INT8U         wait;
}OthersAction_t;

typedef struct _OthersShowCard {
    INT8U         seat;
    CardType_t    cardKind;
}OthersShowCard_t;

typedef struct _ActionResponse {
    INT8U         seat;
    INT8U         waitSeat;
}ActionResponse_t;

#endif
