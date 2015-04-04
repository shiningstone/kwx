
#include <string.h>

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"
#include "./../network/NetMessenger.h"

#include "MsgFormats.h"
#include "CommonMsg.h"
#include "DsInstruction.h"

Logger *CommonMsg::_logger = 0;
SeatInfo *CommonMsg::_seatInfo = 0;

CommonMsg::CommonMsg(int dir)
:_dir(dir) {
	if (_dir==UP_STREAM) {
        _header = new UpHeader();
    } else {
        _header = new DnHeader();
    }
    _body = new MsgBody();

    _seatInfo = SeatInfo::getInstance();
    _logger = LOGGER_REGISTER("CommonMsg");
}

CommonMsg::~CommonMsg() {
    delete _header;
    delete _body;
    LOGGER_DEREGISTER(_logger);
}

/**********************************************************
	Downstream
***********************************************************/
/*******************************************************
        ����ģʽ 
*******************************************************/
DsMsg * DsMsg::_instance   = 0;

DsMsg * DsMsg::getInstance() {
    if(_instance==0) {
        _instance = new DsMsg();
    }

    return _instance;
}

void DsMsg::destroyInstance() {
    delete _instance;
    _instance = 0;
}

DsMsg::DsMsg()
:CommonMsg(DOWN_STREAM){
}

int DsMsg::Dispatch(const INT8U *inMsg,int inLen) {
    Deserialize(inMsg);

    DsInstruction *instruction = _GenerateInstruction();
    if((int)instruction!=KWX_INVALID_PCHC) {
        instruction->Dispatch();
        delete instruction;
    } else {
        LOGGER_WRITE("unknown instruction\n");
        LOGGER_WRITE_ARRAY((char *)inMsg,inLen);
        return INVALID;
    }

	return 0;
}

int DsMsg::Deserialize(const INT8U *inMsg) {
    int len = 0;

    if (memcmp(inMsg,Header::PCHC,3)) {
        return KWX_INVALID_PCHC;
    }

    len += _header->Deserialize(inMsg);
    len += _body->Deserialize(inMsg+len);

    return len;
}

DsInstruction *DsMsg::_GenerateInstruction() {
    switch(GetRequestCode()) {
        case REQ_GAME_SEND_START:
            return new GameStartResponse();
            
        case REQ_GAME_RECV_START:
            return new GameStartNotif();

        case REQ_GAME_SEND_SHOWCARD:
            return new HandoutResponse();

        case REQ_GAME_RECV_SHOWCARD:
            return new HandoutNotif();

        case REQ_GAME_SEND_ACTION:
            return new ActionResponse();

        case REQ_GAME_RECV_ACTION:
            return new ActionNotif();

        case REQ_GAME_DIST_CARD:
            return new DistCardInfo();

        case REQ_GAME_DIST_CARD_TOOTHER:
            return new DistCardNotif();

        case REQ_GAME_DIST_BEGINCARDS:
            return new FirstDistZhuang();

        case REQ_GAME_DIST_BEGINCARDS_OTHER:
            return new FirstDistNonZhuang();

        case REQ_GAME_DIST_REMIND:
            return new RemindInfo();

        case REQ_GAME_SEND_CALSCORE:
            return new ScoreNotif();

        case REQ_GAME_DIST_DECISION:
            return new DecisionNotif();

        case REQ_GAME_GET_TINGINFO:
            return new TingInfoResponse();

        default:
            return (DsInstruction *)KWX_INVALID_PCHC;
    }
}

RequestId_t DsMsg::GetRequestCode() const {
    return (RequestId_t)_header->_requestCode;
}

int DsMsg::GetLevel() const {
    DnHeader *header = static_cast<DnHeader *>(_header);
    return header->_level;
}

INT32U DsMsg::GetItemValue(int idx) const {
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

INT16U DsMsg::GetItemBufLen(int idx) const {
    if(_body->_items[idx]->GetIdType()==ID_WITH_BUF) {
        return _body->_items[idx]->_bufLen;
    } else {
        return INVALID;
    }
}

/**********************************************************
	UpStream
***********************************************************/
UsMsg::UsMsg()
:CommonMsg(UP_STREAM) {
}

int UsMsg::Serialize(INT8U *outMsg) {
    int len = 0;

    len += _header->Serialize(outMsg);
    len += _body->Serialize(outMsg+len);
	_set_size(outMsg,len);

    return len;
}

void UsMsg::_set_size(INT8U *buf,INT16U len) {
	INT8U offset = UpHeader::SIZE;
	*((INT16U *)(buf+offset)) = _htons(len);
}

int UsMsg::SetRequestCode(RequestId_t code) {
	_header->_requestCode = (INT16U)code;
    return 0;
}

int UsMsg::_add_item(Item *item) {
	_body->_items[_body->_itemNum] = item;
	_body->_itemNum++;
	return 0;
}

#include "KwxMsgEnv.h"
int UsMsg::AddSeatInfo() {
    INT32U roomPath = _htonl(_seatInfo->_roomPath);
    INT32U roomId   = _htonl(_seatInfo->_roomId);
    INT32U tableId  = _htonl(_seatInfo->_tableId);

    _add_item( new Item((Item_t)131,4,(INT8U *)&roomPath) );
    _add_item( new Item((Item_t)132,4,(INT8U *)&roomId) );
    _add_item( new Item((Item_t)133,4,(INT8U *)&tableId) );
    _add_item( new Item((Item_t)60,   _seatInfo->_seatId) );

    return 0;
}
