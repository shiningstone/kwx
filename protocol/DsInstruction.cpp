
#include <string.h>

#include "./../network/KwxEnv.h"

#include "MsgFormats.h"
#include "DsInstruction.h"
#include "DsMsgParser.h"

#ifndef __UNIT_TEST__
#include "./../network/KwxMessenger.h"
#include "./../game/RaceLayer.h"
#include "./../game/RoundManager.h"
#include "./../game/NetRoundManager.h"
#endif

#include "DbgRequestDesc.h"

int DsInstruction::_recvCnt = 0;
SeatInfo *DsInstruction::_seatInfo = 0;
Logger *DsInstruction::_logger = 0;

DsInstruction::DsInstruction() {
    _seatInfo = SeatInfo::getInstance();
    
    #ifndef __UNIT_TEST__
    _roundManager = static_cast<NetRoundManager *>(NetRoundManager::getInstance());
    #endif
    _logger = LOGGER_REGISTER("DsInstruction");
}

int DsInstruction::Construct(const DsMsg &msg) {
    request = msg.GetRequestCode();
    return _IsRejection(msg);
}

int DsInstruction::Dispatch() {
    Show();
    
    #ifndef __UNIT_TEST__
    _delay(100);/*防止接收的包在等待函数之前就处理了*/
    if(KwxMessenger::IsWaiting(this->request)) {
        if(failure==REQUEST_ACCEPTED) {                            /*注意 : 非main线程，不能调用UI接口*/
            _roundManager->HandleMsg(this);
        } else {
            _roundManager->RecordError(this);
        }
    } else {
        _roundManager->RecvMsg(this);
    }
    #endif
    
    return 0;
}

void DsInstruction::Show() const {
    LOGGER_WRITE("<---------------- %d:%s:",++_recvCnt,DescReq(request));
}

PlayerDir_t DsInstruction::_GetPlayer(INT8U seat) {
    PlayerDir_t dir = _seatInfo->GetPlayer(seat);
    if(dir==SERVER) {
        return SERVER;
    } else if( dir<LEFT || dir>RIGHT ) {
        return INVALID_DIR;
    } else {
        return dir;
    }
}

bool DsInstruction::_IsRejection(const DsMsg &msg) {
    RequestId_t Responses[] = {
        REQ_GAME_SEND_START,
        REQ_GAME_SEND_SHOWCARD,
        REQ_GAME_SEND_ACTION,
        REQ_GAME_GET_TINGINFO,
        REQ_GAME_SEND_ENTER,
        REQ_LOGIN,
        REQ_DAILY_LOGIN,
        //REQ_GET_DAILY_PRIZE,
        REQ_GAME_SEND_RECONNECT,
        REQ_GAME_SEND_LEAVE_ROOM,
        REQ_GAME_SEND_XIA_PIAO,
        REQ_GAME_SEND_MSG,
    };

    RequestId_t req = (RequestId_t)msg._header->_requestCode;

    for(int i=0;i<sizeof(Responses)/sizeof(RequestId_t);i++) {
        if(req==Responses[i]) {
            if(msg._body->_items[0]->_id==60) {
                failure = (FailureCode_t)msg.GetItemValue(0);

                if(failure<DATAGRAM_ERROR) {
                    failure = REQUEST_ACCEPTED;
                    return false;
                }

                if(failure==RECONNECT_REQUIRED) {
                    DsMsgParser::_load_seat_info(reconnectInfo,msg,1);
                }

                char buf[128] = {0};
                sprintf(buf,"Error code %d",failure);
                
                LOGGER_WRITE("%s (%s)",__FUNCTION__,((DescErr(failure)==NULL)?buf:DescErr(failure)));
                return true;
            }
        }
    }

    failure = REQUEST_ACCEPTED;
    return false;
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
    ting.kindNum = msg._body->_items[1]->_bufLen/4;
    DsMsgParser::_load_ting_info(ting,msg._body->_items[1]->_buf);
    return 0;
}

ShowCardResponse::~ShowCardResponse() {
    DsMsgParser::_unload(ting);
}

int ShowCardNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat         = _GetPlayer(msg.GetItemValue(0));
    kind         = (Card_t)msg.GetItemValue(1);
    ting.kindNum = msg._body->_items[2]->_bufLen;
    DsMsgParser::_load_ting_info(ting,msg._body->_items[2]->_buf);
    return 0;
}

void ShowCardNotif::Show() const {
     DsInstruction::Show();
     LOGGER_WRITE("%s handout %s\n",DescPlayer((PlayerDir_t)seat),DescCard(kind));
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
    actions = msg.GetItemValue(3);
    if(actions==aHU || actions==aMING) {
        DsMsgParser::_load(huCards, huCardsNum, msg, 4);
    } else {
        DsMsgParser::_load(card, cardNum, msg, 4);
    }
    return 0;
}

void ActionNotif::Show() const {
     DsInstruction::Show();
     LOGGER_WRITE("%s act %s %s from %s\n",DescPlayer((PlayerDir_t)seat),DescAct((ActionId_t)actions),
        DescCard(card[0].kind),DescPlayer((PlayerDir_t)whoGive));
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

    remind.actions = msg.GetItemValue(4);
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

void DistCardNotif::Show() const {
     DsInstruction::Show();
}

int ScoreNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);

    for(int i=0;i<3;i++) {
        val[i] = 0;
    }

    for(int i=0;i<msg._body->_items[0]->_bufLen;i++) {
        seat[i] = _GetPlayer(msg._body->_items[0]->_buf[i]);
    }

    for(int i=0;i<msg._body->_items[0]->_bufLen;i++) {
        val[seat[i]] = _ntohl( *((INT32U *)(msg._body->_items[1]->_buf + sizeof(INT32S)*i)) );
    }
    
    return 0;
}

int TingInfoResponse::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    info.kindNum = msg._body->_items[0]->_bufLen/4;
    DsMsgParser::_load_ting_info(info,msg._body->_items[0]->_buf);
    return 0;
}

int HuInfoNotif::Construct(const DsMsg &msg) {
    INT8U       NUM = 3;
    INT32U      fan[3];
    INT8U       status[3];
    INT32U      score[3];
    INT8U       cardNum[3];
    CardNode_t  card[3][18];

    DsInstruction::Construct(msg);

    zhuang    = _GetPlayer(msg.GetItemValue(0));
    
    DsMsgParser::_load(fan,msg,1);
    DsMsgParser::_load(status,msg,2);
    DsMsgParser::_load(score,msg,3);
    DsMsgParser::_load(card,cardNum,msg,4);
    
    for(int i=0;i<PLAYER_NUM;i++) {
        PlayerDir_t dir = PlayerDir_t((zhuang+i)%3);
        
        hu[dir].fan    = fan[i];
        hu[dir].status = (HuKind_t)status[i];
        hu[dir].score  = score[i];
        hu[dir].cardNum= cardNum[i];
        memcpy(hu[dir].card, card[i], sizeof(CardNode_t)*18);
    }
    
    return 0;
}

int TuoGuanResponse::Construct(const DsMsg &msg) {
    status = msg.GetItemValue(0);
    return 0;
}

int TuoGuanNotif::Construct(const DsMsg &msg) {
    dir    = _GetPlayer(msg.GetItemValue(0));
    status = (msg.GetItemValue(1)==1);
    return 0;
}

int CounterNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);

    count     = msg.GetItemValue(0);
    seat      = _GetPlayer(msg.GetItemValue(1));

	return 0;
}

