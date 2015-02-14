
#ifndef __MSG_FORMATS__
#define __MSG_FORMATS__


typedef unsigned char  INT8U;
typedef unsigned short INT16U;
typedef int            INT32U;

#define MSG_MAX_LEN    1024
#define ITEM_BUF_LEN   128
#define ITEM_MAX_NUM   128

const INT8U PCHC[3] = {'K','W','X'};

class Msg {
public:
    virtual int Serialize(INT8U *output) = 0;
    virtual int Deserialize(const INT8U *input) = 0;
};

class UpHeader : public Msg{
public:
    virtual int Serialize(INT8U *output);
    virtual int Deserialize(const INT8U *input);

    INT8U    _pchc[3];
	INT8U    _protocol;
	INT32U   _userId;
    INT8U    _language;
    INT8U    _platform;
    INT8U    _buildNo;
    INT16U   _customerId;
    INT16U   _productId;
    INT16U   _requestCode;
    INT16U   _size;

    static const int UP_HEADER_LEN     = 30;
private:
    static const int PROTOCOL      = 3;
	static const int USER_ID       = PROTOCOL + 1;
	static const int LANGUAGE      = USER_ID + 4;
	static const int PLATFORM      = LANGUAGE + 1;
	static const int BUILD_NO      = PLATFORM + 1;
	static const int CUSTOMER_ID   = BUILD_NO + 1;
	static const int PRODUCT_ID    = CUSTOMER_ID + 2;
	static const int REQUEST_CODE  = PRODUCT_ID + 2;
	static const int SIZE          = REQUEST_CODE + 2;
};

class DnHeader : public Msg{
public:
    virtual int Serialize(INT8U *output);
    virtual int Deserialize(const INT8U *input);

    INT8U    _pchc[3];
    INT16U   _requestCode;
    INT8U    _level;
    INT16U   _size;

    static const int DN_HEADER_LEN     = 20;
private:
	static const int REQUEST_CODE  = 3;
    static const int LEVEL         = REQUEST_CODE + 2;
	static const int SIZE          = LEVEL + 1;
};

class Item : public Msg {
public:
    virtual int Serialize(INT8U *output);
    virtual int Deserialize(const INT8U *input);

    INT8U    _id;
    INT8U    _value;
    INT8U    _bufLen;
    INT8U    _buf[ITEM_BUF_LEN];
protected:
    const static int PURE_ID = 0;
    const static int ID_WITH_INT = 1;
    const static int ID_WITH_BUF = 2;

    int _IdType(INT8U id);
};

class MsgBody : public Msg {
public:
    MsgBody();
    ~MsgBody();

    virtual int Serialize(INT8U *output);
    virtual int Deserialize(const INT8U *input);

    INT8U    _itemNum;
    Item     *_items[ITEM_MAX_NUM];
};

#endif
