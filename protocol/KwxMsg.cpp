
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
	return _add_item( new Item(ActionId,code) );
}

int KwxMsg::AddShowCard(CardType_t card) {
    return _add_item( new Item(CardKind,card) );
}

int KwxMsg::AddCards(CARD *cards,int num) {
    INT8U buf[MAX_HANDIN_NUM*4] = {0};

    for(int i=0;i<num;i++) {
        buf[i*4+1] = cards[i].kind;
        buf[i*4+2] = cards[i].status;
        buf[i*4+3] = cards[i].can_play;
    }

    return _add_item( new Item(CardList,num*4,buf) );
;
}

/**********************************************************
	Interfaces
***********************************************************/
#include "EnvVariables.h"

int KwxMsg::SetAction(ActionId_t code) {
    SeatInfo *seat = SeatInfo::getInstance();

    SetRequestCode(REQ_GAME_SEND_ACTION);
    AddRoomPath(seat->_roomPath);
    AddRoomId(seat->_roomId);
    AddTableId(seat->_tableId);
    AddSeatId(seat->_seatId);
    AddAction(code);

    return 0;
}

int KwxMsg::SetShowCard(CardType_t code) {
    SeatInfo *seat = SeatInfo::getInstance();

    SetRequestCode(REQ_GAME_SEND_SHOW_CARD);
    AddRoomPath(seat->_roomPath);
    AddRoomId(seat->_roomId);
    AddTableId(seat->_tableId);
    AddSeatId(seat->_seatId);
    AddShowCard(code);

    return 0;
}

int KwxMsg::SetRequestDistribute() {
    SeatInfo *seat = SeatInfo::getInstance();

    SetRequestCode(REQ_GAME_SEND_DIST);
    AddRoomPath(seat->_roomPath);
    AddRoomId(seat->_roomId);
    AddTableId(seat->_tableId);
    AddSeatId(seat->_seatId);

    return 0;
}

int KwxMsg::SetUpdateCardList(CARD *cards,int num) {
    SeatInfo *seat = SeatInfo::getInstance();

    SetRequestCode(REQ_GAME_SEND_UPDATELIST);
    AddRoomPath(seat->_roomPath);
    AddRoomId(seat->_roomId);
    AddTableId(seat->_tableId);
    AddSeatId(seat->_seatId);
    AddCards(cards,num);

    return 0;
}