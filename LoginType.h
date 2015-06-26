
#ifndef __LOGIN_TYPE__

#define __LOGIN_TYPE__

#include <string>
#include "utils/UtilBasic.h"
#include "game/GameType.h"

typedef struct _SeatInfo_t {
    INT32U   roomPath;
    INT32U   roomId;
    INT32U   tableId;
    INT32U   seatId;
}SeatInfo_t;

#endif
