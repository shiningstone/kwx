
#ifndef __KWX_MESSENGER_H__
#define __KWX_MESSENGER_H__

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

class NetMessenger;

#include "KwxMsgBasic.h"
class KwxUsMsg;

class KwxMessenger {
public:
    KwxMessenger();
    ~KwxMessenger();
    
    /* auto receive */
    static void StartReceiving();
    static void StopReceiving();
    static void StartReceiving(MsgHandler_t handle);               //this method should only be referenced by test cases.

    int Send(KwxUsMsg &aMsg);/* why cannot declare as const KwxUsMsg??? */

private:
	static NetMessenger *_messenger;
    static Logger       *_logger;
};

class RequestSendAction : public KwxUsMsg {
public:
    int Set(ActionId_t code,Card_t card);
};

class RequestGameStart : public KwxUsMsg {
public:
    int Set();
};

class RequestHandout : public KwxUsMsg {
public:
    int Set(Card_t card);
};

class RequestTingInfo : public KwxUsMsg {
public:
    int Set();
};

#endif
