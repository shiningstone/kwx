
#include "MsgFormats.h"
#include "CommonMsg.h"
#include "DsInstruction.h"

#include "KwxMsgLogin.h"

int RequestLogin::Set() {
    _add_item( new Item(60,_userType) );
    
    _add_utf16_string(131,(const INT8U *)_account);
    _add_utf16_string(132,(const INT8U *)_mac);
    _add_utf16_string(133,(const INT8U *)_imsi);
    _add_utf16_string(134,(const INT8U *)_resolution);
    _add_utf16_string(135,(const INT8U *)_product);
    _add_utf16_string(136,(const INT8U *)_osVer);
    _add_utf16_string(137,(const INT8U *)_session);

    return 0;
}

