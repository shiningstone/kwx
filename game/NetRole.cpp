
#include "NetRole.h"

NetRole::NetRole(PlayerType_t type) {
    _isReady = false;
    memset(&_profile,0,sizeof(UserProfile_t));
    _act = new NetRRound(type);

    _logger = LOGGER_REGISTER("NetRole");
}

NetRole::NetRole(int id,PlayerType_t type) {//this is for default settings ( robot ) 
    _isReady = false;
    memset(&_profile,0,sizeof(UserProfile_t));
    _act = new NetRRound(type);

    _logger = LOGGER_REGISTER("NetRole");
}

NetRole::~NetRole() {
    LOGGER_DEREGISTER(_logger);
}

void NetRole::set_robot_hu_target(ROBOT_TARGET par_target)
{

}
ROBOT_TARGET NetRole::get_robot_hu_target()
{
	return PI_HU_TARGET;
}

/**************************************************
        user's action
**************************************************/
unsigned char NetRole::init(int card_array[],int len,int aim) {
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

