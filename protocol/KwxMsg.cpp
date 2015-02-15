
#include <string.h>

#include "MsgBasic.h"
#include "MsgFormats.h"
#include "KwxMsg.h"

KwxMsg::KwxMsg(int dir)
:_dir(dir) {
	if (_dir==UP_STREAM) {
        _header = new UpHeader();
    } else {
        _header = new DnHeader();
    }

    _body = new MsgBody();
}

KwxMsg::~KwxMsg() {
    delete _header;
    delete _body;
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

int KwxMsg::AddRoomPath(RoomPath_t code) {
	INT32U value = _htonl(code);

	_body->_items[_body->_itemNum] = new Item(131,4,(INT8U *)&value);
	_body->_itemNum++;

	return 0;
}

int KwxMsg::AddRoomId(RoomId_t code) {
	INT32U value = _htonl(code);

	_body->_items[_body->_itemNum] = new Item(132,4,(INT8U *)&value);
	_body->_itemNum++;

    return 0;
}

int KwxMsg::AddTableId(TableId_t code) {
	INT32U value = _htonl(code);

	_body->_items[_body->_itemNum] = new Item(133,4,(INT8U *)&value);
	_body->_itemNum++;

    return 0;
}

int KwxMsg::AddSeatId(INT32U code) {
	_body->_items[_body->_itemNum] = new Item(60,code);
	_body->_itemNum++;

	return 0;
}

int KwxMsg::AddAction(ActionId_t code) {
	_body->_items[_body->_itemNum] = new Item(67,code);
	_body->_itemNum++;

    return 0;
}

