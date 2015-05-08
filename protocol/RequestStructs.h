
#ifndef _REQUEST_STRUCTS_
#define _REQUEST_STRUCTS_

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"

#include "KwxMsgBasic.h"

typedef struct _Reminds_t {
    ActionMask_t  actions;
    INT8U         gangKindNum;
    Card_t        gangCard[4];
    INT8U         kouKindNum;
    Card_t        kouCard[4];
    MingInfo_t    ming;
}Reminds_t;

#endif
