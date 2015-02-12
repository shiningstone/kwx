#include <assert.h>
#include "CTestCase.h"

#include <string.h>

typedef unsigned char INT8U;

#define MSG_MAX_LEN 1024

class UpMsg {
public:
    int Build(INT8U *buf);
private:
    static INT8U _reference[]; 
};

INT8U UpMsg::_reference[] = {
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

int UpMsg::Build(INT8U *buf) {
    memcpy(buf,_reference,sizeof(_reference));
    return sizeof(_reference);
}

class TestUpEmptyPackage : public CTestCase {
public:
    virtual int Execute() {
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

        UpMsg msg;
        len = msg.Build(buf);

        assert(len==sizeof(expect));
        assert(!memcmp(expect,buf,len));

        return OK;
    }
};

void startRun() {
    CTestCase *aCase = new TestUpEmptyPackage();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();
}

