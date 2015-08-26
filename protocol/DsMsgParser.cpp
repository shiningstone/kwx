
#include <string.h>
#include <stdlib.h>

#include "SeatInfo.h"
#include "MsgFormats.h"
#include "CommonMsg.h"
#include "DsMsgParser.h"

int DsMsgParser::_load(INT8U *values,const DsMsg &msg,int itemIdx) {
    INT8U num = (INT8U)msg.GetItemBufLen(itemIdx);
    
    for(int i=0;i<num;i++) {
        values[i] = msg._body->_items[itemIdx]->_buf[i];
    }

    return 0;
}

int DsMsgParser::_load(INT32U *values, const DsMsg &msg,int itemIdx) {
    INT8U num = (INT8U)msg.GetItemBufLen(itemIdx)/4;
    
    for(int i=0;i<PLAYER_NUM;i++) {
        values[i] = _ntohl(*((INT32U *)(msg._body->_items[itemIdx]->_buf+4*i)));
    }

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

int DsMsgParser::_load(CardNode_t cards[18],INT8U &num,const DsMsg &msg,int itemIdx) {
    num = (INT8U)msg.GetItemBufLen(itemIdx);
    
    for(int i=0;i<num;i++) {
        INT8U byte = msg._body->_items[itemIdx]->_buf[i];

        (cards[i]).kind = (Card_t)(byte&0x1f);
        (cards[i]).status = (CardStatus_t)((byte&0xe0) >> 5);
        (cards[i]).canPlay = false;
    }

    if(num==1 && (cards[0]).kind==CARD_UNKNOWN) {
        num = 0;
    }

    return 0;
}

CardStatus_t DsMsgParser::_CardStatus(int netStatus) {
    switch(netStatus) {
        case 1:
            return sPENG;
        case 2:
            return sMING_GANG;
        case 3:
            return sAN_GANG;
        case 4:
            return sMING_KOU;
        default:
            return sFREE;
    }
}

int DsMsgParser::_load(CardNode_t cards[3][18],INT8U cardNum[3],const DsMsg &msg,int itemIdx) {
    int totalNum = (INT8U)msg.GetItemBufLen(itemIdx);

    int player = 0;
    int idx    = 0;
    int i      = 0;
    
    while(i<totalNum) {
        INT8U byte = msg._body->_items[itemIdx]->_buf[i];

        if(byte!=CARD_UNKNOWN) {
            cards[player][idx].kind = (Card_t)(byte&0x1f);
            cards[player][idx].status = _CardStatus((byte&0xe0)>>5);
            cards[player][idx].canPlay = false;
            
            idx++;
        } else {
            cardNum[player++] = idx;
            idx = 0;
        }

        i++;
    }

    cardNum[player] = idx;

    return 0;
}

int DsMsgParser::_load(bool isMing[3],INT8U tingNum[3],Card_t tingCards[3][18],const DsMsg &msg,int itemIdx) {
    for(int i=0;i<3;i++) {
        tingNum[i] = 0;
    }

    int bufLen = msg._body->_items[itemIdx]->_bufLen;
    INT8U *buf = msg._body->_items[itemIdx]->_buf;
    int player = MIDDLE;
    int idx = 0;

    isMing[player] = (buf[idx]==0);
    idx++;

    while(idx<bufLen) {
        if(buf[idx]!=CARD_UNKNOWN) {
            tingCards[player][tingNum[player]] = (Card_t)buf[idx];
            tingNum[player]++;
            idx++;
        } else {
            idx++;
            player = (player+1)%3;
            isMing[player] = (buf[idx]==0);
            idx++;
        }
    }

    return 0;
}

/*****************************************
    input  :  ting.kindNum
*****************************************/
int DsMsgParser::_load_ting_info(TingInfo_t &ting,const INT8U *inMsg) {
    const INT8U *p = inMsg;

    if(_ntohl(*(INT32U *)(inMsg))==0xffffffff) {
        ting.kindNum = 0;
        return 4;
    } else {
        ting.cards = new TingItem_t[ting.kindNum];
        
        int i = 0;
        int num = 0;
        while(i<ting.kindNum) {
            ting.cards[i].kind   = (Card_t)p[0+4*i];
            ting.cards[i].remain = p[1+4*i];
            ting.cards[i].fan    = _ntohs( *((INT16U *)(p+2+4*i)) );
        
            num += ting.cards[i].remain;
            i++;
        }
        
        return i*4;
    }
}

/*****************************************
    input  :  ting.cardNum
*****************************************/
int DsMsgParser::_load_ting_remind(TingInfo_t &ting,const INT8U *inMsg) {
    const INT8U *p = inMsg;

    if(_ntohl(*(INT32U *)(inMsg))==0xffffffff) {
        ting.kindNum = 0;
        return 0;
    } else {
        _TingItem_t cards[9];

        
        int idx = 0;
        int num = 0;
        while(_ntohl(*(INT32U *)(p+idx*4))!=0xffffffff) {
            cards[idx].kind   = (Card_t)p[0+4*idx];
            cards[idx].remain = p[1+4*idx];
            cards[idx].fan    = _ntohs( *((INT16U *)(p+2+4*idx)) );
        
            num += cards[idx].remain;
            idx++;
        }
        
        ting.cards = new TingItem_t[idx];
        ting.kindNum = idx;
        memcpy(ting.cards,cards,sizeof(_TingItem_t)*idx);            

        return idx*4;
    }
}

int DsMsgParser::_unload(TingInfo_t &ting) {
    if(ting.kindNum>0) {
        delete [](ting.cards);

        ting.cards = NULL;
        ting.kindNum = 0;
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

    int idx       = 0;
    int i   = 0;
    while(i<len) {
        i += 2;                 /*0x0000*/
        choices[idx].kind = (Card_t)(*(p+i));
        choices[idx].ting.cardNum = *(p+i+1);
        i += 2;

        i += _load_ting_remind(choices[idx].ting,p+i);
        i += 4;                 /*0xffffffff*/

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

int DsMsgParser::_load_seat_info(SeatInfo_t &info,const DsMsg &msg,int itemIdx) {
    INT8U buf[128] = {0};
    msg.GetString(itemIdx,buf);

    char *num = strtok((char *)buf,",");
    info.roomPath = atoi(num);

    num = strtok(NULL,",");
    info.roomId   = atoi(num);
    
    num = strtok(NULL,",");
    info.tableId  = atoi(num);
    
    num = strtok(NULL,",");
    info.seatId   = atoi(num);
    
    return 0;
}

int DsMsgParser::_load_friends_info(vector<Friend_Info> &infos,const DsMsg &msg,int itemIdx) {
    INT8U buf[512] = {0};
    msg.GetString(itemIdx,buf);

    vector<string> friends;
    _split(friends,buf,"%@");

    for(int i=0;i<friends.size();i++) {
        vector<string> profile;
        _split(profile,(INT8U *)friends.at(i).c_str(),",");

        Friend_Info info;
        info._id           = atoi(profile.at(0).c_str());
        info.photoFileName = profile.at(1);
        info.NikeName      = profile.at(2);
        if(!strcmp(profile.at(3).c_str(),"boy")) {
            info.friendSex = BOY;
        } else {
            info.friendSex = GIRL;
        }
        info.FriendGold    = atoi(profile.at(4).c_str());

        infos.push_back(info);
    }

    return 0;
}

int DsMsgParser::_load_others_info(vector<OtherPlayers_Info> &infos,const DsMsg &msg,int itemIdx) {
    return 0;
}

