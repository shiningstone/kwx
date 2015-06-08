
#ifndef _COMMON_MSG_
#define _COMMON_MSG_

#include "./../utils/UtilBasic.h"
#include "./../game/GameType.h"
#include "./../utils/LogManager.h"

#include "KwxMsgBasic.h"
#include "MsgIntf.h"
#include "KwxMsgEnv.h"

class Header;
class MsgBody;
class Item;

class CommonMsg {
public:
    CommonMsg(int dir);
    ~CommonMsg();

    const int _dir;    
    
    Header  *_header;
    MsgBody *_body;

    static SeatInfo *_seatInfo;
    static Logger   *_logger;
};

/****************************************************
    DOWNSTREAM : Basic Message structure
****************************************************/
class DsInstruction;
class DsMsg : public CommonMsg, public DsMsgIntf {
public:
	static DsMsg *getInstance();
	static void      destroyInstance();

    int Dispatch(const INT8U *inMsg,int inLen);
    virtual int Deserialize(const INT8U *inMsg);
    DsInstruction *_GenerateInstruction();    

    RequestId_t GetRequestCode() const;
    int         GetLevel() const;
    INT32U      GetItemValue(int idx) const;
    INT16U      GetItemBufLen(int idx) const;

private:
	static DsMsg *_instance;
    DsMsg();
};

/****************************************************
    UPSTREAM
****************************************************/
class UsMsg : public CommonMsg, public UsMsgIntf {
public:
    virtual int Serialize(INT8U *outMsg);
    virtual void Desc(char *buf) const;
    
protected:
    UsMsg();
    
	void _set_size(INT8U *buf,INT16U len);
	int  _add_item(Item *item);
    int  _add_utf16_string(Item_t id,const INT8U *hostString);

    int SetRequestCode(RequestId_t code);
    int AddSeatInfo();
    
/* this is for test use */
public:
    friend class TestFullUpMsg;
};

#define KWX_INVALID_PCHC          -1
#define KWX_UNSUPPORTED_PARAM     -2
#define KWX_DISMATCH_LENGTH       -3

#endif
