
#ifndef _KXW_REQUEST_TYPES_
#define _KXW_REQUEST_TYPES_

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"

#define MAX_AVAIL_ACTIONS 4

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

typedef enum {
    aQi        = 0,
    aPENG      = 0x1,
    aMING_GANG = 0x2,
    aAN_GANG   = 0x4,
    aMING      = 0x8,
    aHU        = 0x10,
    aSHOU_GANG = 0x20,
    aKOU       = 0x40,
    aKOU_CANCEL= 0x80,
}ActionId_t;/*ARRAY_ACTION???*/

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

typedef struct _MingChoice_t {
    Card_t          kind;       /*可以出的花色*/
    MsgTingInfo_t   ting;
}MingChoice_t;

typedef struct _MingInfo_t {
    INT8U            choiceNum;     /*可以出的花色数目*/
    MingChoice_t    *handouts;    
}MingInfo_t;

typedef struct _Reminds_t {
    INT8U            actionNum;
    ActionId_t       actions[MAX_AVAIL_ACTIONS];
    INT8U            gangKindNum;
    Card_t           gangCard[4];
    INT8U            kouKindNum;
    Card_t           kouCard[4];
    MingInfo_t      ming;
}Reminds_t;

#endif
