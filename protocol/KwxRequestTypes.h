
#ifndef __KXW_REQUEST_TYPES__
#define __KXW_REQUEST_TYPES__

#include "./../utils/UtilBasic.h"

typedef INT32U RoomPath_t;
typedef INT32U RoomId_t;
typedef INT32U TableId_t;
typedef INT8U  SeatId_t;

typedef INT8U  CardType_t;/*this type could reference to other modules*/

typedef struct _ActionResponse {
    INT8U         seat;
    INT8U         waitSeat;
}ActionResponse_t;

typedef struct _GameStartResponse_t {
    INT32U        score;
}GameStartResponse_t;

#endif
