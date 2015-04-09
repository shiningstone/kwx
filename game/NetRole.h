#ifndef __NET_ROLE_H__
#define __NET_ROLE_H__

#include "./../OutCardList.h"
#include "./../RaceType.h"

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

#include "CardList.h"
#include "NetRaceRound.h"
#include "DataBase.h"

class NetRole {
public:
	NetRole();
    NetRole(int id);
    ~NetRole();

	virtual unsigned char init(int card_array[],int len,int aim);
    virtual int chose_card(HAH *pres,int reseved,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2){return 0;}
    virtual void set_robot_hu_target(ROBOT_TARGET par_target){}
    virtual ROBOT_TARGET get_robot_hu_target(){return SAME_TIAO_TARGET;}
    
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
    CardList *_river;
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
