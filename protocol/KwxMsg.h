
#ifndef __KWX_MSG__
#define __KWX_MSG__

#include "./../RaceType.h"

#include "./../utils/UtilBasic.h"
#include "KwxRequestConsts.h"

#define UP_STREAM     0
#define DOWN_STREAM   1

class Header;
class MsgBody;
class Item;
class NetMessenger;

class KwxMsg : public MsgIntf {
public:
    KwxMsg(int dir);
    ~KwxMsg();
    
    /* auto receive */
    void StartReceiving(MsgHandler_t handle);
    void StopReceiving();

    /* upstream */
    int SetAction(INT8U *buf,ActionId_t code);
    int SetShowCard(INT8U *buf,CardType_t code);
    int SetReaction(INT8U *buf,ActionId_t code);
    int SetRequestDistribute(INT8U *buf);
    int SetUpdateCardList(INT8U *buf,CARD *cards,int cardNum);
    
/* the following items should be only referenced by test */
/*
protected:
*/
    /* downstream*/
    RequestId_t GetRequestCode();
    int         GetLevel();
    int         Construct(DistributeResponse_t &response);
    int         Construct(OthersAction_t &actionInfo);
    int         Construct(OthersShowCard_t &cardInfo);

    virtual int Serialize(INT8U *outMsg);
    virtual int Deserialize(const INT8U *inMsg);

    NetMessenger    *_messenger;
    Header          *_header;
    MsgBody         *_body;
protected:
	const int   _dir;

	void _set_size(INT8U *buf,INT16U len);
	int  _add_item(Item *item);

    /* downstream */
    virtual void _handle_downsteam_packages(const INT8U *pkg,int &len);

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