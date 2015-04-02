
#include <string.h>

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"
#include "./../network/NetMessenger.h"

#include "MsgFormats.h"
#include "KwxMsg.h"

Logger *KwxMsg::_logger = 0;

KwxMsg::KwxMsg(int dir)
:_dir(dir) {
	if (_dir==UP_STREAM) {
        _header = new UpHeader();
    } else {
        _header = new DnHeader();
    }
    _body = new MsgBody();
    _logger = LOGGER_REGISTER("KwxMsg");
}

KwxMsg::~KwxMsg() {
    delete _header;
    delete _body;
    LOGGER_DEREGISTER(_logger);
}


/**********************************************************
	Downstream
***********************************************************/
KwxDsMsg::KwxDsMsg()
:KwxMsg(DOWN_STREAM){
}

int KwxDsMsg::Deserialize(const INT8U *inMsg) {
    int len = 0;

    if (memcmp(inMsg,Header::PCHC,3)) {
        return KWX_INVALID_PCHC;
    }

    len += _header->Deserialize(inMsg);
    len += _body->Deserialize(inMsg+len);

    return len;
}

RequestId_t KwxDsMsg::GetRequestCode() {
    return (RequestId_t)_header->_requestCode;
}

int KwxDsMsg::GetLevel() {
    DnHeader *header = static_cast<DnHeader *>(_header);
    return header->_level;
}

int KwxDsMsg::Construct(ActionResponse_t &waitInfo) {
    waitInfo.seat     = _body->_items[0]->_value;
    waitInfo.waitSeat = _body->_items[1]->_value;

    return 0;
}

/**********************************************************
	UpStream
***********************************************************/
KwxUsMsg::KwxUsMsg()
:KwxMsg(UP_STREAM) {
}

int KwxUsMsg::Serialize(INT8U *outMsg) {
    int len = 0;

    len += _header->Serialize(outMsg);
    len += _body->Serialize(outMsg+len);
	_set_size(outMsg,len);

    return len;
}

void KwxUsMsg::_set_size(INT8U *buf,INT16U len) {
	INT8U offset = UpHeader::SIZE;
	*((INT16U *)(buf+offset)) = _htons(len);
}

int KwxUsMsg::SetRequestCode(RequestId_t code) {
	_header->_requestCode = (INT16U)code;
    return 0;
}

int KwxUsMsg::_add_item(Item *item) {
	_body->_items[_body->_itemNum] = item;
	_body->_itemNum++;
	return 0;
}

/**********************************************************
	Interfaces
***********************************************************/
#include "EnvVariables.h"

int RequestSendAction::Set(ActionId_t action,Card_t card) {
    SeatInfo *seat = SeatInfo::getInstance();

    SetRequestCode(REQ_GAME_SEND_ACTION);

    INT32U roomPath = _htonl(seat->_roomPath);
    INT32U roomId   = _htonl(seat->_roomId);
    INT32U tableId  = _htonl(seat->_tableId);
    INT32U actionId = _htonl(action);
    INT8U  cardKind = card;

    _add_item( new Item(131,4,(INT8U *)&roomPath) );
    _add_item( new Item(132,4,(INT8U *)&roomId) );
    _add_item( new Item(133,4,(INT8U *)&tableId) );
    _add_item( new Item(60,   seat->_seatId) );
    _add_item( new Item(134,4,(INT8U *)&actionId) );
    _add_item( new Item(135,1,(INT8U *)&cardKind) );

    return 0;
}

int RequestGameStart::Set() {
    SeatInfo *seat = SeatInfo::getInstance();

    SetRequestCode(REQ_GAME_SEND_START);

    INT32U roomPath = _htonl(seat->_roomPath);
    INT32U roomId   = _htonl(seat->_roomId);
    INT32U tableId  = _htonl(seat->_tableId);

    _add_item( new Item(131,4,(INT8U *)&roomPath) );
    _add_item( new Item(132,4,(INT8U *)&roomId) );
    _add_item( new Item(133,4,(INT8U *)&tableId) );
    _add_item( new Item(60,   seat->_seatId) );

    return 0;
}
