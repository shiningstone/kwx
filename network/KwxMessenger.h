
#ifndef __KWX_MESSENGER_H__
#define __KWX_MESSENGER_H__

#include <vector>
using namespace std;

#include "./../utils/UtilBasic.h"
#include "./../utils/LogManager.h"
#include "./../game/GameType.h"
#include "./../protocol/KwxMsgBasic.h"
#include "NetMessenger.h"

class UsMsg;

typedef enum {
    MSG_LOGIN,
    MSG_GAME,
    MSG_CHAT,
}MsgType_t;

class KwxMessenger : public NetMessenger {
public:
    KwxMessenger(MsgType_t type);
    ~KwxMessenger();
    
    /* auto receive */
    void StartReceiving();
    void StopReceiving();

    int Send(UsMsg &aMsg,bool ignoreRsp=false);/* why cannot declare as const UsMsg??? */

    static bool IsWaiting(RequestId_t req = REQ_INVALID);
    static bool Wait(RequestId_t req);
    static void Resume(RequestId_t req = REQ_INVALID);
    static void WaitQueueAdd(RequestId_t req);
private:
    /* wait queue for specified request, which will be handled in Non-main process */
    static const int MAX_WAIT_NUM = 4;

    static std::vector<RequestId_t> _waitQueue;
    static int WaitQueueFind(RequestId_t req);

    friend class TestKwxAutoRecv;
    void StartReceiving(MsgHandler_t handle);//this method should only be referenced by test cases.

    INT8U       _serverIp[32];
    int         _port;

    int         _sendCnt;

    static Logger       *_logger;
};

#endif
