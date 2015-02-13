
#include "./../../protocol/MsgFormats.h"

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

class TestDnHeader : public CTestCase {
public:
    virtual int Execute() {
		//解包测试
        const INT8U msgInNetwork[DnHeader::DN_HEADER_LEN] = {
            'K','W','X',           //KWX
            0x01,0x02,             //request code
            0x03,                  //level
            0x04,0x05,             //package size
            0,                     //reserved
        };
        DnHeader aHeader;

		aHeader.Deserialize(msgInNetwork);
		assert(aHeader._requestCode==0x0102);
		assert(aHeader._level==0x03);
		assert(aHeader._size==0x0405);
		
		//组包测试
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DnHeader bHeader;
		bHeader._requestCode=0x0102;
        bHeader._level=0x03;
        bHeader._size=0x0405;

        len = bHeader.Serialize(buf);

        assert(len==20);
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

    aCase = new TestDnHeader();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();
}

