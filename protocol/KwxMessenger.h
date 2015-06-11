
#ifndef __KWX_MESSENGER_H__
#define __KWX_MESSENGER_H__

#include "./../utils/UtilBasic.h"
#include "./../game/GameType.h"
#include "./../utils/LogManager.h"

class NetMessenger;

#include "KwxMsgBasic.h"
class UsMsg;

typedef enum {
    MSG_LOGIN,
    MSG_GAME,
    MSG_CHAT,
}MsgType_t;

class KwxMessenger {
public:
    KwxMessenger(MsgType_t type);
    ~KwxMessenger();
    
    /* auto receive */
    void StartReceiving();
    void StopReceiving();

    int Send(UsMsg &aMsg);/* why cannot declare as const UsMsg??? */

private:
    friend class TestKwxAutoRecv;
    void StartReceiving(MsgHandler_t handle);//this method should only be referenced by test cases.

    INT8U       _serverIp[32];
    int         _port;

    int         _sendCnt;

	static NetMessenger *_messenger;/*it makes more sense to let KwxMessenger derived from NetMessenger*/
    static Logger       *_logger;
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

class RequestTuoGuan : public UsMsg {
public:
    int Set(bool status);
};

#endif
