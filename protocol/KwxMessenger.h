
#ifndef __KWX_MESSENGER_H__
#define __KWX_MESSENGER_H__

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"
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

private:
    friend class TestKwxAutoRecv;
    static void StartReceiving(MsgHandler_t handle);//this method should only be referenced by test cases.

	static NetMessenger *_messenger;/*it makes more sense to let KwxMessenger derived from NetMessenger*/
    static Logger       *_logger;
};

class RequestSendAction : public UsMsg {
public:
    int Set(ActionId_t code,Card_t card);
};

class RequestGameStart : public UsMsg {
public:
    int Set();
};

class RequestHandout : public UsMsg {
public:
    int Set(Card_t card);
};

class RequestTingInfo : public UsMsg {
public:
    int Set();
};

#endif
