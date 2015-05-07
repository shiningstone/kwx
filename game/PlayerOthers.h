
#ifndef __PLAYER_OTHERS__
#define __PLAYER_OTHERS__

#include "RaceType.h"

#include "Player.h"

class Logger;
class PlayerOthers:public Player
{
	public:
		PlayerOthers();
        PlayerOthers(int id);
		virtual ~PlayerOthers();
    private:
        Logger   *_logger;

		ROBOT_TARGET g_target;
	private:
        void _SetContext(HAH *res,CARD_KIND target1[],CARD_KIND target2[],int *len1,int *len2,const RoundManager &context) const;
        
		int Robot_check_pickup_card(CARD_KIND kind,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int river_reserved_card(HAH *hash_table,int card);//
		int Robot_check_card_stable(HAH *card_array,CARD_KIND card);//
		int Robot_picup_single_for_samecolor(int color,HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_for_sevencouples(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_for_fourpeng(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_for_lowwin(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_single(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
	public:
        int  choose_worst(const RoundManager &context,bool &canKou) const;
		void set_robot_hu_target(ROBOT_TARGET par_target);
		ROBOT_TARGET get_robot_hu_target();
		int chose_card(HAH *pres,int reseved,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
};

#endif 

