
#include "./../utils/UtilBasic.h"

#include "Ai.h"
#include "NetRaceRound.h"
#include "CardCollection.h"

USING_NS_CC;

NetRRound::NetRRound(CardInHand *cardInHand,HuFan_t &fan,HuTarget_t aim)
:_fan(fan),_AIM(aim) {
    _cardInHand = cardInHand;
	_fan=0;

    _logger = LOGGER_REGISTER("RaceRound");
}

NetRRound::~NetRRound() {
    LOGGER_DEREGISTER(_logger);
}

long NetRRound::calcScore(Card_t kind,bool isNewDistributed,bool isLastOne,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua) {
    _cardInHand->update_statistics(kind);
    _fan = _cardInHand->statHuFanMask;
    
    if(isLastOne) {
		_fan |= RH_HAIDILAO;
    }

	if(isNewDistributed) {
		_fan |= RH_ZIMO;
	}

	if((continue_gang_times!=0)
        &&(last_action_WithGold==a_MING_GANG
            ||last_action_WithGold==a_AN_GANG
            ||last_action_WithGold==a_QIANG_GANG)) {
		if(isNewDistributed&&last_action_WithGold==a_QIANG_GANG)
			_fan |= RH_QIANGGANG;
		else if(isNewDistributed&&isGangHua)
			_fan |= RH_GANGHUA;
		else if(!isNewDistributed)
			_fan |= RH_GANGPAO;
	}

	if(_AIM!=RA_NOTASK)
		taskCheck(_fan);
    
	return Ai::getInstance()->sum_up_score(_fan);
}

void NetRRound::taskCheck(unsigned int flag) {
	if( !(flag&_AIM) ) {
		_aimDone=0;
    }
}

unsigned char NetRRound::init(int card_array[],int len)
{
	_score=0;

    _cardInHand->clear();

	for(int i=0;i<13;i++) {
        CardNode_t node;

		node.kind    = (Card_t)(card_array[i]);
		node.status  = sFREE;
		node.canPlay = true;
        
		_cardInHand->insert_card(node,1);
	}

    if(len<14) {/*NON-ZHUANG*/
        return a_JUMP;
    } else {
        return hand_in((Card_t)(card_array[13]),0,0,false,a_JUMP,0,false);
    }
}

ActionMask_t NetRRound::ActiontodoCheckAgain() {
    LOGGER_WRITE("%s",__FUNCTION__);

	unsigned char res = 0x0;

    if(_cardInHand->has_shou_gang()) {/*BUG HERE??? always shougang*/
        res |= aSHOU_GANG;
    }

	if( _cardInHand->collect_ming_info() ) {
		res |= aMING;
    }

	return res;
}

ActionMask_t NetRRound::hand_in(Card_t newCard,bool isNewDistributed,bool tingStatus,bool isLastOne,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua) {
    _cardInHand->push_back((Card_t)newCard);

	ActionMask_t action = _cardInHand->judge_action(isNewDistributed,isLastOne);

	if(_cardInHand->can_hu((Card_t)newCard)) {
		_score = calcScore((Card_t)newCard,isNewDistributed,isLastOne,last_action_WithGold,continue_gang_times,isGangHua);

		if(isNewDistributed || (tingStatus==1||_score!=1)) {/* BUG only ming can hu dianpao ??? */
			action |= aHU;
        }
	}

    /* BUG ??? ming can implemented after some action take place, such as PENG/GANG */
    /*     !!! maybe done by ActiontodoCheckAgain                                   */
	if(isNewDistributed) {
		if(!_cardInHand->IsMing && !isLastOne) {
			if(_cardInHand->collect_ming_info())
				action |= aMING;
		}
	}

    LOGGER_WRITE("NETWORK : %x %s action %d: newCard %d,isNewDistributed %d,tingStatus %d,isLastOne %d",
        (int)this,__FUNCTION__,action,newCard,isNewDistributed,tingStatus,isLastOne);
    
	return action;
}

Card_t NetRRound::hand_out(unsigned int place) {
	if( _cardInHand->get_status(place) != sFREE ){
		return CARD_UNKNOWN;
	}

	Card_t kind = _cardInHand->get_kind(place);
    _cardInHand->delete_card(place,1);

    return kind;
}

ACT_RES NetRRound::others_action(bool isNewDistributed,ActionId_t act,Card_t kind) {
    LOGGER_WRITE("%x %s : %d (isNewDistributed=%d)",this,__FUNCTION__,act,isNewDistributed);
    _cardInHand->others_perform(isNewDistributed,act,kind);
	return ar_DONE;
}

ACT_RES NetRRound::action(bool isNewDistributed,ARRAY_ACTION act) {
    LOGGER_WRITE("%x %s : %d (isNewDistributed=%d)",this,__FUNCTION__,act,isNewDistributed);
    _cardInHand->perform((ActionId_t)act,isNewDistributed);
	return ar_DONE;
}

HuTarget_t NetRRound::get_aim() const {
	return _aimDone;
}

HuTarget_t NetRRound::get_hu_flag() const {
	return _fan;
}

long NetRRound::get_score() const {
	return _score;
}

