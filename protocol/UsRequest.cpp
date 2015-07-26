#include <stdio.h>
#include <string.h>

#include "MsgFormats.h"
#include "CommonMsg.h"
#include "UsRequest.h"

int RequestSendAction::Set(ActionId_t action,Card_t card) {
    SetRequestCode(REQ_GAME_SEND_ACTION);

    _act  = action;
    _card = card;
    
    INT32U actionId = _htonl(action);
    INT8U  cardKind = card;

    AddSeatInfo();
    _add_item( new Item((Item_t)134,4,(INT8U *)&actionId) );
    _add_item( new Item((Item_t)135,1,(INT8U *)&cardKind) );

    return 0;
}

int RequestSendAction::Set(ActionId_t action,int kindNum,Card_t card[]) {
    SetRequestCode(REQ_GAME_SEND_ACTION);

    _act  = action;
    
    INT32U actionId = _htonl(action);
    INT8U  cardKind[4] = {0};

    if(kindNum==1 && card==NULL) {
        cardKind[0] = CARD_UNKNOWN;
    } else {
        for(int i=0;i<kindNum;i++) {
            cardKind[i] = (INT8U)card[i];
        }
    }

    AddSeatInfo();
    _add_item( new Item((Item_t)134,4,(INT8U *)&actionId) );
    _add_item( new Item((Item_t)135,kindNum,(INT8U *)cardKind) );

    return 0;
}

void RequestSendAction::Desc(char *buf) const {
    UsMsg::Desc(buf);
    sprintf(buf+strlen(buf),"%s %s",DescAct(_act),DescCard(_card));
}

int RequestGameStart::Set() {
    SetRequestCode(REQ_GAME_SEND_START);
    AddSeatInfo();

    return 0;
}

int RequestGameRestart::Set() {
    SetRequestCode(REQ_GAME_RESTART);

    return 0;
}

int RequestShowCard::Set(Card_t card) {
    SetRequestCode(REQ_GAME_SEND_SHOWCARD);
    AddSeatInfo();
    _add_item( new Item((Item_t)65,card) );

    _card = card;
    
    return 0;
}

void RequestShowCard::Desc(char *buf) const {
    UsMsg::Desc(buf);
    sprintf(buf+strlen(buf),"handout %s",DescCard(_card));
}

int RequestTingInfo::Set() {
    SetRequestCode(REQ_GAME_GET_TINGINFO);
    AddSeatInfo();

    return 0;
}

int RequestTuoGuan::Set(bool status) {
    SetRequestCode(REQ_GAME_SEND_AGENT);
    AddSeatInfo();
    _add_item( new Item(61,(status?1:0)) );

    return 0;
}


