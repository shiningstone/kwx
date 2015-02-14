
#ifndef __MSG_BASIC__
#define __MSG_BASIC__

typedef unsigned char  INT8U;
typedef unsigned short INT16U;
typedef int            INT32U;

#define MSG_MAX_LEN    1024
#define ITEM_BUF_LEN   128
#define ITEM_MAX_NUM   128

class MsgIntf {
public:
    virtual int Serialize(INT8U *outMsg) = 0;
    virtual int Deserialize(const INT8U *inMsg) = 0;
};

#endif