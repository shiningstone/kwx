
#ifndef _KWX_MESSENGER_
#define _KWX_MESSENGER_

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

class NetMessenger;
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
#endif
