
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
	if (_dir==UP_STREAM) {
		INT8U offset = UpHeader::SIZE;
		*((INT16U *)(buf+offset)) = _htons(len);
	}
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

int KwxUsMsg::AddRoomPath(RoomPath_t code) {
	INT32U value = _htonl(code);
	return _add_item( new Item(RoomPath,4,(INT8U *)&value) );
}

int KwxUsMsg::AddRoomId(RoomId_t code) {
	INT32U value = _htonl(code);
	return _add_item( new Item(RoomId,4,(INT8U *)&value) );
}

int KwxUsMsg::AddTableId(TableId_t code) {
	INT32U value = _htonl(code);
	return _add_item( new Item(TableId,4,(INT8U *)&value) );
}

int KwxUsMsg::AddSeatId(INT8U code) {
	return _add_item( new Item(SeatId,code) );
}

int KwxUsMsg::AddAction(ActionId_t code) {
	INT32U value = _htonl(code);
	return _add_item( new Item(ActionId,4,(INT8U *)&value) );
}

int KwxUsMsg::AddCard(Card_t card) {
    INT8U code = card;
	return _add_item( new Item(CardList,1,(INT8U *)&code) );
}

/**********************************************************
	Interfaces
***********************************************************/
#include "EnvVariables.h"

int KwxUsMsg::SendAction(INT8U *buf,ActionId_t code,Card_t card) {
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

