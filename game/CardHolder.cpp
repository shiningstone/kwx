
#include "CardHolder.h"

CardHolder::CardHolder() {
    _isReady = false;
    memset(&_profile,0,sizeof(UserProfile_t));
}

void CardHolder::Set(const UserProfile_t *profile) {
    memcpy(&_profile,profile,sizeof(UserProfile_t));
}

Sex_t CardHolder::GetSex() {
    if( !strcmp(_profile.sex,"Boy") ) {
        return BOY;
    } else {
        return GIRL;
    }
}

