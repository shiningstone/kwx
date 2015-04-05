
#include <string.h>

#include "MsgFormats.h"
#include "DsInstruction.h"
#include "DsMsgParser.h"
#include "KwxMsgEnv.h"

#ifndef __UNIT_TEST__
#include "./../game/RoundManager.h"
#include "./../game/NetRoundManager.h"
#endif

SeatInfo *DsInstruction::_seatInfo = 0;
Logger *DsInstruction::_logger = 0;

DsInstruction::DsInstruction() {
    _seatInfo = SeatInfo::getInstance();
    
    #ifndef __UNIT_TEST__
    _roundManager = NetRoundManager::getInstance();
    #endif
    _logger = LOGGER_REGISTER("DsInstruction");
}

int DsInstruction::Construct(const DsMsg &msg) {
    request = msg.GetRequestCode();
    return 0;
}

int DsInstruction::Dispatch() {
    #ifndef __UNIT_TEST__
    _roundManager->RecvMsg(this);
    #endif
    return 0;
}

PlayerDir_t DsInstruction::_GetPlayer(INT8U seat) {
    PlayerDir_t dir = _seatInfo->GetPlayer(seat);
    if( dir<LEFT || dir>RIGHT ) {
        return INVALID_DIR;
    } else {
        return _seatInfo->GetPlayer(seat);
    }
}
/*****************************************************
    Concrete classes
*****************************************************/
int GameStartResponse::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    score   = msg.GetItemValue(0);
    return 0;
}

int GameStartNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat  = _GetPlayer(msg.GetItemValue(0));
    score = msg.GetItemValue(1);
    return 0;
}

int FirstDistZhuang::Construct(const DsMsg &msg) {
    seat      = msg.GetItemValue(0);
    remain    = msg.GetItemValue(1);
    timer     = msg.GetItemValue(2);
    memcpy(cards, msg._body->_items[3]->_buf, 14);

    DsMsgParser::_load(remind, msg, 4);
    return 0;
}

FirstDistZhuang::~FirstDistZhuang() {
    DsMsgParser::_unload(remind);
}

int FirstDistZhuang::Dispatch() {
    return 0;
}

HandoutResponse::~HandoutResponse() {
    DsMsgParser::_unload(ting);
}

int HandoutResponse::Construct(const DsMsg &msg) {
    status = (Status_t)msg.GetItemValue(0);
    ting.cardNum = msg._body->_items[1]->_bufLen;
    DsMsgParser::_load(ting,msg._body->_items[1]->_buf);
    return 0;
}

int HandoutResponse::Dispatch() {
    return 0;
}

HandoutNotif::~HandoutNotif() {
    DsMsgParser::_unload(ting);
}

int HandoutNotif::Construct(const DsMsg &msg) {
    seat         = (Status_t)msg.GetItemValue(0);
    kind         = (Card_t)msg.GetItemValue(1);
    ting.cardNum = msg._body->_items[2]->_bufLen;
    DsMsgParser::_load(ting,msg._body->_items[2]->_buf);
    return 0;
}

int HandoutNotif::Dispatch() {
    return 0;
}

int ActionResponse::Construct(const DsMsg &msg) {
    seat     = msg.GetItemValue(0);
    waitSeat = msg.GetItemValue(1);
    return 0;
}

int ActionResponse::Dispatch() {
    return 0;
}

int ActionNotif::Construct(const DsMsg &msg) {
    seat    = msg.GetItemValue(0);
    isFromServer = (msg.GetItemValue(1)==0)?true:false;
    next    = msg.GetItemValue(2);

    DsMsgParser::_load(actions, actionNum, msg, 3);
    DsMsgParser::_load(card, cardNum, msg, 4);
    return 0;
}

int ActionNotif::Dispatch() {
    return 0;
}

DistCardInfo::~DistCardInfo() {
    DsMsgParser::_unload(remind);
}

int DistCardInfo::Construct(const DsMsg &msg) {
    seat      = msg.GetItemValue(0);
    timer     = msg.GetItemValue(1);
    remain    = msg.GetItemValue(2);
    kind      = (Card_t)msg.GetItemValue(3);

    DsMsgParser::_load(remind.actions, remind.actionNum, msg, 4);
    DsMsgParser::_load(remind.gangCard, remind.gangKindNum, msg, 5);
    DsMsgParser::_load(remind.kouCard, remind.kouKindNum, msg, 6);
    DsMsgParser::_load(remind.ming, msg, 7);
    return 0;
}

int DistCardInfo::Dispatch() {
    return 0;
}

int FirstDistNonZhuang::Construct(const DsMsg &msg) {
    seat      = msg.GetItemValue(0);
    remain    = msg.GetItemValue(1);
    memcpy(cards, msg._body->_items[2]->_buf, 13);
    zhuang    = msg.GetItemValue(3);
    timer     = msg.GetItemValue(4);
    
    return 0;
}

int FirstDistNonZhuang::Dispatch() {
    return 0;
}

RemindInfo::~RemindInfo() {
    DsMsgParser::_unload(remind);
}

int RemindInfo::Construct(const DsMsg &msg) {
    seat   = msg.GetItemValue(0);
    timer  = msg.GetItemValue(1);
    DsMsgParser::_load(remind, msg, 2);
    wait = msg.GetItemValue(6);
    
    return 0;
}

int RemindInfo::Dispatch() {
    return 0;
}

int DistCardNotif::Construct(const DsMsg &msg) {
    seat      = msg.GetItemValue(0);
    remain    = msg.GetItemValue(1);
    timer     = msg.GetItemValue(2);
    kind      = (Card_t)msg.GetItemValue(3);
    return 0;
}

int DistCardNotif::Dispatch() {
    return 0;
}

int ScoreNotif::Construct(const DsMsg &msg) {
    seat[0] = msg._body->_items[0]->_buf[0];
    seat[1] = msg._body->_items[0]->_buf[1];
    seat[2] = msg._body->_items[0]->_buf[2];

    val[0] = _ntohl( *((INT32U *)(msg._body->_items[1]->_buf)) );
    val[1] = _ntohl( *((INT32U *)(msg._body->_items[1]->_buf+4)) );
    val[2] = _ntohl( *((INT32U *)(msg._body->_items[1]->_buf+8)) );
    return 0;
}

int ScoreNotif::Dispatch() {
    return 0;
}

int DecisionNotif::Construct(const DsMsg &msg) {
    seat      = msg.GetItemValue(0);
    whoGive   = msg.GetItemValue(1);
    next      = msg.GetItemValue(2);
    DsMsgParser::_load(actions, actionNum, msg, 3);
    card      = (Card_t)msg.GetItemValue(4);
    return 0;
}

int DecisionNotif::Dispatch() {
    return 0;
}

int TingInfoResponse::Construct(const DsMsg &msg) {
    info.cardNum = msg._body->_items[0]->_bufLen/4;
    DsMsgParser::_load(info,msg._body->_items[0]->_buf);
    return 0;
}

int TingInfoResponse::Dispatch() {
    return 0;
}


