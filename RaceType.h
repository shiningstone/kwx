#ifndef  _RACE_TYPE_H_
#define  _RACE_TYPE_H_

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
#include "Resources.h"

#define 	TOTAL_CARD_NUM 		84
#define 	TOTAL_CARD_KIND		21
#define 	HAND_IN_CARD_NUM    13
#define 	MAX_HANDIN_NUM      18
#define 	TOTAL_AIM_KIND      24
#define		INIT_CARDS_NUM		40

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
#define		RH_HAOHUAQIDUI			0x8000//ÁúÆß¶Ô
#define		RH_CHAOHAOHUAQIDUI		0x10000//Ë«ÁúÆß¶Ô
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

typedef enum card_kind
{
	ck_YI_TIAO=0,
	ck_ER_TIAO,
	ck_SAN_TIAO,
	ck_SI_TIAO,
	ck_WU_TIAO,
	ck_LIU_TIAO,
	ck_QI_TIAO,
	ck_BA_TIAO,
	ck_JIU_TIAO,

	ck_YI_TONG,
	ck_ER_TONG,
	ck_SAN_TONG,
	ck_SI_TONG,
	ck_WU_TONG,
	ck_LIU_TONG,
	ck_QI_TONG,
	ck_BA_TONG,
	ck_JIU_TONG,

	ck_ZHONG,
	ck_FA,
	ck_BAI,
	ck_NOT_DEFINED=0xff
}CARD_KIND;

typedef enum card_status
{
	c_FREE=0,
	c_PENG,
	c_MING_GANG,
	c_AN_GANG,
	c_MING_KOU,
	c_KOU_ENABLE,
	c_NOT_DEFINDED=0xff
}CARD_STATUS;
typedef enum card_play_status
{
	cps_NO=0,
	cps_YES,
	cps_NOT_DEFINDED=0xff
}CARD_PLAY_STATUS;

typedef enum array_action
{
    a_JUMP=0x00,
    a_PENG=0x01,
    a_MING_GANG=0x02,
    a_AN_GANG=0x04,
    a_MING=0x8,
    a_HU=0x10,
    a_SHOU_GANG=0x20,
    a_KOU=0x40,
    a_KOU_CANCEL=0x80,
    
    a_QIANG_GANG=0xfd,
    a_TING=0xfe,

    a_TIMEOUT=0xff
}ARRAY_ACTION;

typedef enum robot_target
{
	SAME_TIAO_TARGET=0,
	SAME_TONG_TARGET,
	SEVEN_COUPLES_TARGET,
	FOUR_PENG_TARGET,
	PI_HU_TARGET
}ROBOT_TARGET;

typedef enum Action_Result
{
	ar_DONE=0,
	ar_HU,
	ar_FAILED=0xff
}ACT_RES;


typedef enum Role_Type
{
	SINGLE_BOARD_ROBOT=0,
	SINGLE_BOADR_ME,
	INTERNET_PLAYER,
	INTERNET_ME,
}RT;

typedef struct Card
{
	CARD_KIND kind;
	CARD_STATUS status;
	CARD_PLAY_STATUS can_play;
	Card(){can_play=cps_YES;}
}CARD;

typedef struct Card_Array
{
	CARD data[MAX_HANDIN_NUM];
	int atcvie_place;
	int len;
}CARD_ARRAY;

typedef struct hash_array{
	int same_times;
	int place[4];
}HA;

typedef struct hash_array_head{
		HA list[TOTAL_CARD_KIND];
		ROBOT_TARGET target;
		CARD_KIND card_in_river[TOTAL_CARD_NUM];
		int river_len;
		int reserved_card_num;
		int hu_nums;
}HAH;

typedef struct ming_resource_info
{
	int hu_places;
	int hu_places_num;
	int hu_cards_num[MAX_HANDIN_NUM];
	CARD_KIND hu_cards[MAX_HANDIN_NUM][9];
}MRES;

typedef struct dist_card_info
{
	CARD_KIND card;
	int num;
}DCI;
#endif // _RACE_TYPE_H_
