
#include "NetRole.h"

NetRole::NetRole() {
    _isReady = false;
    
    memset(&_profile,0,sizeof(UserProfile_t));
    _cards = new CardInHand();
    _river = new CardList();
    _act   = new NetRRound(_cards);

    _logger = LOGGER_REGISTER("NetRole");
}

NetRole::NetRole(int id) {//this is for default settings ( robot ) 
    _isReady = false;
    memset(&_profile,0,sizeof(UserProfile_t));
    _cards = new CardInHand();
    _river = new CardList();
    _act   = new NetRRound(_cards);

    _logger = LOGGER_REGISTER("NetRole");
}

NetRole::~NetRole() {
    delete _act;
    delete _river;
    
    LOGGER_DEREGISTER(_logger);
}

/**************************************************
        user's action
**************************************************/
unsigned char NetRole::init(int card_array[],int len,int aim) {
    _aim = aim;
	return _act->init(card_array,len,aim);
}

unsigned char NetRole::init(Card_t cards[],int len,int aim) {
    int card_array[14];
    for(int i=0;i<len;i++) {
        card_array[i] = cards[i]*4;
    }
    
	return init(card_array,len,aim);
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

