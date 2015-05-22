
#include "./../utils/LogManager.h"

#include "CardCollection.h"
#include "Player.h"

Player::Player(PlayerDir_t dir) {
	_strategy = new StrategyPlayer(this);

    _dir = dir;
    _cards = NULL;
    _river = NULL;

    _isReady = false;

    memset(&_profile,0,sizeof(UserProfile_t));

    _logger = LOGGER_REGISTER("Player");
}

Player::Player(int id,PlayerDir_t dir) {//this is for default settings ( robot ) 
	_strategy = new StrategyPlayer(this);

    _dir = dir;
	_cards = NULL;
    _river = NULL;

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
unsigned char Player::init(int cards[],int len,int aim) {
    Card_t kinds[14];
    for(int i=0;i<len;i++) {
        kinds[i] = (Card_t)cards[i];
    }

    return init(kinds,len,aim);
}

unsigned char Player::init(Card_t cards[],int len,int aim) {
    _AIM     = aim;
    _aimDone = 0;
    _fan     = 0;
	_score   = 0;

    _cards = new CardInHand();
    _river = new CardList();

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
        return hand_in(cards[13],true,0,false,aQi,0,false);
    }
}

unsigned char Player::refresh(CardNode_t cards[],int len,HuFan_t fan) {
    _cards->clear();

    for(int i=0;i<len;i++) {
        CardNode_t *node = new CardNode_t;
        memcpy(node,&cards[i],sizeof(CardNode_t));
        
        _cards->push_back(node);
    }

    _fan = fan;

	return 0;
}

ActionMask_t Player::judge_action_again() {
    LOGGER_WRITE("%s",__FUNCTION__);

	unsigned char res = 0x0;

    if(_cards->has_shou_gang()) {/*BUG HERE??? always shougang*/
        res |= aSHOU_GANG;
    }

	if( _cards->collect_ming_info() ) {
		res |= aMING;
    }

	return res;
}

ActionMask_t Player::hand_in(Card_t newCard,bool isNewDistributed,bool tingStatus,bool isLastOne,ActionId_t lastActionWithGold,unsigned int continue_gang_times,bool isGangHua) {
	ActionMask_t actions = _cards->judge_action(newCard,isNewDistributed,isLastOne);

	if(_cards->can_hu(newCard)) {
		_score = _strategy->calc_score(newCard);

		if(isNewDistributed || (tingStatus==1||_score!=1)) {/* BUG only ming can hu dianpao ??? */
			actions |= aHU;
        }
	}

	if(isNewDistributed) {
		if(!_cards->IsMing && !isLastOne) {
			if(_cards->collect_ming_info())
				actions |= aMING;
		}
	}

    LOGGER_WRITE("NETWORK : %x %s actions %d: newCard %d,isNewDistributed %d,tingStatus %d,isLastOne %d",
        (int)this,__FUNCTION__,actions,newCard,isNewDistributed,tingStatus,isLastOne);
    
	return actions;
}

Card_t Player::hand_out(unsigned int place) {
	if( _cards->get_status(place) != sFREE ){
		return CARD_UNKNOWN;
	}

	Card_t kind = _cards->get_kind(place);
    _cards->delete_card(place,1);
    _river->push_back(kind);

    return kind;
}

ACT_RES Player::others_action(bool isNewDistributed,ActionId_t act,Card_t kind) {
    LOGGER_WRITE("%x %s : %d %d(isNewDistributed=%d)",this,__FUNCTION__,act,kind,isNewDistributed);
    _cards->others_perform(isNewDistributed,act,kind);
	return ar_DONE;
}

ACT_RES Player::action(bool isNewDistributed,ActionId_t act) {
    LOGGER_WRITE("%x %s : %d (isNewDistributed=%d)",this,__FUNCTION__,act,isNewDistributed);
    _cards->perform((ActionId_t)act,isNewDistributed);
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

