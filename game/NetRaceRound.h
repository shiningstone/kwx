#ifndef  _NET_RACE_ROUND_H_
#define  _NET_RACE_ROUND_H_

#include "Role.h"

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

#include "CardCollection.h"

class CardList;
class NetRRound
{
public:
	NetRRound(CardInHand *cardInHand,HuFan_t &fan,HuTarget_t aim);
    virtual ~NetRRound();

	unsigned char init(int card_array[],int len);

    CardInHand *_cardInHand;
private:
    Logger *_logger;

    const HuTarget_t _AIM;
    HuTarget_t       _aimDone;
	HuFan_t         &_fan;
    long             _score;

	int InsertPlaceForMG;
    
    long calcScore(Card_t kind,bool isNewDistributed,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua);//·ÖÊý¼ÆËã
    void taskCheck(unsigned int flag);
public:
    ActionMask_t hand_in(Card_t kind,bool isNewDistributed,bool tingStatus,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua); //0:sever, 1:player
    Card_t  hand_out(unsigned int place);
	ActionMask_t ActiontodoCheckAgain();

    ACT_RES action(bool isNewDistributed,ARRAY_ACTION act);
    ACT_RES others_action(bool isNewDistributed,ActionId_t act,Card_t kind);

    HuTarget_t get_aim() const;
	HuTarget_t get_hu_flag() const;
    long       get_score() const;
};

#endif // _RACE_ROUND_H_
