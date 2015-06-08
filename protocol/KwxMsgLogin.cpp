
#include "MsgFormats.h"
#include "CommonMsg.h"

#include "KwxMsgLogin.h"

int RequestLogin::Set() {
    _add_item( new Item(60,_userType) );
    _add_utf16_string(131,(const INT8U *)_account);
    
    return 0;
}


