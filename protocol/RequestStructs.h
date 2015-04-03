
#ifndef _REQUEST_STRUCTS_
#define _REQUEST_STRUCTS_

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"

#include "KwxMsgBasic.h"

#define MAX_AVAIL_ACTIONS 4

/*there is another TingInfo_t, maybe could combine them*/
typedef struct _TingItem_t {
    Card_t        kind;        /* ���Ժ��Ļ�ɫ */
    int           remain;      /* ʣ������     */
    int           fan;         /* ����         */
}TingItem_t;

typedef struct _MsgTingInfo_t {
    int              cardNum;      /* ���Ժ��Ļ�ɫ��Ŀ */
    TingItem_t       *cards;
}MsgTingInfo_t;

typedef struct _MingChoice_t {
    Card_t          kind;       /*���Գ��Ļ�ɫ*/
    MsgTingInfo_t   ting;
}MingChoice_t;

typedef struct _MingInfo_t {
    INT8U            choiceNum;     /*���Գ��Ļ�ɫ��Ŀ*/
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
