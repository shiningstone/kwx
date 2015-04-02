
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

INT32U KwxDsMsg::GetItemValue(int idx) {
    switch(_body->_items[idx]->GetIdType()) {
        case PURE_ID:
            return INVALID;
        case ID_WITH_INT:
            return _body->_items[idx]->_value;
        case ID_WITH_BUF:
            {
                INT16U len = _body->_items[idx]->_bufLen;
                
                if(len==1) {
                    return _body->_items[idx]->_buf[0];
                } else if(len==2) {
                    return _ntohs( *(INT16U *)(_body->_items[idx]->_buf) );
                } else if(len==4) {
                    return _ntohl( *(INT32U *)(_body->_items[idx]->_buf) );
                } else {
                    return INVALID;
                }
            }
        default:
            return INVALID;
    }
}

int KwxDsMsg::Construct(ActionResponse_t &waitInfo) {
    waitInfo.seat     = GetItemValue(0);
    waitInfo.waitSeat = GetItemValue(1);
    return 0;
}

int KwxDsMsg::Construct(GameStartResponse_t &startInfo) {
    startInfo.score = GetItemValue(0);
    return 0;
}

int KwxDsMsg::Construct(GameStartNotif_t &startInfo) {
    startInfo.seat = GetItemValue(0);
    startInfo.score = GetItemValue(1);
    return 0;
}

int KwxDsMsg::_load(_MsgTingInfo_t &ting,const Item *item) {
    ting.num = (item->_bufLen)/4;

    const INT8U *p = item->_buf;
    for(int i=0;i<ting.num;i++) {
        ting.card[i].kind   = (Card_t)p[0+4*i];
        ting.card[i].remain = p[1+4*i];
        ting.card[i].fan    = _ntohs( *((INT16U *)(p+2+4*i)) );
    }

	return 0;
}

int KwxDsMsg::Construct(HandoutResponse_t &handoutResponse) {
    handoutResponse.status = (Status_t)GetItemValue(0);
    _load(handoutResponse.ting,_body->_items[1]);
    return 0;
}

int KwxDsMsg::Construct(HandoutNotif_t &handoutInfo) {
    handoutInfo.seat    = (Status_t)GetItemValue(0);
    handoutInfo.handout = (Card_t)GetItemValue(1);
    _load(handoutInfo.ting,_body->_items[2]);
    return 0;
}

int KwxDsMsg::Construct(ActionNotif_t &action) {
    action.seat    = GetItemValue(0);
    action.isFromServer = (GetItemValue(1)==0)?true:false;
    action.next    = GetItemValue(2);
    action.action  = (ActionId_t)GetItemValue(3);
    action.cardNum = _body->_items[4]->_bufLen;
    memcpy(action.card,_body->_items[4]->_buf,action.cardNum);
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

#include "EnvVariables.h"
int KwxUsMsg::SetSeatInfo() {
    SeatInfo *seat = SeatInfo::getInstance();

    INT32U roomPath = _htonl(seat->_roomPath);
    INT32U roomId   = _htonl(seat->_roomId);
    INT32U tableId  = _htonl(seat->_tableId);

    _add_item( new Item((Item_t)131,4,(INT8U *)&roomPath) );
    _add_item( new Item((Item_t)132,4,(INT8U *)&roomId) );
    _add_item( new Item((Item_t)133,4,(INT8U *)&tableId) );
    _add_item( new Item((Item_t)60,   seat->_seatId) );

    return 0;
}

/**********************************************************
	Interfaces
***********************************************************/
int RequestSendAction::Set(ActionId_t action,Card_t card) {
    SetRequestCode(REQ_GAME_SEND_ACTION);
    
    INT32U actionId = _htonl(action);
    INT8U  cardKind = card;

    SetSeatInfo();
    _add_item( new Item((Item_t)134,4,(INT8U *)&actionId) );
    _add_item( new Item((Item_t)135,1,(INT8U *)&cardKind) );

    return 0;
}

int RequestGameStart::Set() {
    SetRequestCode(REQ_GAME_SEND_START);
    SetSeatInfo();

    return 0;
}

int RequestHandout::Set(Card_t card) {
    SetRequestCode(REQ_GAME_SEND_SHOWCARD);
    SetSeatInfo();
    _add_item( new Item((Item_t)65,card) );

    return 0;
}


