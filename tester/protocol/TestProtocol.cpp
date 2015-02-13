
#include <string.h>

typedef unsigned char  INT8U;
typedef unsigned short INT16U;
typedef int            INT32U;

#define MSG_MAX_LEN 1024

INT32U _ntohl(INT32U n) {
    char *p = (char *)&n;
    char ha[4] = {0};

    ha[0] = p[3];
    ha[1] = p[2];
    ha[2] = p[1];
    ha[3] = p[0];

    return *((INT32U *)ha);
}

INT16U _ntohs(INT16U n) {
    char *p = (char *)&n;
    char ha[2] = {0};

    ha[0] = p[1];
    ha[1] = p[0];

    return *((INT16U *)ha);
}

class UpHeader {
public:
    int Serialize(INT8U *output);
    int Deserialize(const INT8U *input);

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
    static INT8U _reference[]; 

    static const INT8U PCHC[3];
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

const INT8U UpHeader::PCHC[3] = {'K','W','X',};

INT8U UpHeader::_reference[] = {
    'K','W','X',           //KWX
    0x10,                  //protocol version
    0x00,0x00,0x00,0x00,   //user id
    0x01,                  //language id
    0x01,                  //client platform
    0x00,                  //client build number
    0x00,0x00,             //customer id
    0x00,0x00,             //product id
    0x00,0x00,             //request code
    0x00,0x12,             //package size
};

int UpHeader::Serialize(INT8U *output) {
    memcpy(output,PCHC,3);
    output[PROTOCOL] = _protocol;
    *((INT32U *)(output+USER_ID)) = _ntohl(_userId);    
    output[LANGUAGE] = _language;
    output[PLATFORM] = _platform;
    output[BUILD_NO] = _buildNo;
    *((INT16U *)(output+CUSTOMER_ID)) = _ntohs(_customerId);
    *((INT16U *)(output+PRODUCT_ID)) = _ntohs(_productId);
    *((INT16U *)(output+REQUEST_CODE)) = _ntohs(_requestCode);
    *((INT16U *)(output+SIZE)) = _ntohs(_size);

    memset(output+19,0,UP_HEADER_LEN-19);

    return UP_HEADER_LEN;
}

int UpHeader::Deserialize(const INT8U *input) {
	_protocol     = input[PROTOCOL];
	_userId       = _ntohl( *(INT32U *)(input+USER_ID) );
    _language     = *(input+LANGUAGE);
    _platform     = *(input+PLATFORM);
    _buildNo      = *(input+BUILD_NO);
    _customerId   = _ntohs( *(INT16U *)(input+CUSTOMER_ID) );
    _productId    = _ntohs( *(INT16U *)(input+PRODUCT_ID) );
    _requestCode  = _ntohs( *(INT16U *)(input+REQUEST_CODE) );
    _size         = _ntohs( *(INT16U *)(input+SIZE) );

    return UP_HEADER_LEN; 
}

/********************************************************************
	TestCases
*********************************************************************/
#include <assert.h>
#include "./CTestProtocol.h"

class TestUpHeader : public CTestCase {
public:
    virtual int Execute() {
		//解包测试
        const INT8U msgInNetwork[UpHeader::UP_HEADER_LEN] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x05,                  //language id
            0x06,                  //client platform
            0x07,                  //client build number
            0x08,0x09,             //customer id
            0x0a,0x0b,             //product id
            0x0c,0x0d,             //request code
            0x0e,0x0f,             //package size
            0,                     //reserved
        };
        UpHeader aHeader;

		aHeader.Deserialize(msgInNetwork);
		assert(aHeader._protocol==0x10);
		assert(aHeader._userId==0x01020304);
		assert(aHeader._customerId==0x0809);
		assert(aHeader._productId==0x0a0b);
		assert(aHeader._requestCode==0x0c0d);
        assert(aHeader._size==0x0e0f);
		
		//组包测试
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        UpHeader bHeader;
		bHeader._protocol=16;
		bHeader._userId=0x01020304;
		bHeader._language=0x05;
		bHeader._platform=0x06;
		bHeader._buildNo=0x07;
		bHeader._customerId=0x0809;
		bHeader._productId=0x0a0b;
		bHeader._requestCode=0x0c0d;
        bHeader._size=0x0e0f;

        len = bHeader.Serialize(buf);

        assert(len==30);
        assert(!memcmp(msgInNetwork,buf,len));

		return OK;
    }
};

/********************************************************************
	Main entrance
*********************************************************************/
void startRun() {
    CTestCase *aCase = new TestUpHeader();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();
}

