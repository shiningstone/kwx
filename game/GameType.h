
#ifndef __GAME_TYPE__
#define __GAME_TYPE__

#include "./../utils/UtilBasic.h"

#define     INVALID             -1

#define     PLAYER_NUM          3
#define     ROBOT_MAX_NUM       16
#define 	TOTAL_CARD_NUM 		84
#define 	TOTAL_CARD_KIND		21
#define 	MAX_HANDIN_NUM      18

#define 	TOTAL_AIM_KIND      24

#define 	RH_MING 	 			0x01
#define		RH_ZIMO 	 			0x02
#define		RH_QINYISE 				0x04
#define		RH_SIPENG  				0x08
#define		RH_HAIDILAO				0x40000
/**/
#define 	RH_QIANGGANG			0x10
#define 	RH_GANGHUA   			0x20
#define 	RH_GANGPAO				0x40
/////////////////////////////////////////
#define		RA_ANGANGHU				0x10
#define		RA_GANGHU				0x20
/**/
#define		RH_SHOUYIZHUA			0x80
#define 	RH_DASANYUAN			0x100
#define 	RH_XIAOSANYUAN 			0x200
#define 	RH_MINGSIGUI			0x400
#define 	RH_ANSIGUI				0x800
#define		RH_QIDUI				0x1000
#define 	RH_SANYUANQIDUI 		0x2000
#define 	RH_KAWUXIN				0x4000
#define		RH_HAOHUAQIDUI			0x8000//龙七对
#define		RH_CHAOHAOHUAQIDUI		0x10000//双龙七对
#define		RH_CHAOCHAOHAOHUAQIDUI 	0x20000//3
#define 	RH_ZHONG				0x20000000
#define 	RH_FA					0x40000000
#define 	RH_BAI					0x80000000

/*aim prize have not defined*/
#define		RA_NOTASK				0
#define		RA_SHENG				RH_KAWUXIN|RH_ZIMO
#define		RA_XING					RH_QINYISE
#define		RA_BAI					RA_ANGANGHU
#define		RA_BIAN					RH_SIPENG|RH_ZHONG
#define		RA_DOU					RH_SIPENG|RH_FA
#define		RA_ZHUAN				RH_SIPENG|RH_BAI
#define		RA_XINGX				RH_MINGSIGUI|RH_KAWUXIN
#define 	RA_YI					RA_GANGHU|RH_BAI
#define		RA_XIAO					RA_GANGHU|RH_FA
#define		RA_WU					RA_GANGHU|RH_ZHONG
#define		RA_XIANG				RH_DASANYUAN
#define		RA_GONG					RH_HAOHUAQIDUI
#define		RA_DA					RH_XIAOSANYUAN
#define  	RA_GOU					RH_ANSIGUI|RH_KAWUXIN
#define		RA_BANG					RH_MING|RH_QINYISE
#define		RA_FA					RH_ANSIGUI
#define		RA_JIU					RA_GANGHU|RH_KAWUXIN
#define		RA_YIN					RH_MING|RH_ZIMO
#define		RA_ZHEN					RH_MINGSIGUI|RH_QINYISE
#define		RA_JING					RH_ANSIGUI|RH_QINYISE
#define		RA_KUI					RH_QIDUI
#define		RA_HUA					RH_SHOUYIZHUA
#define		RA_BAO					RH_MING|RH_SHOUYIZHUA
#define		RA_DIAN					RH_MINGSIGUI

typedef enum {
    LOCAL_GAME,
    NETWORK_GAME,
}GameMode_t;

typedef enum {
    LEFT,
    MIDDLE,
    RIGHT,
    INVALID_DIR,
    NEXT          = 3,
    DOUBLE_PLAYER = 3,
    SERVER        = 4,
}PlayerDir_t;

typedef enum {
    aNULL      = 0,
    aQi        = 0,
    aPENG      = 0x1,
    aMING_GANG = 0x2,
    aAN_GANG   = 0x4,
    aMING      = 0x8,
    aHU        = 0x10,
    aSHOU_GANG = 0x20,
    aKOU       = 0x40,
    aKOU_CANCEL= 0x80,

    aGANG      = aMING_GANG|aAN_GANG|aSHOU_GANG,
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
    sGANG_ENABLE,
    sGANG,
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
    int           huNum;        /* 可以胡的总张数   */
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


typedef enum _RobotTarget_t {
	SAME_TIAO,
	SAME_TONG,
	SEVEN_COUPLES,
	FOUR_PENG,
	PI_HU
}RobotTarget_t;

#endif

