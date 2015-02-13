
#ifdef WIN32/*WINDOWS*/
#include <WinSock2.h>
#else/*LINUX*/
#include <arpa/inet.h>
#endif

#include <string.h>

typedef unsigned char INT8U;
typedef int           INT32U;

#define MSG_MAX_LEN 1024

class UpHeader {
public:
    int Serialize(INT8U *output);
    int Deserialize(const INT8U *input);

	INT8U    _protocol;
	INT32U   _userId;
private:
    static INT8U _reference[]; 

	static const int PROTOCOL = 3;
	static const int USER_ID = PROTOCOL + 1;
};

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
    memcpy(output,_reference,sizeof(_reference));
    return sizeof(_reference);
}

int UpHeader::Deserialize(const INT8U *input) {
	_protocol = input[PROTOCOL];
	_userId = 0x01020304;/*ntohl( *(INT32U *)(input+USER_ID) );*/

    return 0;
}

/********************************************************************
	TestCases
*********************************************************************/
#include <assert.h>
#include "./CTestProtocol.h"

class TestRandomUpHeader : public CTestCase {
public:
    virtual int Execute() {
		//组包测试
        INT8U expect[] = {
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
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        UpHeader msg;
        len = msg.Serialize(buf);

        assert(len==sizeof(expect));
        assert(!memcmp(expect,buf,len));

		//解包测试
        INT8U msgBuf[] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x01,                  //language id
            0x01,                  //client platform
            0x00,                  //client build number
            0x00,0x00,             //customer id
            0x00,0x00,             //product id
            0x00,0x00,             //request code
            0x00,0x12,             //package size
        };

		msg.Deserialize(msgBuf);
		assert(msg._protocol==16);
		assert(msg._userId==0x01020304);

		
		return OK;
    }
};

/********************************************************************
	Main entrance
*********************************************************************/
void startRun() {
    CTestCase *aCase = new TestRandomUpHeader();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();
}

