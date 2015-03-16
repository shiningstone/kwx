
#ifndef __CARD_HOLDER__
#define __CARD_HOLDER__

#include "./../utils/BasicType.h"
#include "DataBase.h"

class CardHolder {
public:
    CardHolder();
    void Set(const UserProfile_t *profile);
    Sex_t GetSex();

    bool          _isReady;
    UserProfile_t _profile;
};

#endif

