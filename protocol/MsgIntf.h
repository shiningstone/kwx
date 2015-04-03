
#ifndef _MSG_INTF_
#define _MSG_INTF_

#include "./../utils/UtilBasic.h"

class UsMsgIntf {
public:
    virtual int Serialize(INT8U *outMsg) = 0;
};

class DsMsgIntf {
public:
    virtual int Deserialize(const INT8U *inMsg) = 0;
};

class MsgIntf : public UsMsgIntf, public DsMsgIntf {
public:
    virtual int Serialize(INT8U *outMsg) = 0;
    virtual int Deserialize(const INT8U *inMsg) = 0;
};

#endif