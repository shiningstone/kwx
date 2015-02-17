
#ifndef __KWX_MSG__
#define __KWX_MSG__

#include "./../RaceType.h"

#include "MsgBasic.h"
#include "KwxRequestConsts.h"

#define UP_STREAM     0
#define DOWN_STREAM   1

class Header;
class MsgBody;
class Item;

class KwxMsg : public MsgIntf {
public:
    KwxMsg(int dir);
    ~KwxMsg();
    
    /* upstream */
    int SetAction(ActionId_t code);
    int SetShowCard(CardType_t code);
    int SetRequestDistribute();
    int SetUpdateCardList(CARD *cards,int cardNum);
    
    /* downstream*/
    int         Construct(DistributeResponse_t &response,const INT8U *inMsg);
    int         Construct(OthersAction_t &actionInfo,const INT8U *inMsg);
    RequestId_t GetRequestCode();
    int         GetLevel();

/* the following items should be only referenced by test*/
/*
protected:
*/
    virtual int Serialize(INT8U *outMsg);
    virtual int Deserialize(const INT8U *inMsg);

    Header   *_header;
    MsgBody  *_body;
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
    int AddShowCard(CardType_t card);
    int AddCards(CARD *cards,int num);
};

#define KWX_INVALID_PCHC          -1
#define KWX_UNSUPPORTED_PARAM     -2
#define KWX_DISMATCH_LENGTH       -3

#endif