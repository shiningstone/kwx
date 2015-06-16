
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

    bool IsWaiting(RequestId_t req = REQ_INVALID) const;
    bool Wait(RequestId_t req);
    void WaitQueueDel(RequestId_t req);
private:
    /* wait queue for specified request, which will be handled in Non-main process */
    static const int MAX_WAIT_NUM = 4;

    RequestId_t _waitQueue[MAX_WAIT_NUM];
    int         _waitNum;

    void WaitQueueAdd(RequestId_t req);

    friend class TestKwxAutoRecv;
    void StartReceiving(MsgHandler_t handle);//this method should only be referenced by test cases.

    INT8U       _serverIp[32];
    int         _port;
	NetMessenger *_messenger;

    int         _sendCnt;

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
