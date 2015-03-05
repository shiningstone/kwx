
#include "NetRole.h"

void NetRole::set_sex(std::string sex) {
    _sex = (sex=="Boy")? BOY : GIRL;
}

Sex_t NetRole::get_sex() {
    return _sex;
}

