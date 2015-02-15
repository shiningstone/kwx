
#include "./../../protocol/MsgFormats.h"
#include "./../../protocol/KwxMsg.h"
#include "./../../protocol/EnvVariables.h"

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
            0x00,47,               //request code(发送玩家操作)
            0x00,53,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            5,
            131,4,0,1,2,3,         //roomPath:0x00010203
            132,4,4,5,6,7,         //roomId:  0x04050607
            133,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                  //site:    1
            67,1,                  //act:     1(碰)
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        SeatInfo *seat = SeatInfo::getInstance();
        seat->Set(0x00010203,0x04050607,0x08090a0b,1);

        KwxMsg aMsg(UP_STREAM);
        aMsg.SetAction(PENG);

        len = aMsg.Serialize(buf);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestGameSendShowCard : public CTestCase {
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
            0x00,48,               //request code(发送玩家出牌)
            0x00,53,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            5,
            131,4,0,1,2,3,         //roomPath:0x00010203
            132,4,4,5,6,7,         //roomId:  0x04050607
            133,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                  //site:    1
            65,0,                  //card:    0
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        SeatInfo *seat = SeatInfo::getInstance();
        seat->Set(0x00010203,0x04050607,0x08090a0b,1);

        KwxMsg aMsg(UP_STREAM);
        aMsg.SetShowCard(0);

        len = aMsg.Serialize(buf);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestGameSendDistributeRequest : public CTestCase {
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
            0x00,50,               //request code(发送发牌请求)
            0x00,51,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            4,
            131,4,0,1,2,3,         //roomPath:0x00010203
            132,4,4,5,6,7,         //roomId:  0x04050607
            133,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                  //site:    1
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        SeatInfo *seat = SeatInfo::getInstance();
        seat->Set(0x00010203,0x04050607,0x08090a0b,1);

        KwxMsg aMsg(UP_STREAM);
        aMsg.SetRequestDistribute();

        len = aMsg.Serialize(buf);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestGameRecvDistributeResponse : public CTestCase {
public:
    virtual int Execute() {
        const INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,50,               //request code(发送发牌请求)
            7,                     //package level
            0x00,27,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            3,
            60,1,                  //roomId
            61,2,                  //seat
            70,0,                  //card kind
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxMsg aMsg(DOWN_STREAM);
        len = aMsg.Deserialize(msgInNetwork);

        DnHeader *aHeader = static_cast<DnHeader *>(aMsg._header);        
        assert(len==sizeof(msgInNetwork));
        assert(aHeader->_requestCode==REQ_GAME_SEND_DIST);
        assert(aHeader->_level==7);
        assert(aMsg._body->_itemNum==3);
            assert(aMsg._body->_items[0]->_id==60);
            assert(aMsg._body->_items[0]->_value==1);
            assert(aMsg._body->_items[1]->_id==61);
            assert(aMsg._body->_items[1]->_value==2);
            assert(aMsg._body->_items[2]->_id==70);
            assert(aMsg._body->_items[2]->_value==0);

        return 0;
    }
};

void testRequests() {
    CTestCase *aCase = new TestGameSendAction();
    aCase->Execute();

    aCase = new TestGameSendShowCard();
    aCase->Execute();

    aCase = new TestGameSendDistributeRequest();
    aCase->Execute();

    aCase = new TestGameRecvDistributeResponse();
    aCase->Execute();
}
