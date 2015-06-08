
#include "MsgFormats.h"
#include "CommonMsg.h"
#include "DsInstruction.h"

#include "KwxMsgLogin.h"

int RequestLogin::Set() {
    SetRequestCode(REQ_LOGIN);

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

int LoginResponse::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    INT8U status = msg.GetItemValue(0);
    if(status!=0) {
        return status;
    }

    _userType          = (UserType_t)msg.GetItemValue(1);
    _userActivated     = msg.GetItemValue(2);
    _reconnectRequired = msg.GetItemValue(3);
    _userId            = msg.GetItemValue(4);
    _key               = msg.GetItemValue(5);
    _roomServerId      = msg.GetItemValue(6);
    Utf16ToUtf8((const Utf16 *)msg._body->_items[7]->_buf,(Utf8 *)_roomServerIp);
    _roomServerPort    = msg.GetItemValue(8);

    return 0;
}

int RequestReconnect::Set() {
    SetRequestCode(REQ_GAME_SEND_RECONNECT);

    INT32U value = _htons(_key);
    _add_item( new Item(130,4,(INT8U *)&value) );
    AddSeatInfo();

    return 0;
}

int ReconnectResponse::Construct(const DsMsg &msg) {
    return 0;
}

int ReconnectNotif::Construct(const DsMsg &msg) {
    _dir   = _GetPlayer(msg.GetItemValue(0));
    return 0;
}

int RequestLeave::Set() {
    SetRequestCode(REQ_GAME_SEND_LEAVE_ROOM);
    AddSeatInfo();
    
    return 0;
}

int LeaveResponse::Construct(const DsMsg &msg) {
    _score = msg.GetItemValue(0);
    return 0;
}

int LeaveNotif::Construct(const DsMsg &msg) {
    _dir    = _GetPlayer(msg.GetItemValue(0));
    _status = (LeaveStatus_t)msg.GetItemValue(1);
    return 0;
}

int LoginConflictNotif::Construct(const DsMsg &msg) {
    Utf16ToUtf8((const Utf16 *)msg._body->_items[0]->_buf,(Utf8 *)_info);
    return 0;
}

int RequestXiaPiao::Set(INT32U score) {
    SetRequestCode(REQ_GAME_SEND_XIA_PIAO);
    AddSeatInfo();

    INT32U value = _htonl(score);
    _add_item( new Item(134,4,(INT8U *)&value) );
    return 0;
}

int XiaPiaoResponse::Construct(const DsMsg &msg) {
    _score = msg.GetItemValue(0);
    return 0;
}

int XiaPiaoNotif::Construct(const DsMsg &msg) {
    _dir   = _GetPlayer(msg.GetItemValue(0));
    _score = msg.GetItemValue(1);
    return 0;
}

int RequestTalk::Set(const INT8U *content, TalkType_t type, PlayerDir_t dir) {
    SetRequestCode(REQ_GAME_SEND_MSG);
    AddSeatInfo();

    _add_item( new Item(61,_seatInfo->GetSeatId(_sendTo)) );
    _add_item( new Item(62,_type) );
    _add_utf16_string(134,(const INT8U *)_content);
    
    return 0;
}

int TalkResponse::Construct(const DsMsg &msg) {
    _status = msg.GetItemValue(0);
    return 0;
}

int TalkNotif::Construct(const DsMsg &msg) {
    _type = (TalkType_t)msg.GetItemValue(0);
    Utf16ToUtf8((const Utf16 *)msg._body->_items[1]->_buf,(Utf8 *)_content);
        
    return 0;
}

