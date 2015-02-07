#ifndef  _RESOURCES_H_
#define  _RESOURCES_H_

/*local database,updated from if internet connect*/
//#define MAX_HEAD_IMAGE_NUM				20
#define INT_LAST_LOAD_TIME             	"load_time"
#define INT_CONTINUE_LOAD_DAYS			"load_days"
#define INT_LOAD_PRIZE_STATUS			"cur_sta"
#define INT_VOICE_LEVEL					"voice_level"
#define INT_VOICE_EFFECT				"voice_effeect"
#define BOOL_CLICK_MODE					"clicks"
#define BOOL_IS_SHAKE					"is_shake"
#define STR_HELP_INFO					"help_doc"
#define INT_PLAYER_ID                   "player_id"
#define INT_ACCOUNT						"account_type"
#define STR_HEAD_IMAGE					"image_path"
#define STR_NICKNAME					"nick_name"
#define BOOL_GENDER						"gender"
#define INT_WIN_NUM						"wins"
#define INT_LOSE_NUM					"loses"
#define INT_MAX_ROUND_SCORE				"max_score"
#define INT_TOTAL_SCORE					"total_score"
#define INT_RANK_INFO					"rank"

#define START_CALLBACK_EVENT_TYPE		"start_callback"
#define WAIT_START_CALLBACK_EVENT_TYPE	"wait_race_again"
#define NOONE_WIN_EVENT_TYPE			"noone_win"
#define DISTRIBUTE_CALL_EVENT_TYPE		"distribute_card_call"
#define DISTRIBUTE_DONE_EVENT_TYPE      "distribute_card_done"
#define ALL_PALYER_HU_DONE_EVENT_TYPE	"hu_done"
#define DOUBLE_HU_WITH_ME				"double_hu"

/*clock reserved time children*/
#define ALARM_CLOCK_CHILD_NUM_TAG_ID		1

/*reserved card related children*/
#define RESERVED_BKG_CHILD_TAG_ID			1//（1，5）

/*head image related children*/
#define SCORE_TAG_ID						1
#define NICK_NAME_TAG_ID					2
#define HEAD_IMG_TAG_ID						3
#define PROPERTY_FONT_UNIT					4

/*Race Accout layer related children*/
#define ACCOUNTBAR_PLAYERZERO				0
#define ACCOUNTBAR_PLAYERONE				1
#define ACCOUNTBAR_PLAYERTWO				2
#define ACCOUNT_JINBI						1
#define ACCOUNT_PROPRETY					2
#define ACCOUNT_PROPRETY_UNIT				1
#define ACCOUNT_NIKENAME					3
#define ACCOUNT_IMAGE						4
#define ACCOUNT_HU_FONT						5
#define ACCOUNT_ZIMO_FONT					6
#define ACCOUNT_DIANPAO_FONT				7
#define ACCOUNT_ADD_SYMBOL					8
#define ACCOUNT_MINUS_SYMBOL				9
#define ACCOUNT_WINGOLD_NUM					10
#define ACCOUNT_BAOZHUANG_FONT				11

/*this scene related children*/
#define GOLD_NUM_INSERT_JINBI				400
#define GOLD_NUM_INSERT_JIAHAO				401
#define GOLD_NUM_INSERT_JIANHAO				402
#define GOLD_NUM_INSERT_NUMBER				403

/*GAME myframe cards related children*/
#define RACE_ACCOUT_TAG_ID					1//结算页面
#define START_CARDS_IN_TAG_ID				2//牌局开始发牌TAG

#define HAND_IN_CARDS_TAG_ID				100//（100，160）
#define HAND_OUT_CARDS_TAG_ID				300//（300，360）

#define GAME_BKG_TAG_ID 					200//myframe->tag
			//delete_act_tip   actionRemind//
#define REMIND_ACT_TAG_ID 					500//
#define QI_REMIND_ACT_TAG_ID				501//qi
#define QI_REMIND_ACT_BKG_TAG_ID			502//
#define GANG_REMING_ACT_TAG_ID				503//gang
#define GANG_REMING_ACT_BKG_TAG_ID			504//
#define HU_REMIND_ACT_TAG_ID				505//hu
#define HU_REMIND_ACT_BKG_TAG_ID			506//
#define PENG_REMIND_ACT_TAG_ID				507//peng
#define PENG_REMIND_ACT_BKG_TAG_ID			508//
#define MING_REMIND_ACT_TAG_ID				509//ming
#define MING_REMIND_ACT_BKG_TAG_ID			510//
#define MING_KOU_ENSURE						511
#define MING_KOU_CANCEL						512
#define MING_CANCEL							513
#define MING_KOU_SIGN						514
#define HUANG_ZHUANG_LAYER					515
#define HUANG_ZHUANG_FONT					516


#define MING_STATUS_PNG_0					519//明牌提示图片
#define MING_STATUS_PNG_1					520
#define MING_STATUS_PNG_2					521
			//UnnecessaryResource//
#define TING_SING_BUTTON					550//番型提示
#define TING_SING_BAR						551
#define TING_SING_LEFTBAR					552
#define TING_SING_RIGHTBAR					553	
#define ROBOT_TUO_GUAN						554//托管LayerColor
#define TUOGUAN_CANCEL_BUTTON				555
#define EFFECT_TEMP_CARD_TAG_ONE			556//effect card tempTag
#define EFFECT_TEMP_CARD_TAG_TWO			557
#define EFFECT_TEMP_CARD_TAG_THREE			558
#define EFFECT_TEMP_CARD_TAG_FOUR			559
///////////////////////////////////////////////
					//action effect//
#define MOJI_EFFECT_TAG_ID					700
#define MOJI1_EFFECT_TAG_ID					701
#define IMG_101_EFFECT_TAG_ID				702
#define IMG_4_EFFECT_TAG_ID					703
#define IMG_Q13_EFFECT_TAG_ID				704
#define IMG_Q13_2_EFFECT_TAG_ID				705
#define IMG_888_EFFECT_TAG_ID				706
#define IMG_104_EFFECT_TAG_ID				707
#define IMG_444_EFFECT_TAG_ID				708
#define IMG_666_EFFECT_TAG_ID				709
#define SINGLE_ACTION_EFFECT_TAG_ID			710//右
#define SINGLE_ACTION2_EFFECT_TAG_ID		711//1
#define SINGLE_ACTION3_EFFECT_TAG_ID		712
#define SINGLE_ACTION4_EFFECT_TAG_ID		713
#define IMG_Q3_EFFECT_TAG_ID				714
#define IMG_Q15_EFFECT_TAG_ID				715
#define IMG_G1_EFFECT_TAG_ID				716
#define IMG_C3_EFFECT_TAG_ID				717
#define IMG_C33_EFFECT_TAG_ID				718
#define IMG_Q4_EFFECT_TAG_ID				719
#define EFFET_NEWCATD1_TAG					720//效果牌tag
#define EFFET_NEWCATD2_TAG					721
#define EFFET_NEWCATD3_TAG					722
#define EFFET_NEWCATD4_TAG					723
#define PARTICLE_SOMKE						724
#define PARTICLE_SMALL						725
#define PENG_EFFECT_NODE_ID					726//effect action run Node
#define MING_GANG_EFFECT_NODE				727
#define AN_GANG_EFFECT_NODE					728
#define MING_EFFECT_DAMING					729//明牌效果
#define MING_EFFECT_ANIMATE					730

#define CHOOSE_CARD_TAG_ID					777//选中的牌
#define OUT_CARD_FRAME_TAG_ID 				778//出的牌
#define SHOW_CARD_INIDCATOR_TAD_ID			779//出牌提示框



#define MING_KOU_MASK						2
#define MING_KOU_BKG						2
#define MING_KOU							2
///////////////////this scene related children////////////////////////////
#define CENTER_BKG_TAG_ID					100
#define MENU_BKG_TAG_ID						101
#define RESERVED_BKG_TAG_ID					102
//#define SYSTEM_INFO_BKG_TAG_ID				103//公告条
//#define HORN_TAG_ID							104//喇叭
#define LEFT_IMG_BKG_TAG_ID					105
#define RIGHT_IMG_BKG_TAG_ID				106
#define MID_IMG_BKG_TAG_ID					107
#define MIC_TAG_ID							108
#define SINGLE_PLAY_TAG_ID					109
#define ALARM_CLOCK_INDICATOR_TAG_ID		110
#define ALARM_CLOCK_INDICATE_DOWN_TAG_ID   	111
#define ALARM_CLOCK_INDICATE_RIGHT_TAG_ID 	112
#define ALARM_CLOCK_INDICATE_LEFT_TAG_ID	113
#define SHOWCARD_INDICATOR_TAG_ID			114
#define READY_INDICATE_LEFT_TAG_ID			115
#define READY_INDICATE_RIGHT_TAG_ID			116
#define READY_INDICATE_MID_TAG_ID 			117
#define START_GAME_TAG_ID               	118
#define START_GAME_SHADE_TAG_ID				119//???
#define CARD_ARRAGE_LEFT_TAG_ID				120
#define CARD_ARRAGE_BOTTOM_TAG_ID			121
#define CARD_ARRAGE_RIGHT_TAG_ID 			122
#define CARD_ARRAGE_TOP_TAG_ID				123
#define COLOR_LAYER_TAG_ID					124
#define RACE_START_TAG_ID					125
#define RACE_LIGHT_TAG_ID					126
#define LAST_WINNER_TAG_ID					127
#define RACE_RESTART_TAG_ID					128
#define SHINE_TAG_ID						129
#define GAME_BACK_BAR						130

#define BAOMING_MENU_BUTTON					1
#define TUOGUAN_MENU_BUTTON					2
#define SHEZHI_MENU_BUTTON					3
#define SHOP_MENU_BUTTON					4
#define GAMEBACK_MENU_BUTTON				5

					

/*updated when recv from internet*/
typedef struct PrizeList
{
	int key;
	int times;
	int status;
	int prize;
	char name[32];
	struct PrizeList *next;
}PLIST;

typedef struct BagList
{
	char name[32];
	char info[32];
	int num[4];
	int status_num;
	struct BagList *next;
}BLIST;

#endif // _RESOURCES_H_
