
#include "NetRole.h"

NetRole::NetRole() {
    _isReady = false;

    memset(&_profile,0,sizeof(UserProfile_t));

    _cardsInHand = new CardInHand();
    _river       = new CardList();
    _act         = new NetRRound(_cardsInHand);

    _logger = LOGGER_REGISTER("NetRole");
}

NetRole::NetRole(int id) {//this is for default settings ( robot ) 
    _isReady = false;

    memset(&_profile,0,sizeof(UserProfile_t));

    _cardsInHand = new CardInHand();
    _river       = new CardList();
    _act         = new NetRRound(_cardsInHand);

    _logger = LOGGER_REGISTER("NetRole");
}

NetRole::~NetRole() {
    delete _act;
    delete _cardsInHand;
    delete _river;
    
    LOGGER_DEREGISTER(_logger);
}

/**************************************************
        user's action
**************************************************/
unsigned char NetRole::init(int card_array[],int len,int aim) {
    _cardsInHand->(card_array,len);
	return _act->init(card_array,len,aim);
}

NetRRound* NetRole::get_parter() {
	return _act;
}

/**************************************************
        user's profile
**************************************************/
void NetRole::Set(const UserProfile_t *profile) {
    memcpy(&_profile,profile,sizeof(UserProfile_t));
}

Sex_t NetRole::GetSex() {
    if( !strcmp(_profile.sex,"Boy") ) {
        return BOY;
    } else {
        return GIRL;
    }
}

int NetRole::UpdateProperty(int change) {
    _profile.property += change;
    return _profile.property ;
}

