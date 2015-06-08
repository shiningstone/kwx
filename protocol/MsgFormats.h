
#ifndef _MSG_FORMATS_
#define _MSG_FORMATS_

#include "./../utils/UtilBasic.h"

#include "MsgIntf.h"

#define ITEM_BUF_LEN   128
#define ITEM_MAX_NUM   128

typedef enum {
    PURE_ID,
    ID_WITH_INT,
    ID_WITH_BUF,
}ItemType_t;

typedef INT8U Item_t;

class Header : public MsgIntf {
public:
    virtual int Serialize(INT8U *outMsg) = 0;
    virtual int Deserialize(const INT8U *inMsg) = 0;

    INT8U    _pchc[3];
    INT16U   _requestCode;
    INT16U   _size;

    static const INT8U PCHC[3];
};

class UpHeader : public Header {
public:
	UpHeader();

    virtual int Serialize(INT8U *outMsg);
    virtual int Deserialize(const INT8U *inMsg);

	INT8U    _protocol;
	INT32U   _userId;
    INT8U    _language;
    INT8U    _platform;
    INT8U    _buildNo;
    INT16U   _customerId;
    INT16U   _productId;

    static const int UP_HEADER_LEN = 30;
private:
    static const int PROTOCOL      = 3;
	static const int USER_ID       = PROTOCOL + 1;
	static const int LANGUAGE      = USER_ID + 4;
	static const int PLATFORM      = LANGUAGE + 1;
	static const int BUILD_NO      = PLATFORM + 1;
	static const int CUSTOMER_ID   = BUILD_NO + 1;
	static const int PRODUCT_ID    = CUSTOMER_ID + 2;
	static const int REQUEST_CODE  = PRODUCT_ID + 2;
public:
	static const int SIZE          = REQUEST_CODE + 2;
};

class DnHeader : public Header{
public:
    virtual int Serialize(INT8U *outMsg);
    virtual int Deserialize(const INT8U *inMsg);

    INT8U    _level;

    static const int DN_HEADER_LEN     = 20;
	static const int REQUEST_CODE  = 3;
    static const int LEVEL         = REQUEST_CODE + 2;
	static const int SIZE          = LEVEL + 1;
};

class Item : public MsgIntf {
public:
	Item();
	Item(Item_t itemId, INT8U value);
	Item(Item_t itemId, INT16U bufLen,INT8U *buf);

    virtual int Serialize(INT8U *outMsg);
    virtual int Deserialize(const INT8U *inMsg);

    ItemType_t  GetIdType() const ;

    Item_t   _id;
    INT8U    _value;
    INT16U   _bufLen;
    INT8U    _buf[ITEM_BUF_LEN];
protected:
    static ItemType_t _IdType(Item_t id);
};

class MsgBody : public MsgIntf {
public:
    MsgBody();
    ~MsgBody();

    virtual int Serialize(INT8U *outMsg);
    virtual int Deserialize(const INT8U *inMsg);

    INT8U    _itemNum;
    Item     *_items[ITEM_MAX_NUM];
};

#endif
