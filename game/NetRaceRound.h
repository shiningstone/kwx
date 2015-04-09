#ifndef  _NET_RACE_ROUND_H_
#define  _NET_RACE_ROUND_H_

#include "Role.h"

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

#include "CardList.h"
class NetRRound
{
public:
	NetRRound();
    virtual ~NetRRound();

private:
    Logger *_logger;

    unsigned int  rr_aim;
    unsigned char rr_ting_flag;
    unsigned int  archive_ming_indexes;
    
    long card_score;
	int InsertPlaceForMG;
	unsigned int kind_hu;
    CARD_ARRAY *card_list;
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
    int card_delete(unsigned int from,unsigned int len);
    void card_insert(CARD data,int times);
    int pattern_match(CARD_KIND data[],int len);
    int cards_stable(CARD_KIND clist[],int len);
    long cal_score(CARD_KIND kind,unsigned char who_give,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua);//分数计算
    int cal_times(CARD_KIND kind,CARD_KIND data[],int len);
    void task_check(unsigned int flag);//修改1//,unsigned char last_action
    int hu_check(CARD_KIND data_kind); //0:not 1:hu
    unsigned int ting_check(int index,CARD_KIND cur_card,int kind,CARD_KIND rlist[]);
    void array_sort(CARD clist[],int index,int len,CARD_KIND kind,CARD_KIND rlist[]);
    void array_sort2(CARD clist[],int index1,int index2,int len,CARD_KIND kind1,CARD_KIND kind2,CARD_KIND rlist[]);
public:
	int judge_kou_cards(CARD_KIND card,int no,CARD_KIND otherHandedOut);
    unsigned int ming_check();
	unsigned char init(int card_array[],int len,int aim);
    unsigned char hand_in(CARD_KIND kind,unsigned char who_give,unsigned char tingStatus,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua); //0:sever, 1:player
    CARD_KIND hand_out(unsigned int place);
    ACT_RES action(unsigned char who_give,ARRAY_ACTION act);
    ACT_RES net_action(unsigned char who_give,ARRAY_ACTION act,Card_t kind);
	void MingCancel();
	void LockAllCards();
    unsigned int get_aim();
	unsigned int get_ming_indexes();
	int *get_ming_reserved_cards_num(CardList *list);
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
