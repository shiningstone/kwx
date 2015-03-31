
#ifndef __KWX_MSG__
#define __KWX_MSG__


#include "./../RaceType.h"

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

#include "KwxRequestConsts.h"
#include "KwxRequestTypes.h"

#define UP_STREAM     0
#define DOWN_STREAM   1

class Header;
class MsgBody;
class Item;
class NetMessenger;
/**********************************************************************
关于下行包接收：
    用户可以自行创建一个线程，将接收到的报文交给Deserialize/Construct依次处理；
    或者
    使用StartReceiving()启动自动接收处理模式，处理函数为_HANDLE_DS_PACKAGES。
**********************************************************************/
class KwxMsg : public MsgIntf {
public:
    KwxMsg(int dir);
    ~KwxMsg();
    
    /* upstream */
    int SendAction(INT8U *buf,ActionId_t code,Card_t card);
    int SetShowCard(INT8U *buf,CardType_t code);
    int SetReaction(INT8U *buf,ActionId_t code,CardType_t kind);
    int SetReaction(INT8U *buf,ActionId_t code,int num,Card_t *kind);
    int SetRequestDistribute(INT8U *buf);
    int SetUpdateCardList(INT8U *buf,CARD *cards,int cardNum);
    
    /* downstream*/
    RequestId_t GetRequestCode();
    int         GetLevel();
    int         Construct(DistributeResponse_t &response);
    int         Construct(OthersAction_t &actionInfo);
    int         Construct(OthersShowCard_t &cardInfo);

/* the following items should be only referenced by test */
/*
protected:
*/
    virtual int Serialize(INT8U *outMsg);
    virtual int Deserialize(const INT8U *inMsg);

    Header          *_header;
    MsgBody         *_body;
protected:
	const int   _dir;

	void _set_size(INT8U *buf,INT16U len);
	int  _add_item(Item *item);

    /* upstream */
    int SetRequestCode(RequestId_t code);
    int AddRoomPath(RoomPath_t code);
    int AddRoomId(RoomId_t code);
    int AddTableId(TableId_t code);
    int AddSeatId(INT8U code);
    int AddAction(ActionId_t code);
    int AddCard(Card_t card);
    
    int AddShowCard(CardType_t card);
    int AddCards(CARD *cards,int num);
    int AddCardKind(CardType_t code);
    int AddCardKind(int num,Card_t *card);

    static Logger       *_logger;
};

#define KWX_INVALID_PCHC          -1
#define KWX_UNSUPPORTED_PARAM     -2
#define KWX_DISMATCH_LENGTH       -3

#endif