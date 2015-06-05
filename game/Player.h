#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameType.h"

#include "StrategyPlayer.h"
#include "DataBase.h"

class Player {
public:
	Player(PlayerDir_t dir);
    ~Player();

    virtual unsigned char init(Card_t cards[],int len,int aim);
    virtual unsigned char refresh(CardNode_t cards[],int len,HuFan_t fan);
    virtual unsigned char refresh(CardNode_t cards[],int len);

    StrategyPlayer *_strategy;

    bool          _isReady;
    UserProfile_t _profile;    

    /**************************************************
            user's action
    **************************************************/
    ActionMask_t hand_in(Card_t kind,bool isNewDistributed,bool pervMing,bool isLastOne);
    Card_t       hand_out(unsigned int place);
	ActionMask_t judge_action_again();

    ACT_RES action(bool isNewDistributed,ActionId_t act,Card_t kind = CARD_UNKNOWN);
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
    PlayerDir_t   _dir;

    Logger    *_logger;
};

#endif
