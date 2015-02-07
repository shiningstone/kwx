#ifndef _RACTION_H_
#define _RACTION_H_
#include "RaceType.h"
#include "OutCardList.h"

class Raction
{
public:
	virtual outCardList *getOutCardList()=0;//°¡°¡°¡°¡°¡°¡°¡°¡°¡
	virtual unsigned int get_ming_indexes()=0;
	virtual void set_ting_status(unsigned char flag)=0;
	virtual void set_ming_indexes(unsigned int indexesFlag)=0;
	virtual unsigned char ActiontodoCheckAgain()=0;
	virtual unsigned char init(int card_array[],int len,int aim)=0;
	virtual unsigned char hand_in(CARD_KIND kind,unsigned char who_give,unsigned char tingStatus,bool is_last_one,unsigned char last_action,unsigned int continue_gang_times,bool isGangHua)=0; 
	//virtual void kou_check(CARD_KIND river_list[],int river_len,CARD_KIND list[],int *len)=0;
	virtual int judge_kou_cards(CARD_KIND card,int no,CARD_KIND RototHandOutIndex)=0;
	virtual void MingCancel()=0;
	virtual CARD_KIND hand_out(unsigned int place)=0;
	virtual bool get_Hu_Flag(unsigned int *hu_kind)=0;
	virtual ACT_RES action(unsigned char who_give,ARRAY_ACTION act)=0;
	virtual void action1()=0;
	virtual long get_card_score()=0;
	virtual	void update_score(long score)=0;
	virtual	CARD_ARRAY *get_card_list()=0;
	virtual	unsigned char get_ting_status()=0;
	virtual bool get_ming_check_result(MRES *res)=0;
	virtual void get_hu_cards(CARD_KIND c_list[],int *len)=0;
	virtual void set_role_type(RT type)=0;
	virtual RT get_role_type()=0;
	virtual int *get_ming_reserved_cards_num(outCardList *river_list)=0;
	virtual unsigned int ming_check()=0;

	virtual void get_hu_NumForEveryCard(int curArray[MAX_HANDIN_NUM])=0;//ÕÅÊý
	virtual void get_hu_residueForEvery(int curArray[MAX_HANDIN_NUM][9])=0;//Ê£ÓàÅÆÊý
	virtual void get_huTiemsForEveryOne(int curArray[MAX_HANDIN_NUM][9])=0;//·¬ÐÍ
	virtual void get_hu_cards(CARD_KIND hu_cards[MAX_HANDIN_NUM][9])=0;//ºúÄÄ¼¸ÕÅÅÆ	

};
#endif

