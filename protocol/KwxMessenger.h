
#ifndef _KWX_MESSENGER_
#define _KWX_MESSENGER_

#include "./../RaceType.h"

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"
#include "./../network/NetMessenger.h"
#include "./../utils/LogManager.h"

#include "KwxRequestConsts.h"
#include "KwxRequestTypes.h"

class KwxMessenger {
public:
    KwxMessenger();
    ~KwxMessenger();
    
    /* auto receive */
    static void StartReceiving();
    static void StopReceiving();
    void StartReceiving(MsgHandler_t handle);               //this method should only be referenced by test cases.

    /* upstream */
    int SetAction(ActionId_t code);
    int SetShowCard(CardType_t code);
    int SetReaction(ActionId_t code,CardType_t kind);
    int SetReaction(ActionId_t code,int num,Card_t *kind);
    int SetRequestDistribute();
    int SetUpdateCardList(CARD *cards,int cardNum);

private:
	static NetMessenger *_messenger;
    static Logger       *_logger;
};
#endif
