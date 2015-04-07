#ifndef __NET_ROLE_H__
#define __NET_ROLE_H__

#include "./../OutCardList.h"
#include "./../RaceType.h"
#include "./../Raction.h"

#include "./../utils/BasicType.h"

class NetRole {
	public:
		virtual void set_robot_hu_target(ROBOT_TARGET par_target)=0;
		virtual ROBOT_TARGET get_robot_hu_target()=0;
		virtual int chose_card(HAH *pres,int reseved,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2){return 0;};
		virtual void init_target(ROBOT_TARGET *target,int hu_len1,int hu_len2){};
		void set_sex(std::string sex); 
		Sex_t get_sex();

		virtual unsigned char init(int card_array[],int len,int aim)=0;
		virtual void set_player_id(int player_id)=0;
		virtual bool get_player_id(int & player_id)=0;
		virtual bool get_nick_name(std::string & name)=0;
		virtual void set_nick_name(const std::string name)=0;
		virtual void set_photo(const std::string photo)=0;
		virtual bool get_photo(std::string & photo)=0;
		virtual void set_property(int pro)=0; 
		virtual bool get_property(int & pro)=0;
		virtual void set_language(std::string language)=0; 
		virtual bool get_language(std::string & language)=0;
		virtual Raction* get_parter()=0;
		virtual void set_parter(Raction* p_parter)=0;
    protected:
        Sex_t _sex;

        int idOfPlayer;
        std::string nickName;
        std::string photo;
        std::string playerSex;
        std::string playerLanguage;
        int property;
        CARD_KIND dist_card;
		Raction *parter;
};

#endif
