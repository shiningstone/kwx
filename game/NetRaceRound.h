#ifndef  _NET_RACE_ROUND_H_
#define  _NET_RACE_ROUND_H_

#include "Role.h"

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

#include "CardCollection.h"

class CardList;
class NetRRound
{
public:
	NetRRound(CardInHand *cardInHand,HuFan_t &fan,HuTarget_t aim);
    virtual ~NetRRound();

	unsigned char init(int card_array[],int len);

    CardInHand *_cardInHand;
private:
    Logger *_logger;

    const HuTarget_t _AIM;
    HuTarget_t       _aimDone;
	HuFan_t         &_fan;

    TingInfo_t       _TingInfo;
    void get_hu_residueForEvery2(int curArray[MAX_HANDIN_NUM][9]);
    unsigned int ming_check2();
    void InitTingInfo();

    unsigned int  archive_ming_indexes;
    
    long card_score;
	int InsertPlaceForMG;
	int hu_places;
	int hu_places_num;
	CARD_KIND hucards[9];
	int hu_len;
	int huTiemsForEveryOne[MAX_HANDIN_NUM][9];//番型
	int hu_reserved_num[MAX_HANDIN_NUM];//总剩余牌数
	int hu_residueForEvery[MAX_HANDIN_NUM][9];//剩余牌数
	int hu_cards_num[MAX_HANDIN_NUM];
	int hu_NumForEveryCard[MAX_HANDIN_NUM];//胡张数
	CARD_KIND hu_cards[MAX_HANDIN_NUM][9];//胡哪几张牌
    long calcScore(Card_t kind,bool isNewDistributed,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua);//分数计算
    long calcTimes(Card_t kind);
    void task_check(unsigned int flag);//修改1//,unsigned char last_action
public:
    unsigned int ming_check();
    unsigned char hand_in(CARD_KIND kind,unsigned char isNewDistributed,unsigned char tingStatus,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua); //0:sever, 1:player
    CARD_KIND hand_out(unsigned int place);
    ACT_RES action(bool isCardFromOther,ARRAY_ACTION act);
    ACT_RES others_action(bool isNewDistributed,ARRAY_ACTION act,Card_t kind);
	void MingCancel();
	void LockAllCards();
    unsigned int get_aim();
	unsigned int get_ming_indexes();
	int *get_ming_reserved_cards_num(CardList *river);
	void set_ming_indexes(unsigned int indexesFlag);
	unsigned char ActiontodoCheckAgain();
	void set_ting_status(unsigned char flag);
	unsigned char get_ting_status();
    long get_card_score();
	void get_hu_cards(CARD_KIND c_list[],int *len);
	bool get_Hu_Flag(unsigned int *hu_kind);
	bool get_ming_check_result(MRES *res);
	void get_hu_NumForEveryCard(int curArray[MAX_HANDIN_NUM]);//张数
	void get_hu_residueForEvery(int curArray[MAX_HANDIN_NUM][9]);//剩余牌数
	void get_huTiemsForEveryOne(int curArray[MAX_HANDIN_NUM][9]);//番型
	void get_hu_cards(CARD_KIND hu_cards[MAX_HANDIN_NUM][9]);//胡哪几张牌	
};

#endif // _RACE_ROUND_H_
