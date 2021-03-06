#ifndef __ROLE_H__
#define __ROLE_H__
#include "OutCardList.h"
#include "RaceType.h"
#include "Raction.h"

class Role
{
	public:
		//Role();
		//~Role();
	public:
		virtual unsigned char init(int card_array[],int len,int aim)=0;
		virtual void init_target(ROBOT_TARGET *target,int hu_len1,int hu_len2){};
		virtual void set_player_id(int player_id)=0;
		virtual bool get_player_id(int & player_id)=0;
		virtual void set_nick_name(const std::string name)=0;
		virtual bool get_nick_name(std::string & name)=0;
		virtual void set_photo(const std::string pho)=0;
		virtual bool get_photo(std::string & pho)=0;
		virtual void set_property(int pro)=0; 
		virtual bool get_property(int & pro)=0;
		virtual void set_sex(std::string sex)=0; 
		virtual bool get_sex(std::string & sex)=0;
		virtual void set_language(std::string language)=0; 
		virtual bool get_language(std::string & language)=0;
		virtual Raction* get_parter()=0;
		virtual void set_parter(Raction* p_parter)=0;
		virtual void set_robot_hu_target(ROBOT_TARGET par_target)=0;
		virtual ROBOT_TARGET get_robot_hu_target()=0;
		virtual int chose_card(HAH *pres,int reseved,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2){return 0;};
};
#endif // __ROLE_H__