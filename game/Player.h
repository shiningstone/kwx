#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "./../RaceType.h"

#include "./../utils/BasicType.h"

#include "DataBase.h"

class Player {
public:
	Player();
    Player(int id);
    ~Player();

    virtual int chose_card(HAH *pres,int reseved,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2){return 0;}

    virtual unsigned char init(int cards[],int len,int aim);
    virtual unsigned char init(Card_t cards[],int len,int aim);
    
    bool          _isReady;
    UserProfile_t _profile;    

    /**************************************************
            user's action
    **************************************************/
    long calcScore(Card_t kind,bool isNewDistributed,bool isLastOne,ActionId_t last_action_WithGold,unsigned int continue_gang_times,bool isGangHua);//·ÖÊý¼ÆËã
    void taskCheck(unsigned int flag);

    ActionMask_t hand_in(Card_t kind,bool isNewDistributed,bool tingStatus,bool isLastOne,ActionId_t last_action_WithGold,unsigned int continue_gang_times,bool isGangHua); //0:sever, 1:player
    Card_t       hand_out(unsigned int place);
	ActionMask_t ActiontodoCheckAgain();

    ACT_RES action(bool isNewDistributed,ActionId_t act);
    ACT_RES others_action(bool isNewDistributed,ActionId_t act,Card_t kind);

    HuTarget_t get_aim() const;
	HuTarget_t get_hu_flag() const;
    long       get_score() const;

    HuTarget_t       _AIM;
    HuTarget_t       _aimDone;
    HuFan_t          _fan;
    long             _score;
    /**************************************************
            user's card info
    **************************************************/
    /* during game */
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
