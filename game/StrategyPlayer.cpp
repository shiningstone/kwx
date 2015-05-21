
#include "CardCollection.h"
#include "Player.h"
#include "RoundManager.h"

#include "StrategyPlayer.h"

StrategyPlayer::StrategyPlayer(Player *aPlayer) {
    _employer = aPlayer;
}

void StrategyPlayer::assign(RoundManager *context) {
    _ctx = context;
}

long StrategyPlayer::calc_score(Card_t kind) {
    bool isNewDistributed = _ctx->_isNewDistributed;
    bool isLastOne        = (_ctx->_distributedNum==TOTAL_CARD_NUM);
    ActionId_t action     = _ctx->_lastActionWithGold;
    int continueGang      = _ctx->_continue_gang_times;
    bool isGangHua        = _ctx->_isGangHua;

    _employer->_cards->update_statistics(kind);
    _employer->_fan = _employer->_cards->statHuFanMask;
    
    if(isLastOne) {
		_employer->_fan |= RH_HAIDILAO;
    }

	if(isNewDistributed) {
		_employer->_fan |= RH_ZIMO;
	}

	if((continueGang!=0)
        &&(action==aMING_GANG||action==aAN_GANG||action==aQIANG_GANG)) {
		if(isNewDistributed&&action==aQIANG_GANG)
			_employer->_fan |= RH_QIANGGANG;
		else if(isNewDistributed&&isGangHua)
			_employer->_fan |= RH_GANGHUA;
		else if(!isNewDistributed)
			_employer->_fan |= RH_GANGPAO;
	}

	if(_employer->_AIM!=RA_NOTASK)
		check_task(_employer->_fan);
    
	return _employer->_cards->sum_up_score(_employer->_fan);
}

void StrategyPlayer::check_task(unsigned int flag) {
	if( !(flag&_employer->_AIM) ) {
		_employer->_aimDone=0;
    }
}

