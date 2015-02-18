
#include "./../../protocol/MsgFormats.h"
#include "./../../protocol/KwxMsg.h"
#include "./../../protocol/EnvVariables.h"

/********************************************************************
	TestCases
*********************************************************************/
#include <assert.h>
#include "./../../RaceType.h"
#include "./CTestProtocol.h"

class TestGameSendAction : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
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
        len = aMsg.SetAction(buf,PENG);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestGameSendShowCard : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
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
        len = aMsg.SetShowCard(buf,0);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestGameSendReaction : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x05,                  //language id
            0x06,                  //client platform
            0x07,                  //client build number
            0x08,0x09,             //customer id
            0x0a,0x0b,             //product id
            0x00,49,               //request code(发送玩家反应)
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
        len = aMsg.SetReaction(buf,PENG);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestGameSendDistributeRequest : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
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
        len = aMsg.SetRequestDistribute(buf);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestGameSendUpdateList : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x05,                  //language id
            0x06,                  //client platform
            0x07,                  //client build number
            0x08,0x09,             //customer id
            0x0a,0x0b,             //product id
            0x00,52,               //request code(发送更新牌队列)
            0x00,61,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            5,
            131,4,0,1,2,3,         //roomPath:0x00010203
            132,4,4,5,6,7,         //roomId:  0x04050607
            133,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                  //site:    1
            135,8,  
                0,1,2,1,           //一张牌：0-reserved；1-ck_ER_TIAO；2-c_MING_GANG；0-cps_YES
                0,3,4,0,           //一张牌：0-reserved；3-ck_SI_TIAO；4-c_MING_KOU ；0-cps_NO
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        SeatInfo *seat = SeatInfo::getInstance();
        seat->Set(0x00010203,0x04050607,0x08090a0b,1);

        CARD cards[2];
            cards[0].kind      = ck_ER_TIAO;
            cards[0].status    = c_MING_GANG;
            cards[0].can_play  = cps_YES;

            cards[1].kind      = ck_SI_TIAO;
            cards[1].status    = c_MING_KOU;
            cards[1].can_play  = cps_NO;

        KwxMsg aMsg(UP_STREAM);
        len = aMsg.SetUpdateCardList(buf,cards,sizeof(cards)/sizeof(CARD));

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestGameRecvDistributeResponse : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
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
        DistributeResponse_t response = {0};

        len = aMsg.Construct(response,msgInNetwork);

        assert( len==sizeof(msgInNetwork) );
        assert( aMsg.GetRequestCode()==REQ_GAME_SEND_DIST );
        assert( aMsg.GetLevel()==7 );

        assert( response.room==1 );
        assert( response.seat==2 );
        assert( response.cardKind==0 );
        
        return 0;
    }
};

class TestGameRecvOthersAction : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,74,               //request code(下发其他玩家动作)
            7,                     //package level
            0x00,25,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            2,
            60,1,                  //seatId
            67,2,                  //action
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxMsg aMsg(DOWN_STREAM);
        OthersAction_t actionInfo = {0};

        len = aMsg.Construct(actionInfo,msgInNetwork);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_RECV_ACTION );
        assert( aMsg.GetLevel()==7 );
        assert( actionInfo.seat==1 );
        assert( actionInfo.action==2 );

        return 0;
    }
};


class TestGameRecvOthersShowCard : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,75,               //request code(下发其他玩家出牌)
            7,                     //package level
            0x00,25,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            2,
            60,1,                  //seatId
            65,2,                  //card kind
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxMsg aMsg(DOWN_STREAM);
        OthersShowCard_t cardInfo = {0};

        len = aMsg.Construct(cardInfo,msgInNetwork);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_RECV_SHOWCARD );
        assert( aMsg.GetLevel()==7 );
        assert( cardInfo.seat==1 );
        assert( cardInfo.cardKind==2 );

        return 0;
    }
};


class TestGameRecvOthersReaction : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,76,               //request code(下发其他玩家反应)
            7,                     //package level
            0x00,25,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            2,
            60,1,                  //seatId
            65,2,                  //reaction
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxMsg aMsg(DOWN_STREAM);
        OthersAction_t actionInfo = {0};

        len = aMsg.Construct(actionInfo,msgInNetwork);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_RECV_RESPONSE );
        assert( aMsg.GetLevel()==7 );
        assert( actionInfo.seat==1 );
        assert( actionInfo.action==2 );

        return 0;
    }
};

void testRequests() {
    CTestCase *aCase = new TestGameSendAction();
    aCase->Execute();

    aCase = new TestGameSendShowCard();
    aCase->Execute();

    aCase = new TestGameSendReaction();
    aCase->Execute();

    aCase = new TestGameSendDistributeRequest();
    aCase->Execute();

    aCase = new TestGameRecvDistributeResponse();
    aCase->Execute();

    aCase = new TestGameSendUpdateList();
    aCase->Execute();

    aCase = new TestGameRecvOthersAction();
    aCase->Execute();

    aCase = new TestGameRecvOthersShowCard();
    aCase->Execute();

    aCase = new TestGameRecvOthersReaction();
    aCase->Execute();
}
