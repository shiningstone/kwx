
#include "./../../protocol/KwxMsg.h"

/********************************************************************
	TestCases
*********************************************************************/
#include <assert.h>
#include "./CTestProtocol.h"

class TestGameSendAction : public CTestCase {
public:
    virtual int Execute() {
        const INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x05,                  //language id
            0x06,                  //client platform
            0x07,                  //client build number
            0x08,0x09,             //customer id
            0x0a,0x0b,             //product id
            0x00,47,               //request code(·¢ËÍÍæ¼Ò²Ù×÷)
            0x00,53,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            5,
            131,4,0,1,2,3,         //roomPath:0x00010203
            132,4,4,5,6,7,         //roomId:  0x04050607
            133,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                  //site:    1
            67,1,                  //act:     1(Åö)
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxMsg aMsg(UP_STREAM);
        aMsg.SetRequestCode(REQ_GAME_SEND_ACTION);
        aMsg.AddRoomPath(0x00010203);
        aMsg.AddRoomId(0x04050607);
        aMsg.AddTableId(0x08090a0b);
        aMsg.AddSeatId(1);
        aMsg.AddAction(PENG);

        len = aMsg.Serialize(buf);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

void testRequests() {
    CTestCase *aCase = new TestGameSendAction();
    aCase->Execute();
}