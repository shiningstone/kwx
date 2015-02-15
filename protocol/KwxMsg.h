
#ifndef __KWX_MSG__
#define __KWX_MSG__

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

    virtual int Serialize(INT8U *outMsg);
    virtual int Deserialize(const INT8U *inMsg);

    int SetAction(ActionId_t code);
    int SetShowCard(CardType_t code);
    int SetRequestDistribute();

    int SetRequestCode(RequestId_t code);
    int AddRoomPath(RoomPath_t code);
    int AddRoomId(RoomId_t code);
    int AddTableId(TableId_t code);
    int AddSeatId(INT8U code);
    int AddAction(ActionId_t code);
    int AddShowCard(CardType_t card);

    Header   *_header;
    MsgBody  *_body;
protected:
	const int   _dir;

	void _set_size(INT8U *buf,INT16U len);
	int  _add_item(Item *item);
};

#define KWX_INVALID_PCHC          -1
#define KWX_UNSUPPORTED_PARAM     -2
#define KWX_DISMATCH_LENGTH       -3

#endif