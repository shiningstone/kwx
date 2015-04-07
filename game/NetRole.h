#ifndef __NET_ROLE_H__
#define __NET_ROLE_H__

#include "./../OutCardList.h"
#include "./../RaceType.h"
#include "./../NetRaceRound.h"

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

class NetRole {
	public:
		NetRole();
        NetRole(int id);

		virtual void set_robot_hu_target(ROBOT_TARGET par_target);
		virtual ROBOT_TARGET get_robot_hu_target();

		void set_sex(std::string sex); 
		Sex_t get_sex();

		virtual unsigned char init(int card_array[],int len,int aim);
		virtual void set_player_id(int player_id);
		virtual bool get_player_id(int & player_id);
		virtual bool get_nick_name(std::string & name);
		virtual void set_nick_name(const std::string name);
		virtual void set_photo(const std::string photo);
		virtual bool get_photo(std::string & photo);
		virtual void set_property(int pro); 
		virtual bool get_property(int & pro);
		virtual void set_language(std::string language); 
		virtual bool get_language(std::string & language);
		virtual NetRRound* get_parter();
		virtual void set_parter(NetRRound* p_parter);
        
    protected:
        Logger   *_logger;

        Sex_t _sex;

        int idOfPlayer;
        std::string nickName;
        std::string photo;
        std::string playerSex;
        std::string playerLanguage;
        int property;
        CARD_KIND dist_card;
		NetRRound *parter;
};

#endif
