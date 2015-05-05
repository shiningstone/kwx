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
    unsigned char hand_in(CARD_KIND kind,bool isNewDistributed,bool tingStatus,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua); //0:sever, 1:player
    CARD_KIND hand_out(unsigned int place);
    ACT_RES action(bool isNewDistributed,ARRAY_ACTION act);
    ACT_RES others_action(bool isNewDistributed,ARRAY_ACTION act,Card_t kind);
	unsigned char ActiontodoCheckAgain();

    HuTarget_t get_aim() const;
	HuTarget_t get_hu_flag() const;
    long       get_score() const;
};

#endif // _RACE_ROUND_H_
