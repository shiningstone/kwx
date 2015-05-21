
#include <string.h>

#include "MsgFormats.h"
#include "DsInstruction.h"
#include "DsMsgParser.h"
#include "KwxMsgEnv.h"

#ifndef __UNIT_TEST__
#include "./../game/RaceLayer.h"
#include "./../protocol/KwxMessenger.h"
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
    return 0;
}

int DsInstruction::Dispatch() {
    Show();
    
    #ifndef __UNIT_TEST__
    _delay(1);/*防止接收的包在等待函数之前就处理了*/
    _roundManager->_messenger->Resume();
    _roundManager->RecvMsg(this);
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
    DsMsgParser::_load(card, cardNum, msg, 4);
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
     LOGGER_WRITE("%s\n",DescPlayer((PlayerDir_t)seat));
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
        
    info.cardNum = msg._body->_items[0]->_bufLen/4;
    DsMsgParser::_load(info,msg._body->_items[0]->_buf);
    return 0;
}

int HuInfoNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);

    seat      = _GetPlayer(msg.GetItemValue(0));
    zhuang    = _GetPlayer(msg.GetItemValue(1));

    INT8U   playerNum = 0;
    INT8U   winType[3];
    INT32U  fan[3];
    INT32U  score[3];
    INT32U  bet[3];
    INT8U   name[3][128];
    INT8U   image[3][128];
    INT8U   cardNum[3];
    Card_t  card[3][18];

    DsMsgParser::_load(winType,playerNum,msg,3);
    
    bool    status[3];
    for(int i=0;i<3;i++) {
        if(winType[i]==HU_PAI || winType[i]==ZI_MO) {
            status[i] = true;
        } else {
            status[i] = false;
        }
    }
    DsMsgParser::_load(fan,status,msg,2);
    DsMsgParser::_load(score,playerNum,msg,4);
    DsMsgParser::_load(bet,playerNum,msg,5);
    DsMsgParser::_load(card,cardNum,msg,8);

    for(int i=0;i<PLAYER_NUM;i++) {
        hu[i].winType= winType[(i+2)%3];
        hu[i].fan    = fan[(i+2)%3];
        hu[i].score  = score[(i+2)%3];
        hu[i].bet    = bet[(i+2)%3];
        hu[i].cardNum= cardNum[(i+2)%3];
        for(int k=0;k<hu[i].cardNum;k++) {
            memcpy(hu[i].card, card[(i+2)%3], sizeof(Card_t)*18);
        }
    }
    
    return 0;
}

int EndInfoNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
    
    seat      = _GetPlayer(msg.GetItemValue(0));

    INT8U   num[3];
    Card_t  cards[3][18];
    DsMsgParser::_load(cards,num,msg,1);

    for(int i=0;i<PLAYER_NUM;i++) {
        cardNum[i] = num[(i+2)%3];
        memcpy(card[i], cards[(i+2)%3], sizeof(Card_t)*18);
    }

    return 0;
}

int EnterRoomResponse::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    roomPath = msg.GetItemValue(0);
    roomId   = msg.GetItemValue(1);
    tableId  = msg.GetItemValue(2);
    seat     = msg.GetItemValue(3);
    SeatInfo::getInstance()->Set(roomPath,roomId,tableId,seat);
    
    baseScore= msg.GetItemValue(4);
    
    playerNum= msg._body->_items[5]->_bufLen;
    
    for(int i=0;i<playerNum;i++) {
        status[i] = (msg._body->_items[5]->_buf[i]!=0);
        score[i]  = _ntohl( *(INT32U *)(msg._body->_items[6]->_buf + 4*i) );
        loadFromUtf16(name[i],msg._body->_items[7]->_buf);
        loadFromUtf16(image[i],msg._body->_items[8]->_buf);
    }
    
    return 0;
}

int EnterRoomResponse::loadFromUtf16(INT8U *buf,const INT8U *input) {
    return 0;
}


int EnterRoomNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);
        
    seat     = msg.GetItemValue(0);
    
    return 0;
}

int CounterNotif::Construct(const DsMsg &msg) {
    DsInstruction::Construct(msg);

    count     = msg.GetItemValue(0);
    seat      = _GetPlayer(msg.GetItemValue(1));

	return 0;
}

