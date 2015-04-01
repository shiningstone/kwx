
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

class KwxMsg {
public:
    KwxMsg(int dir);
    ~KwxMsg();

    const int _dir;    
    Header  *_header;
    MsgBody *_body;

    static Logger       *_logger;
};

class KwxDsMsg : public KwxMsg, public DsMsgIntf {
public:
    KwxDsMsg();
    
    RequestId_t GetRequestCode();
    int         GetLevel();
    int         Construct(ActionResponse_t &waitInfo);

    virtual int Deserialize(const INT8U *inMsg);
};

class KwxUsMsg : public KwxMsg, public UsMsgIntf {
public:
    KwxUsMsg();
    
    int SendAction(INT8U *buf,ActionId_t code,Card_t card);

    virtual int Serialize(INT8U *outMsg);
	void _set_size(INT8U *buf,INT16U len);
	int  _add_item(Item *item);

    int SetRequestCode(RequestId_t code);
    int AddRoomPath(RoomPath_t code);
    int AddRoomId(RoomId_t code);
    int AddTableId(TableId_t code);
    int AddSeatId(INT8U code);
    int AddAction(ActionId_t code);
    int AddCard(Card_t card);
};

#define KWX_INVALID_PCHC          -1
#define KWX_UNSUPPORTED_PARAM     -2
#define KWX_DISMATCH_LENGTH       -3

#endif
