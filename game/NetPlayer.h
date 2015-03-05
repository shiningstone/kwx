
#ifndef __NET_PLAYER__
#define __NET_PLAYER__

/* This file is copied from Robot.h/Robot.cpp */

#include "NetRaceRound.h"
#include "Raction.h"
#include "RaceType.h"

#include "NetRole.h"
#include "DataBase.h"

class Logger;
class NetPlayer:public NetRole
{
	public:
		NetPlayer();
        NetPlayer(int id);
		virtual ~NetPlayer();
    private:
        Logger   *_logger;
        //Profile_t _profile;

		int property;
		int idOfPlayer;
		std::string nickName;
		std::string photo;
		std::string playerSex;
		std::string playerLanguage;
		Raction *parter;
		CARD_KIND dist_card;
		ROBOT_TARGET g_target;
	private:
		int Robot_check_pickup_card(CARD_KIND kind,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int river_reserved_card(HAH *hash_table,int card);//
		int Robot_check_card_stable(HAH *card_array,CARD_KIND card);//
		int Robot_picup_single_for_samecolor(int color,HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_for_sevencouples(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_for_fourpeng(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_for_lowwin(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_single(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
	public:
		unsigned char init(int card_array[],int len,int aim);
		void init_target(ROBOT_TARGET *target,int hu_len1,int hu_len2);
		int chose_card(HAH *pres,int reseved,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//

		void set_player_id(int player_id);
		bool get_player_id(int & player_id);

		void set_nick_name(const std::string name);
		bool get_nick_name(std::string & name);
		void set_photo(const std::string pho);
		bool get_photo(std::string & pho);
		void set_property(int pro);
		bool get_property(int & pro);
		void set_language(std::string language); 
		bool get_language(std::string & language);
		void get_hu_cards(CARD_KIND c_list[],int *len);
		Raction* get_parter();
		void set_parter(Raction* p_parter);
		int get_dist_card(CARD_KIND &card);
		void set_dist_card(const CARD_KIND card);
		void set_robot_hu_target(ROBOT_TARGET par_target);
		ROBOT_TARGET get_robot_hu_target();
};

#endif // __ROBOT_H__
