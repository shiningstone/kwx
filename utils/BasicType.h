
#ifndef __BASIC_TYPE__
#define __BASIC_TYPE__

#include "UtilBasic.h"

#define INVALID -1
#define PLAYER_NUM  3

typedef enum {
    LOCAL_GAME,
    NETWORK_GAME,
}GameMode_t;

typedef enum {
    LEFT,
    MIDDLE,
    RIGHT,
    INVALID_DIR,
    DOUBLE_PLAYER = 3,
    SERVER        = 4,
}PlayerDir_t;

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

    aQIANG_GANG= 0xfd,
    aTING      = 0xfe,

    aLIU_JU    = 0x80000
}ActionId_t;

typedef INT32U ActionMask_t;

typedef enum {
    TIAO_1,
    TIAO_2,
    TIAO_3,
    TIAO_4,
    TIAO_5,
    TIAO_6,
    TIAO_7,
    TIAO_8,
    TIAO_9,
    TONG_1,
    TONG_2,
    TONG_3,
    TONG_4,
    TONG_5,
    TONG_6,
    TONG_7,
    TONG_8,
    TONG_9,
    ZHONG,
    FA,
    BAI,        
    CARD_KIND_MAX,
    CARD_HIDE = 0xfe,
    CARD_UNKNOWN = 0xff,
}Card_t;

typedef enum {
	sFREE = 0,
	sPENG,
	sMING_GANG,
	sAN_GANG,
	sMING_KOU,
	sKOU_ENABLE,
	sUNDEFINDED=0xff,
}CardStatus_t;

typedef struct _CardNode_t {
	Card_t       kind;
	CardStatus_t status;
	bool         canPlay;
}CardNode_t;

typedef enum {
    BOY,
    GIRL,
    SEX_MAX,
}Sex_t;

typedef enum {
    SINGLE_WIN,
    DOUBLE_WIN,
    NONE_WIN,
}WinKind_t;

typedef struct {
    WinKind_t    kind;
    PlayerDir_t  winner;
    PlayerDir_t  giver;
}WinInfo_t;

typedef enum {
    LAYDOWN_HIDE,
    LAYDOWN_SHOW,
    NORMAL_APPERANCE,
}CartApperance_t;

typedef enum {
    AN_GANG = 1,
    MING_GANG,
    HU_WIN,
}GoldKind_t;

typedef int TagId_t;

typedef struct _TingItem_t {
    Card_t        kind;        /* 可以胡的花色 */
    int           remain;      /* 剩余张数     */
    int           fan;         /* 番数         */
}TingItem_t;

typedef struct _TingInfo_t {
    int           cardNum;      /* 可以胡的花色数目 */
    TingItem_t    *cards;       /* 注意: 这是一个数组指针，使用者需要负责动态分配内存*/
}TingInfo_t;

typedef struct _MingChoice_t {
    Card_t        kind;         /*可以出的花色*/
    TingInfo_t    ting;          
}MingChoice_t;

typedef struct _MingInfo_t {
    INT8U         choiceNum;    /*可以出的花色数目*/
    MingChoice_t  *handouts;    /* 注意: 这是一个数组指针，使用者需要负责动态分配内存*/
}MingInfo_t;

typedef unsigned int HuTarget_t;
typedef unsigned int HuFan_t;

#endif

