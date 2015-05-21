
#ifndef __KWX_MESSENGER_H__
#define __KWX_MESSENGER_H__

#include "./../utils/UtilBasic.h"
#include "./../game/GameType.h"
#include "./../utils/LogManager.h"

class NetMessenger;

#include "KwxMsgBasic.h"
class UsMsg;

class KwxMessenger {
public:
    KwxMessenger();
    ~KwxMessenger();
    
    /* auto receive */
    static void StartReceiving();
    static void StopReceiving();

    int Send(UsMsg &aMsg);/* why cannot declare as const UsMsg??? */
    bool Wait(RequestId_t rsp);
    void Resume(RequestId_t req = REQ_INVALID);

private:
    friend class TestKwxAutoRecv;
    static void StartReceiving(MsgHandler_t handle);//this method should only be referenced by test cases.

    RequestId_t _waitReq;
    int         _sendCnt;

	static NetMessenger *_messenger;/*it makes more sense to let KwxMessenger derived from NetMessenger*/
    static Logger       *_logger;
};

class CSocket;
class KwxHeart {
public:
    static KwxHeart *getInstance(int second=5);
    static void destroyInstance();

    void SetRate(int second);
private:
    KwxHeart(int second);
    ~KwxHeart();

    void    _Beats();

    CSocket *_socket;
    int      _rate;
    bool     _running;

    static KwxHeart *_instance;
};

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

class RequestTingInfo : public UsMsg {
public:
    int Set();
};


class RequestEnterRoom : public UsMsg {
public:
    int Set(int id=0);
};

class RequestHeartBeat : public UsMsg {
public:
    int Set();
};

#endif
