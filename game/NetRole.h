#ifndef __NET_ROLE_H__
#define __NET_ROLE_H__

#include "./../OutCardList.h"
#include "./../RaceType.h"

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

#include "CardCollection.h"
#include "NetRaceRound.h"
#include "DataBase.h"

class NetRole {
public:
	NetRole();
    NetRole(int id);
    ~NetRole();

    virtual int chose_card(HAH *pres,int reseved,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2){return 0;}
    virtual void init_target(ROBOT_TARGET *target,int hu_len1,int hu_len2){}
    virtual void set_robot_hu_target(ROBOT_TARGET par_target){}
    virtual ROBOT_TARGET get_robot_hu_target(){return SAME_TIAO_TARGET;}

	virtual unsigned char init(int card_array[],int len,int aim);
    virtual unsigned char init(Card_t card_array[],int len,int aim);
    
    bool          _isReady;
    UserProfile_t _profile;    
    NetRRound    *_act;

    /**************************************************
            user's action
    **************************************************/
    virtual NetRRound* get_parter();

    /**************************************************
            user's card info
    **************************************************/
    CardList   *_river;
    CardInHand *_cards;
    /**************************************************
            user's profile
    **************************************************/
    Sex_t GetSex();
    int   UpdateProperty(int change);

    void Set(const UserProfile_t *profile);
protected:
    Logger    *_logger;
};

#endif
