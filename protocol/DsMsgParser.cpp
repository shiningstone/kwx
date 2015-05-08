
#include <string.h>

#include "MsgFormats.h"
#include "CommonMsg.h"
#include "DsMsgParser.h"

int DsMsgParser::_load(INT8U *status,INT8U &num,const DsMsg &msg,int itemIdx) {
    num = (INT8U)msg.GetItemBufLen(itemIdx);
    
    for(int i=0;i<num;i++) {
        status[i] = msg._body->_items[itemIdx]->_buf[i];
    }

    return 0;
}

int DsMsgParser::_load(INT32U *fan, const bool *status,const DsMsg &msg,int itemIdx) {
    int idx      = 0;
    
    for(int i=0;i<PLAYER_NUM;i++) {
        if(status[i]) {
            *(fan+i) = _ntohl(*((INT32U *)(msg._body->_items[itemIdx]->_buf+idx)));
            idx += 4;
        } else {
            *(fan+i) = 0;
        }
    }

    return 0;
}

int DsMsgParser::_load(INT32U *score, INT8U &num, const DsMsg &msg,int itemIdx) {
    num = (INT8U)msg.GetItemBufLen(itemIdx)/4;
    
    for(int i=0;i<PLAYER_NUM;i++) {
        score[i] = _ntohl(*((INT32U *)(msg._body->_items[itemIdx]->_buf+4*i)));
    }

    return 0;
}

int DsMsgParser::_load(Card_t cards[3][18],INT8U cardNum[3],const DsMsg &msg,int itemIdx) {
    int totalNum = (INT8U)msg.GetItemBufLen(itemIdx);

    int player = 0;
    int idx    = 0;
    int i      = 0;
    
    while(i<totalNum) {
        Card_t kind = (Card_t)msg._body->_items[itemIdx]->_buf[i];
        
        if(kind!=CARD_UNKNOWN) {
            cards[player][idx] = kind;
            idx++;
        } else {
            cardNum[player++] = idx;
            idx = 0;
        }

        i++;
    }

    cardNum[player++] = idx;

    return 0;
}

int DsMsgParser::_load(Card_t *cards,INT8U &num,const DsMsg &msg,int itemIdx) {
    num = (INT8U)msg.GetItemBufLen(itemIdx);
    
    for(int i=0;i<num;i++) {
        cards[i] = (Card_t)msg._body->_items[itemIdx]->_buf[i];
    }

    if(num==1 && cards[0]==CARD_UNKNOWN) {
        num = 0;
    }

    return 0;
}

int DsMsgParser::_load(TingInfo_t &ting,const INT8U *inMsg) {
    const INT8U *p = inMsg;

    if(_ntohl(*(INT32U *)(inMsg))==0xffffffff) {
        ting.cardNum = 0;
        return 4;
    } else {
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
}

int DsMsgParser::_unload(TingInfo_t &ting) {
    if(ting.cardNum>0) {
        delete []ting.cards;

        ting.cards = NULL;
        ting.cardNum = 0;
    }

    return 0;
}

int DsMsgParser::_load(MingInfo_t &ming,const DsMsg &msg,int itemIdx) {
    Item *item = msg._body->_items[itemIdx];

    if(_ntohl(*(INT32U *)(item->_buf))==0xffffffff) {
        ming.choiceNum = 0;
        return 0;
    }

    int   len = item->_bufLen;
    INT8U *p = item->_buf;
    MingChoice_t choices[13];

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
    ming.handouts  = new MingChoice_t[idx];
    memcpy(ming.handouts,choices,sizeof(MingChoice_t)*idx);

	return 0;
}

int DsMsgParser::_unload(MingInfo_t &ming) {
    for(int i=0;i<ming.choiceNum;i++) {
        MingChoice_t choice = ming.handouts[i];
        _unload(choice.ting);
    }

    if(ming.choiceNum>0) {
        delete []ming.handouts;

        ming.handouts = NULL;
        ming.choiceNum = 0;
    }
    
    return 0;
}

int DsMsgParser::_load(Reminds_t &remind,const DsMsg &msg,int itemIdx) {
    remind.actions = _ntohl(*(INT32U *)(msg._body->_items[itemIdx]->_buf));
    _load(remind.gangCard, remind.gangKindNum, msg, itemIdx+1);
    _load(remind.kouCard, remind.kouKindNum, msg, itemIdx+2);
    _load(remind.ming,msg, itemIdx+3);

    return 0;
}

int DsMsgParser::_unload(Reminds_t &remind) {
    _unload(remind.ming);
    return 0;
}

