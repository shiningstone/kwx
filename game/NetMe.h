#ifndef __NET_ME_H__
#define __NET_ME_H__

#include "Raction.h"

#include "NetRole.h"
#include "utils/LogManager.h"

class NetMe:public NetRole
{
	public:
		NetMe();
		virtual ~NetMe();
	private:
        Logger *_logger;

		Raction *parter;
		int idOfPlayer;
		std::string nickName;
		std::string photo;
		std::string playerSex;
		std::string playerLanguage;
		int property;
		CARD_KIND dist_card;
	//private:
		//void get_hu_cards(CARD_KIND *c_list[],int *len);
	public:
		unsigned char init(int card_array[],int len,int aim);/**/
		virtual void set_player_id(int player_id);/**/
		virtual bool get_player_id(int & player_id);/**/
		bool get_nick_name(std::string & name);/**/
		void set_nick_name(const std::string name);/**/
		void set_photo(const std::string photo);/**/
		bool get_photo(std::string & photo);/**/
		void set_property(int pro);/**/
		bool get_property(int & pro);/**/
		void set_sex(std::string sex); 
		bool get_sex(std::string &sex);
		void set_language(std::string language); 
		bool get_language(std::string &language);
		int get_dist_card(CARD_KIND &card);/**/
		void set_dist_card(const CARD_KIND card);/**/
		Raction* get_parter();/**/
		void set_parter(Raction* p_parter);/**/
		void set_robot_hu_target(ROBOT_TARGET par_target);
		ROBOT_TARGET get_robot_hu_target();
		//int chose_card(int target,int reseved,CARD_KIND river_list[],CARD_KIND list1[],CARD_KIND list2[],int len,int len1,int len2);
};

#endif // __Me_H__