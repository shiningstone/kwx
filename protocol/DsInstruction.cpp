
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

int DsInstruction::GetAvailActions(int actNum,const ActionId_t actions[]) {
    int actionToDo = 0;
    
    for (int i=0;i<actNum; i++) {
        actionToDo |= actions[i];
    }

    return actionToDo;
}

PlayerDir_t DsInstruction::_GetPlayer(INT8U seat) {
    PlayerDir_t dir = _seatInfo->GetPlayer(seat);
    if( dir<LEFT || dir>RIGHT ) {
        return INVALID_DIR;
    } else {
        return dir;
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
    DsInstruction::Construct(msg);
        
    INT8U num = 0;
    
    seat      = _GetPlayer(msg.GetItemValue(0));
    remain    = msg.GetItemValue(1);
    timer     = msg.GetItemValue(2);
    DsMsgParser::_load(cards, num, msg, 3);
    DsMsgParser::_load(remind, msg, 4);
    return 0;
}

FirstDistZhuang::~FirstDistZhuang() {
    DsMsgParser::_unload(remind);
}

int FirstDistNonZhuang::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    INT8U num = 0;
        
    seat      = _GetPlayer(msg.GetItemValue(0));
    remain    = msg.GetItemValue(1);
    DsMsgParser::_load(cards, num, msg, 2);
    zhuang    = _GetPlayer(msg.GetItemValue(3));
    timer     = msg.GetItemValue(4);
    
    return 0;
}

int ShowCardResponse::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    status = (Status_t)msg.GetItemValue(0);
    ting.cardNum = msg._body->_items[1]->_bufLen/4;
    DsMsgParser::_load(ting,msg._body->_items[1]->_buf);
    return 0;
}

ShowCardResponse::~ShowCardResponse() {
    DsMsgParser::_unload(ting);
}

int ShowCardNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat         = _GetPlayer(msg.GetItemValue(0));
    kind         = (Card_t)msg.GetItemValue(1);
    ting.cardNum = msg._body->_items[2]->_bufLen;
    DsMsgParser::_load(ting,msg._body->_items[2]->_buf);
    return 0;
}

ShowCardNotif::~ShowCardNotif() {
    DsMsgParser::_unload(ting);
}

int ActionResponse::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat     = _GetPlayer(msg.GetItemValue(0));
    waitSeat = _GetPlayer(msg.GetItemValue(1));
    return 0;
}

int ActionNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat    = _GetPlayer(msg.GetItemValue(0));
    whoGive = _GetPlayer(msg.GetItemValue(1));
    next    = _GetPlayer(msg.GetItemValue(2));

    DsMsgParser::_load(actions, actionNum, msg, 3);
    DsMsgParser::_load(card, cardNum, msg, 4);
    return 0;
}

DistCardInfo::~DistCardInfo() {
    DsMsgParser::_unload(remind);
}

int DistCardInfo::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat      = _GetPlayer(msg.GetItemValue(0));
    timer     = msg.GetItemValue(1);
    remain    = msg.GetItemValue(2);
    kind      = (Card_t)msg.GetItemValue(3);

    DsMsgParser::_load(remind.actions, remind.actionNum, msg, 4);
    DsMsgParser::_load(remind.gangCard, remind.gangKindNum, msg, 5);
    DsMsgParser::_load(remind.kouCard, remind.kouKindNum, msg, 6);
    DsMsgParser::_load(remind.ming, msg, 7);
    return 0;
}

RemindInfo::~RemindInfo() {
    DsMsgParser::_unload(remind);
}

int RemindInfo::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat   = _GetPlayer(msg.GetItemValue(0));
    timer  = msg.GetItemValue(1);
    kind   = (Card_t)msg.GetItemValue(2);
    whoGive = _GetPlayer(msg.GetItemValue(3));
    DsMsgParser::_load(remind, msg, 4);
    wait = _GetPlayer(msg.GetItemValue(8));
    
    return 0;
}

int DistCardNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat      = _GetPlayer(msg.GetItemValue(0));
    remain    = msg.GetItemValue(1);
    timer     = msg.GetItemValue(2);
    kind      = (Card_t)msg.GetItemValue(3);
    return 0;
}

int ScoreNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat[0] = _GetPlayer(msg._body->_items[0]->_buf[0]);
    seat[1] = _GetPlayer(msg._body->_items[0]->_buf[1]);
    seat[2] = _GetPlayer(msg._body->_items[0]->_buf[2]);

    val[0] = _ntohl( *((INT32U *)(msg._body->_items[1]->_buf)) );
    val[1] = _ntohl( *((INT32U *)(msg._body->_items[1]->_buf+4)) );
    val[2] = _ntohl( *((INT32U *)(msg._body->_items[1]->_buf+8)) );
    return 0;
}

int DecisionNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat      = _GetPlayer(msg.GetItemValue(0));
    whoGive   = _GetPlayer(msg.GetItemValue(1));
    next      = _GetPlayer(msg.GetItemValue(2));
    DsMsgParser::_load(actions, actionNum, msg, 3);
    card      = (Card_t)msg.GetItemValue(4);
    return 0;
}

int TingInfoResponse::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    info.cardNum = msg._body->_items[0]->_bufLen/4;
    DsMsgParser::_load(info,msg._body->_items[0]->_buf);
    return 0;
}

