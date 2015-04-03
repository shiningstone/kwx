
#ifndef _REQUEST_STRUCTS_
#define _REQUEST_STRUCTS_

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"

#include "KwxMsgBasic.h"

#define MAX_AVAIL_ACTIONS 4

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
