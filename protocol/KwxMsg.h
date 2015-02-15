
#ifndef __KWX_MSG__
#define __KWX_MSG__

#include "MsgBasic.h"
#include "KwxRequestConsts.h"

#define UP_STREAM     0
#define DOWN_STREAM   1

class Header;
class MsgBody;

class KwxMsg : public MsgIntf {
public:
    KwxMsg(int dir);
    ~KwxMsg();

    virtual int Serialize(INT8U *outMsg);
    virtual int Deserialize(const INT8U *inMsg);

    int SetRequestCode(RequestId_t code);
    int AddRoomPath(RoomPath_t code);
    int AddRoomId(RoomId_t code);
    int AddTableId(TableId_t code);
    int AddSeatId(INT32U code);
    int AddAction(ActionId_t code);

    Header   *_header;
    MsgBody  *_body;
protected:
	const int   _dir;

	void _set_size(INT8U *buf,INT16U len);
};

#define KWX_INVALID_PCHC          -1
#define KWX_UNSUPPORTED_PARAM     -2
#define KWX_DISMATCH_LENGTH       -3

#endif