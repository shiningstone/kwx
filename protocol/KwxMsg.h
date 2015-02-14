
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
    int SetRoomPath(RoomPath_t code);
    int SetRoomId(RoomId_t code);
    int SetTableId(TableId_t code);
    int SetSeatId(INT32U code);
    int SetAction(ActionId_t code);

    Header   *_header;
    MsgBody  *_body;
};

#define KWX_INVALID_PCHC          -1
#define KWX_UNSUPPORTED_PARAM     -2
#define KWX_DISMATCH_LENGTH       -3

#endif