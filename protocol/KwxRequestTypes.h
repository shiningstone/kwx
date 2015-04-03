
#ifndef __KXW_REQUEST_TYPES__
#define __KXW_REQUEST_TYPES__

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

typedef struct __MingChoice_t {
    Card_t           kind;       /*���Գ��Ļ�ɫ*/
    MsgTingInfo_t   ting;
}_MingChoice_t;

typedef struct __MingInfo_t {
    INT8U            choiceNum;     /*���Գ��Ļ�ɫ��Ŀ*/
    _MingChoice_t    *handouts;    
}_MingInfo_t;

typedef struct __Reminds_t {
    INT8U            actionNum;
    ActionId_t       actions[MAX_AVAIL_ACTIONS];
    INT8U            gangKindNum;
    Card_t           gangCard[4];
    INT8U            kouKindNum;
    Card_t           kouCard[4];
    _MingInfo_t      ming;
}_Reminds_t;

#endif
