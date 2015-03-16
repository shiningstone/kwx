
#ifndef __CARD_HOLDER__
#define __CARD_HOLDER__

#include "DataBase.h"

class CardHolder {
public:
    CardHolder();
    void Set(const UserProfile_t *profile);

    bool          _isReady;
    UserProfile_t _profile;
};

#endif

