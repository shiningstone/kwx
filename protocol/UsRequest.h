
#ifndef __US_REQUEST__
#define __US_REQUEST__

#include "./../game/GameType.h"
#include "KwxMsgBasic.h"
class UsMsg;

class RequestSendAction : public UsMsg {
public:
    int Set(ActionId_t action,Card_t card);
    int Set(ActionId_t code,int kindNum=1,Card_t cards[]=NULL);
    virtual void Desc(char *buf) const;

    ActionId_t _act;
    Card_t     _card;
};

class RequestShowCard : public UsMsg {
public:
    virtual void Desc(char *buf) const;
    int Set(Card_t card);

    Card_t _card;
};

class RequestGameStart : public UsMsg {
public:
    int Set();
};

class RequestGameRestart : public UsMsg {
public:
    int Set();
};

class RequestTingInfo : public UsMsg {
public:
    int Set();
};

class RequestTuoGuan : public UsMsg {
public:
    int Set(bool status);
};


#endif

