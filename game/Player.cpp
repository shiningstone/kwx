
#include <string.h>

#include "./../utils/LogManager.h"

#include "CardCollection.h"
#include "Player.h"

Player::Player(PlayerDir_t dir) {
	_strategy = new StrategyPlayer(this);

    _dir = dir;
    _cards = new CardInHand();
    _river = new CardList();

    _isExist = false;
    _isReady = false;

    memset(&_profile,0,sizeof(UserProfile_t));

    _logger = LOGGER_REGISTER("Player");
}

Player::~Player() {
    if(_cards!=NULL) {
        delete _river;
        delete _cards;
    }

	delete _strategy;
    
    LOGGER_DEREGISTER(_logger);
}

/**************************************************
        user's action
**************************************************/
unsigned char Player::init(Card_t cards[],int len,int aim) {
    _AIM     = aim;
    _aimDone = 0;
    _fan     = 0;
	_score   = 0;

    _cards->clear();
    _cards->IsMing    = false;
	_cards->FreeStart = 0;
    
    _river->clear();
    
    for(int i=0;i<13;i++) {
        CardNode_t node;

		node.kind    = cards[i];
		node.status  = sFREE;
		node.canPlay = true;
        
		_cards->insert_card(node,1);
	}

    if(len<14) {/*NON-ZHUANG*/
        return a_JUMP;
    } else {
        return hand_in(cards[13],true,0,false);
    }
}

unsigned char Player::refresh(CardNode_t cards[],int len,HuFan_t fan) {
    _cards->refresh(cards,len);
    _fan = fan;

	return 0;
}

ActionMask_t Player::judge_action_again() {
	unsigned char res = 0x0;

    if(_cards->has_shou_gang()) {/*BUG HERE??? always shougang*/
        res |= aSHOU_GANG;
    }

	if( _strategy->scan_ming() ) {
		res |= aMING;
    }

	return res;
}

ActionMask_t Player::hand_in(Card_t newCard,bool isNewDistributed,bool pervMing,bool isLastOne) {
	ActionMask_t actions = _cards->judge_action(newCard,isNewDistributed,isLastOne);//peng / gang

	if(isNewDistributed) {
		if(!_cards->IsMing && !isLastOne) {
			if(_strategy->scan_ming()) {
				actions |= aMING;
             }
		}
	}

	auto curHuFlag=_cards->can_hu(newCard);
	if(curHuFlag!=NoHuForGame) {
		_score = _strategy->calc_score(newCard,curHuFlag);

		if(isNewDistributed || pervMing || _score>1) {
			actions |= aHU;
		}
	}

	//if(_cards->can_hu(newCard)) {
	//	_score = _strategy->calc_score(newCard);

	//	if(isNewDistributed || pervMing || _score>1) {
	//		actions |= aHU;
 //       }
	//}

	return actions;
}

Card_t Player::hand_out(unsigned int place) {
	if( _cards->get_status(place) != sFREE ){
		return CARD_UNKNOWN;
	}

	Card_t kind = _cards->get_kind(place);
    _cards->delete_card(place,1);
    _river->push_back(kind);

    LOGGER_WRITE("%s handout %s",DescPlayer(_dir),DescCard(kind));
    return kind;
}

ACT_RES Player::others_action(bool isNewDistributed,ActionId_t act,Card_t kind) {
    _cards->show();
    LOGGER_WRITE("%s act %s (%s)",DescPlayer(_dir),DescAct(act),DescCard(kind));

    _cards->others_perform(isNewDistributed,act,kind);
	return ar_DONE;
}

ACT_RES Player::action(bool isNewDistributed,ActionId_t act,Card_t kind) {
    _cards->show();
    LOGGER_WRITE("%s act %s (%s)",DescPlayer(_dir),DescAct(act),DescCard(kind));
    
    _cards->perform((ActionId_t)act,kind,isNewDistributed);
	return ar_DONE;
}

HuTarget_t Player::get_aim() const {
	return _aimDone;
}

HuTarget_t Player::get_hu_flag() const {
	return _fan;
}

long Player::get_score() const {
	return _score;
}

/**************************************************
        user's profile
**************************************************/
void Player::Set(const UserProfile_t *profile) {
    memcpy(&_profile,profile,sizeof(UserProfile_t));
}

Sex_t Player::GetSex() {
    if( !strcmp(_profile.sex,"Boy") ) {
        return BOY;
    } else {
        return GIRL;
    }
}

int Player::UpdateProperty(int change) {
    _profile.property += change;
    return _profile.property ;
}

