
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

int KwxMsg::Serialize(INT8U *outMsg) {
    int len = 0;

    len += _header->Serialize(outMsg);
    len += _body->Serialize(outMsg+len);
	_set_size(outMsg,len);

    return len;
}

int KwxMsg::Deserialize(const INT8U *inMsg) {
    int len = 0;

    if (memcmp(inMsg,Header::PCHC,3)) {
        return KWX_INVALID_PCHC;
    }

    len += _header->Deserialize(inMsg);
    len += _body->Deserialize(inMsg+len);

    return len;
}

/**********************************************************
	UpStream
***********************************************************/
void KwxMsg::_set_size(INT8U *buf,INT16U len) {
	if (_dir==UP_STREAM) {
		INT8U offset = UpHeader::SIZE;
		*((INT16U *)(buf+offset)) = _htons(len);
	}
}

int KwxMsg::SetRequestCode(RequestId_t code) {
	_header->_requestCode = (INT16U)code;
    return 0;
}

int KwxMsg::_add_item(Item *item) {
	_body->_items[_body->_itemNum] = item;
	_body->_itemNum++;
	return 0;
}

int KwxMsg::AddRoomPath(RoomPath_t code) {
	INT32U value = _htonl(code);
	return _add_item( new Item(RoomPath,4,(INT8U *)&value) );
}

int KwxMsg::AddRoomId(RoomId_t code) {
	INT32U value = _htonl(code);
	return _add_item( new Item(RoomId,4,(INT8U *)&value) );
}

int KwxMsg::AddTableId(TableId_t code) {
	INT32U value = _htonl(code);
	return _add_item( new Item(TableId,4,(INT8U *)&value) );
}

int KwxMsg::AddSeatId(INT8U code) {
	return _add_item( new Item(SeatId,code) );
}

int KwxMsg::AddAction(ActionId_t code) {
	INT32U value = _htonl(code);
	return _add_item( new Item(ActionId,4,(INT8U *)&value) );
}

int KwxMsg::AddCard(Card_t card) {
    INT8U code = card;
	return _add_item( new Item(CardList,1,(INT8U *)&code) );
}

/**********************************************************
	Interfaces
***********************************************************/
#include "EnvVariables.h"

int KwxMsg::SendAction(INT8U *buf,ActionId_t code,Card_t card) {
    SeatInfo *seat = SeatInfo::getInstance();

    SetRequestCode(REQ_GAME_SEND_ACTION);
    
    AddRoomPath(seat->_roomPath);
    AddRoomId(seat->_roomId);
    AddTableId(seat->_tableId);
    AddSeatId(seat->_seatId);
    AddAction(code);
    AddCard(card);

    LOGGER_WRITE("%s : %d\n",__FUNCTION__,code);

    return Serialize(buf);
}

RequestId_t KwxMsg::GetRequestCode() {
    return (RequestId_t)_header->_requestCode;
}

int KwxMsg::GetLevel() {
    DnHeader *header = static_cast<DnHeader *>(_header);
    return header->_level;
}

int KwxMsg::Construct(ActionResponse_t &waitInfo) {
    waitInfo.seat     = _body->_items[0]->_value;
    waitInfo.waitSeat = _body->_items[1]->_value;

    return 0;
}

