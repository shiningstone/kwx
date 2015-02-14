
#ifndef __KWX_MSG__
#define __KWX_MSG__

#include "MsgFormats.h"

#define UP_STREAM     0
#define DOWN_STREAM   1

class KwxMsg : public MsgIntf {
public:
    KwxMsg(int dir);
    ~KwxMsg();

    virtual int Serialize(INT8U *outMsg);
    virtual int Deserialize(const INT8U *inMsg);

    Header   *_header;
    MsgBody  *_body;
};

#define KWX_INVALID_PCHC -1

#endif