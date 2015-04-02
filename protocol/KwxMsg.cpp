
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

int KwxDsMsg::Construct(HandoutResponse_t &handoutResponse) {
    handoutResponse.status = (Status_t)GetItemValue(0);
    handoutResponse.ting.cardNum = _body->_items[1]->_bufLen;
    _load(handoutResponse.ting,_body->_items[1]->_buf);
    return 0;
}

int KwxDsMsg::Construct(HandoutNotif_t &handoutInfo) {
    handoutInfo.seat    = (Status_t)GetItemValue(0);
    handoutInfo.kind = (Card_t)GetItemValue(1);
    handoutInfo.ting.cardNum = _body->_items[2]->_bufLen;
    _load(handoutInfo.ting,_body->_items[2]->_buf);
    return 0;
}

int KwxDsMsg::Construct(ActionNotif_t &action) {
    action.seat    = GetItemValue(0);
    action.isFromServer = (GetItemValue(1)==0)?true:false;
    action.next    = GetItemValue(2);
    action.action  = (ActionId_t)GetItemValue(3);
    
    _load(action.card, action.cardNum, _body->_items[4]);

    return 0;
}

int KwxDsMsg::Construct(DistCardInfo_t &dist) {
    dist.seat      = GetItemValue(0);
    dist.timer     = GetItemValue(1);
    dist.remain    = GetItemValue(2);
    dist.kind      = (Card_t)GetItemValue(3);

    _load(dist.remind,4);
    
    return 0;
}

int KwxDsMsg::Construct(RemindInfo_t &remind) {
    remind.seat   = GetItemValue(0);
    remind.timer  = GetItemValue(1);

    _load(remind.remind,2);

    remind.wait = GetItemValue(6);
    
    return 0;
}

int KwxDsMsg::Construct(DistCardNotif_t &dist) {
    dist.seat      = GetItemValue(0);
    dist.remain    = GetItemValue(1);
    dist.timer     = GetItemValue(2);
    dist.kind      = (Card_t)GetItemValue(3);
    
    return 0;
}

int KwxDsMsg::Construct(ScoreNotif_t &score) {
    score.seat[0] = _body->_items[0]->_buf[0];
    score.seat[1] = _body->_items[0]->_buf[1];
    score.seat[2] = _body->_items[0]->_buf[2];

    score.val[0] = _ntohl( *((INT32U *)(_body->_items[1]->_buf)) );
    score.val[1] = _ntohl( *((INT32U *)(_body->_items[1]->_buf+4)) );
    score.val[2] = _ntohl( *((INT32U *)(_body->_items[1]->_buf+8)) );
    
    return 0;
}

int KwxDsMsg::_load(Card_t *cards,INT8U &num,const Item *item) {
    num = (INT8U)item->_bufLen;
    
    for(int i=0;i<num;i++) {
        cards[i] = (Card_t)item->_buf[i];
    }

    return 0;
}

int KwxDsMsg::_load(_MsgTingInfo_t &ting,const INT8U *inMsg) {
    const INT8U *p = inMsg;

    ting.cards = new TingItem_t[ting.cardNum];
    
    int i = 0;
    int num = 0;
    while(num<ting.cardNum) {
        ting.cards[i].kind   = (Card_t)p[0+4*i];
        ting.cards[i].remain = p[1+4*i];
        ting.cards[i].fan    = _ntohs( *((INT16U *)(p+2+4*i)) );

        num += ting.cards[i].remain;
        i++;
    }

	return i*4;
}

int KwxDsMsg::_load(_MingInfo_t &ming,const Item *item) {
    if(_ntohl(*(INT32U *)(item->_buf))==0xffffffff) {
        ming.choiceNum = 0;
        return 0;
    }

    const int   len = item->_bufLen;
    const INT8U *p = item->_buf;
    _MingChoice_t choices[13];

    int idx = 0;
    int i   = 0;
    while(i<len) {
        i += 2;

        choices[idx].kind = (Card_t)(*(p+i));
        choices[idx].ting.cardNum = *(p+i+1);
        i += 2;

        i += _load(choices[idx].ting,p+i);

        idx++;
    }

    ming.choiceNum = idx;
    ming.handouts  = new _MingChoice_t[idx];
    memcpy(ming.handouts,choices,sizeof(_MingChoice_t)*idx);

	return 0;
}

int KwxDsMsg::_load(_Reminds_t &remind,int itemIdx) {
    remind.actions = (ActionId_t)GetItemValue(itemIdx);

    _load(remind.gangCard, remind.gangKindNum, _body->_items[itemIdx+1]);
    _load(remind.kouCard, remind.kouKindNum, _body->_items[itemIdx+2]);
    _load(remind.ming,_body->_items[itemIdx+3]);

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


