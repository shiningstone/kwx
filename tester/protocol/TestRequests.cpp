
#include "./../../utils/BasicType.h"

#include "./../../protocol/MsgFormats.h"
#include "./../../protocol/KwxMsg.h"
#include "./../../protocol/EnvVariables.h"

/********************************************************************
	TestCases
*********************************************************************/
#include <assert.h>
#include "./../../RaceType.h"
#include "./CTestProtocol.h"

#if 0

class TestGameSendReactionGang : public CTestCase {
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
            0x00,56,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            6,
            131,4,0,1,2,3,         //roomPath:0x00010203
            132,4,4,5,6,7,         //roomId:  0x04050607
            133,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                  //site:    1
            66,2,                  //act:     2 
            135,1,3                //card kind: 3
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        SeatInfo *seat = SeatInfo::getInstance();
        seat->Set(0x00010203,0x04050607,0x08090a0b,1);

        KwxDsMsg aMsg(UP_STREAM);
        len = aMsg.SetReaction(buf,aGANG,TIAO_4);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestGameSendReactionMing1Card : public CTestCase {
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
            0x00,57,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            6,
            131,4,0,1,2,3,         //roomPath:0x00010203
            132,4,4,5,6,7,         //roomId:  0x04050607
            133,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                  //site:    1
            66,6,                  //act:     6 
            135,2,0xff,3           //card kind: 3
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        SeatInfo *seat = SeatInfo::getInstance();
        seat->Set(0x00010203,0x04050607,0x08090a0b,1);

        Card_t mingCard[] = {TIAO_4};
        KwxDsMsg aMsg(UP_STREAM);
        len = aMsg.SetReaction(buf,aMING,1,mingCard);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestGameSendReactionMing2Cards : public CTestCase {
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
            0x00,58,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            6,
            131,4,0,1,2,3,         //roomPath:0x00010203
            132,4,4,5,6,7,         //roomId:  0x04050607
            133,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                  //site:    1
            66,6,                  //act:     6 
            135,3,0xff,3,4         //card kind: 3
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        SeatInfo *seat = SeatInfo::getInstance();
        seat->Set(0x00010203,0x04050607,0x08090a0b,1);

        Card_t mingCard[] = {TIAO_4,TIAO_5};
        KwxDsMsg aMsg(UP_STREAM);
        len = aMsg.SetReaction(buf,aMING,2,mingCard);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

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
            0x00,27,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            3,
            60,1,                  //seatId
            66,2,                  //reaction
            67,3,                  //card kind
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg();
        OthersAction_t actionInfo = {0};

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(actionInfo);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_RECV_RESPONSE );
        assert( aMsg.GetLevel()==7 );
        assert( actionInfo.seat==1 );
        assert( actionInfo.action==2 );
        assert( actionInfo.cardKind==3 );

        return 0;
    }
};
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

        KwxDsMsg aMsg(UP_STREAM);
        len = aMsg.SendAction(buf,PENG);

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

        KwxDsMsg aMsg(UP_STREAM);
        len = aMsg.SetShowCard(buf,0);

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

        KwxDsMsg aMsg(UP_STREAM);
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

        CARD cards[2];
            cards[0].kind      = ck_ER_TIAO;
            cards[0].status    = c_MING_GANG;
            cards[0].can_play  = cps_YES;

            cards[1].kind      = ck_SI_TIAO;
            cards[1].status    = c_MING_KOU;
            cards[1].can_play  = cps_NO;

        KwxDsMsg aMsg(UP_STREAM);
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
            0x00,33,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            4,
            60,1,                  //seat
            61,2,                  //card kind
            62,3,                  //counter
            130,4,0,0,0,4          //reminder
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg();
        DistributeResponse_t response = {0};

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(response);

        assert( len==sizeof(msgInNetwork) );
        assert( aMsg.GetRequestCode()==REQ_GAME_SEND_DIST );
        assert( aMsg.GetLevel()==7 );

        assert( response.seat==1 );
        assert( response.cardKind==2 );
        assert( response.counter==3 );
        assert( response.reminder==4 );
        
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

        KwxDsMsg aMsg();
        OthersAction_t actionInfo = {0};

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(actionInfo);

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

        KwxDsMsg aMsg();
        OthersShowCard_t cardInfo = {0};

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(cardInfo);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_RECV_SHOWCARD );
        assert( aMsg.GetLevel()==7 );
        assert( cardInfo.seat==1 );
        assert( cardInfo.cardKind==2 );

        return 0;
    }
};

#endif

class TestSendAction_peng3tiao : public CTestCase {
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
            0x00,49,               //request code(发送玩家反应 REQ_GAME_SEND_ACTION)
            0x00,65,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            6,
            131,0,4,0,1,2,3,         //roomPath:0x00010203
            132,0,4,4,5,6,7,         //roomId:  0x04050607
            133,0,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                    //site:    1
            134,0,4,0,0,0,1,         //act:     1(碰)                 
            135,0,1,2                //card:    3条
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        SeatInfo *seat = SeatInfo::getInstance();
        seat->Set(0x00010203,0x04050607,0x08090a0b,1);

        RequestSendAction aMsg;
        aMsg.Set(aPENG,TIAO_3);
        len = aMsg.Serialize(buf);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestRecvActionResponse_waitRight : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,49,               //request code
            7,                     //package level
            0x00,25,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            2,
            60,1,                  //seatId
            63,2,                  //wait 2
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        ActionResponse_t waitInfo = {0};

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(waitInfo);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_SEND_ACTION );
        assert( aMsg.GetLevel()==7 );
        assert( waitInfo.seat==1 );
        assert( waitInfo.waitSeat==2 );

        return 0;
    }
};

class TestRecvActionNotif : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,76,               //request code
            7,                     //package level
            0x00,38,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            5,
            60,1,                  //seatId
            61,0,                  //who show card: 0 server
            62,2,                  //next player
            131,0,4,0,0,0,1,       //action : 碰
            132,0,1,2,             //card :   3条
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        ActionNotif_t action = {0};

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(action);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_RECV_ACTION );
        assert( aMsg.GetLevel()==7 );
        assert( action.seat==1 );
        assert( action.isFromServer==true );
        assert( action.next==2 );
        assert( action.actions[0]==a_PENG );
        assert( action.card[0]==TIAO_3 );

        return 0;
    }
};

class TestSendGameStart : public CTestCase {
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
            0x00,43,               //request code(请求开始 REQ_GAME_SEND_START)
            0x00,54,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            4,
            131,0,4,0,1,2,3,         //roomPath:0x00010203
            132,0,4,4,5,6,7,         //roomId:  0x04050607
            133,0,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                    //site:    1
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        SeatInfo *seat = SeatInfo::getInstance();
        seat->Set(0x00010203,0x04050607,0x08090a0b,1);

        RequestGameStart aMsg;
        aMsg.Set();
        len = aMsg.Serialize(buf);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestRecvGameStartResponse : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,43,               //request code
            7,                     //package level
            0x00,28,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            1,
            131,0,4,0,0,0,1,       //score
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        GameStartResponse_t startInfo = {0};

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(startInfo);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_SEND_START );
        assert( aMsg.GetLevel()==7 );
        assert( startInfo.score==1 );

        return 0;
    }
};

class TestRecvOthersGameStart : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,77,               //request code
            7,                     //package level
            0x00,30,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            2,
            60,2,                  //other's seat
            131,0,4,0,0,0,1,       //score
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        GameStartNotif_t startInfo = {0};

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(startInfo);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_RECV_START );
        assert( aMsg.GetLevel()==7 );
        assert( startInfo.seat==2 );
        assert( startInfo.score==1 );

        return 0;
    }
};

class TestSendShowCard : public CTestCase {
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
            0x00,48,               //request code(发送玩家出牌 REQ_GAME_SEND_SHOWCARD)
            0x00,56,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            5,
            131,0,4,0,1,2,3,         //roomPath:0x00010203
            132,0,4,4,5,6,7,         //roomId:  0x04050607
            133,0,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                    //site:    1
            65,2,                    //show card : 3条
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        SeatInfo *seat = SeatInfo::getInstance();
        seat->Set(0x00010203,0x04050607,0x08090a0b,1);

        RequestHandout aMsg;
        aMsg.Set(TIAO_3);
        len = aMsg.Serialize(buf);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestRecvShowCardResponse : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,43,               //request code
            7,                     //package level
            0x00,30,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            2,
            60,0,                   //status
            131,0,4,1,2,0,3,       //ting info : 胡2条，剩2张，赢3番
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        HandoutResponse_t handoutInfo;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(handoutInfo);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_SEND_START );
        assert( aMsg.GetLevel()==7 );
        assert( handoutInfo.status==SUCCESS );
        assert( handoutInfo.ting.cards[0].kind==TIAO_2);
        assert( handoutInfo.ting.cards[0].remain==2 );
        assert( handoutInfo.ting.cards[0].fan==3 );

        return 0;
    }
};

class TestRecvOthersShowCard : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,75,               //request code
            7,                     //package level
            0x00,32,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            3,
            60,2,                  //seatId
            65,0,                  //show card:  1条
            131,0,4,9,2,0,3,       //ting info : 胡2条，剩2张，赢3番
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        HandoutNotif_t handoutInfo;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(handoutInfo);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_RECV_SHOWCARD );
        assert( aMsg.GetLevel()==7 );
        assert( handoutInfo.seat==2 );
        assert( handoutInfo.kind==TIAO_1);
        assert( handoutInfo.ting.cards[0].kind==TONG_1);
        assert( handoutInfo.ting.cards[0].remain==2 );
        assert( handoutInfo.ting.cards[0].fan==3 );

        return 0;
    }
};

class TestRecvDistCard_MingGang4tiao : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,51,               //request code/*发牌(下行) REQ_GAME_DIST_CARD*/
            7,                     //package level
            0x00,46,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            8,
            60,0,                  //seat
            61,1,                  //timer
            62,2,                  //reserved card num
            63,3,                  //card:               发4条
            64,2,                  //remind:             明杠
            130,0,1,3,             //gang remind:        可杠4条
            131,0,1,0xff,          //kou remind:         不可扣
            132,0,4,0xff,0xff,0xff,0xff,  //ming remind: 不可明
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        DistCardInfo_t dist;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(dist);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_DIST_CARD );
        assert( aMsg.GetLevel()==7 );
        assert( dist.seat==0 );
        assert( dist.timer==1 );
        assert( dist.remain==2 );
        assert( dist.kind==TIAO_4 );
        assert( dist.remind.actions[0]==a_MING_GANG );
        assert( dist.remind.gangCard[0]==TIAO_4 );
        assert( dist.remind.kouCard[0]==CARD_UNKNOWN);
        assert( dist.remind.ming.choiceNum==0 );

        return 0;
    }
};

class TestRecvDistCard_MingOut1Ting2 : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,51,               //request code/*发牌(下行) REQ_GAME_DIST_CARD*/
            7,                     //package level
            0x00,54,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            8,
            60,0,                  //seat
            61,1,                  //timer
            62,2,                  //reserved card num
            63,3,                  //card:               发4条
            64,0x40,               //remind:             明
            130,0,1,0xff,          //gang remind:        不可杠
            131,0,1,0xff,          //kou remind:         不可扣
            132,0,12,
                0,0,4,3,           //ming remind:        出5条可胡3张
                5,1,0,2,           //                    胡6条，剩1张，赢2番
                6,2,0,4,           //                    胡7条，剩2张，赢4番
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        DistCardInfo_t dist;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(dist);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_DIST_CARD );
        assert( aMsg.GetLevel()==7 );
        assert( dist.seat==0 );
        assert( dist.timer==1 );
        assert( dist.remain==2 );
        assert( dist.kind==TIAO_4 );
        assert( dist.remind.actions[0]==a_MING );
        assert( dist.remind.gangCard[0]==CARD_UNKNOWN );
        assert( dist.remind.kouCard[0]==CARD_UNKNOWN );
        
        assert( dist.remind.ming.choiceNum==1 );
        assert( dist.remind.ming.handouts[0].kind==TIAO_5 );
        assert( dist.remind.ming.handouts[0].ting.cards[0].kind==TIAO_6 );
        assert( dist.remind.ming.handouts[0].ting.cards[0].remain==1 );
        assert( dist.remind.ming.handouts[0].ting.cards[0].fan==2 );
        assert( dist.remind.ming.handouts[0].ting.cards[1].kind==TIAO_7 );
        assert( dist.remind.ming.handouts[0].ting.cards[1].remain==2 );
        assert( dist.remind.ming.handouts[0].ting.cards[1].fan==4 );

        return 0;
    }
};

class TestRecvDistCard_MingOut2Ting3 : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,51,               //request code/*发牌(下行) REQ_GAME_DIST_CARD*/
            7,                     //package level
            0x00,62,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            8,
            60,0,                  //seat
            61,1,                  //timer
            62,2,                  //reserved card num
            63,3,                  //card:               发4条
            64,0x40,               //remind:             明
            130,0,1,0xff,          //gang remind:        不可杠
            131,0,1,0xff,          //kou remind:         不可扣
            132,0,20,
                0,0,4,3,           //ming remind:        出5条可胡3张
                5,1,0,2,           //                    胡6条，剩1张，赢2番
                6,2,0,4,           //                    胡7条，剩2张，赢4番
                0,0,7,1,           //ming remind:        出8条可胡1张
                8,1,0,9,           //                    胡9条，剩1张，赢9番
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        DistCardInfo_t dist;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(dist);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_DIST_CARD );
        assert( aMsg.GetLevel()==7 );
        assert( dist.seat==0 );
        assert( dist.timer==1 );
        assert( dist.remain==2 );
        assert( dist.kind==TIAO_4 );
        assert( dist.remind.actions[0]==a_MING );
        assert( dist.remind.gangCard[0]==CARD_UNKNOWN );
        assert( dist.remind.kouCard[0]==CARD_UNKNOWN );
        
        assert( dist.remind.ming.choiceNum==2 );
        assert( dist.remind.ming.handouts[0].kind==TIAO_5 );
        assert( dist.remind.ming.handouts[0].ting.cards[0].kind==TIAO_6 );
        assert( dist.remind.ming.handouts[0].ting.cards[0].remain==1 );
        assert( dist.remind.ming.handouts[0].ting.cards[0].fan==2 );
        assert( dist.remind.ming.handouts[0].ting.cards[1].kind==TIAO_7 );
        assert( dist.remind.ming.handouts[0].ting.cards[1].remain==2 );
        assert( dist.remind.ming.handouts[0].ting.cards[1].fan==4 );
        assert( dist.remind.ming.handouts[1].kind==TIAO_8 );
        assert( dist.remind.ming.handouts[1].ting.cards[0].kind==TIAO_9 );
        assert( dist.remind.ming.handouts[1].ting.cards[0].remain==1 );
        assert( dist.remind.ming.handouts[1].ting.cards[0].fan==9 );

        return 0;
    }
};

class TestRecvDistCardNotif : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,52,               //request code/*发牌(下行) REQ_GAME_DIST_CARD_TOOTHER*/
            7,                     //package level
            0x00,39,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            4,
            60,0,                  //seat
            61,1,                  //reserved card num
            62,2,                  //timer
            63,3,                  //card:               发4条
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        DistCardNotif_t dist;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(dist);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_DIST_CARD_TOOTHER );
        assert( aMsg.GetLevel()==7 );
        assert( dist.seat==0 );
        assert( dist.remain==1 );
        assert( dist.timer==2 );
        assert( dist.kind==TIAO_4 );

        return 0;
    }
};

class TestRecvScoreNotif : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,53,               //request code/*下发计分结算(下行) REQ_GAME_SEND_CALSCORE*/
            7,                     //package level
            0x00,42,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            2,
            131,0,3,1,2,3,                     //seat[]
            135,0,12,0,0,0,1,0,0,0,2,0,0,0,3   //score[]
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        ScoreNotif_t score;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(score);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_SEND_CALSCORE );
        assert( aMsg.GetLevel()==7 );
        assert( score.seat[0]==1 );
        assert( score.seat[1]==2 );
        assert( score.seat[2]==3 );
        assert( score.val[0]==1 );
        assert( score.val[1]==2 );
        assert( score.val[2]==3 );

        return 0;
    }
};

class TestRecvRemindNotif : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,55,               //request code/*下发提醒(下行) REQ_GAME_DIST_REMIND*/
            7,                     //package level
            0x00,42,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            7,
            60,1,                        //seat
            61,2,                        //timer
            129,0,4,0,0,0,1,             //remind :      碰
            130,0,1,0xff,                //gang remind : 不可杠
            131,0,1,0xff,                //kou remind :  不可扣
            132,0,4,0xff,0xff,0xff,0xff, //ming remind : 不可明
            63,0,                        //wait :        不等
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        RemindInfo_t remind;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(remind);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_DIST_REMIND );
        assert( aMsg.GetLevel()==7 );
        assert( remind.seat==1 );
        assert( remind.timer==2 );
        assert( remind.remind.actions[0]==a_PENG );
        assert( remind.remind.gangCard[0]==CARD_UNKNOWN );
        assert( remind.remind.kouCard[0]==CARD_UNKNOWN );
        assert( remind.remind.ming.choiceNum==0 );
        assert( remind.wait==0 );

        return 0;
    }
};

class TestRecvFirstDistZhuangNotif : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,56,               //request code/*开局发牌(下行) REQ_GAME_DIST_BEGINCARDS*/
            7,                     //package level
            0x00,66,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            8,
            60,0,                  //seat
            61,1,                  //reserved card num
            62,2,                  //timer
            128,0,14,              //cards
                10,10,10,10,0,   
                1,2,3,4,5, 
                6,7,8,9,
            129,0,4,0,0,0,4,       //remind:             暗杠
            130,0,1,10,             //gang remind:       可杠2筒
            131,0,1,0xff,          //kou remind:         不可扣
            132,0,4,0xff,0xff,0xff,0xff,  //ming remind: 不可明
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        FirstDistZhuang_t dist;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(dist);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_DIST_BEGINCARDS );
        assert( aMsg.GetLevel()==7 );
        assert( dist.seat==0 );
        assert( dist.remain==1 );
        assert( dist.timer==2 );
        assert( dist.remind.actions[0]==a_AN_GANG );
        assert( dist.remind.gangCard[0]==TONG_2 );
        assert( dist.remind.kouCard[0]==CARD_UNKNOWN);
        assert( dist.remind.ming.choiceNum==0 );

        return 0;
    }
};

class TestRecvFirstDistNonZhuangNotif : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,57,               //request code/*非庄家开局发牌(下行) REQ_GAME_DIST_BEGINCARDS_OTHER*/
            7,                     //package level
            0x00,45,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            5,
            60,0,                  //seat
            61,1,                  //reserved card num
            128,0,13,              //cards
                10,10,10,10,0,   
                1,2,3,4,5, 
                6,7,8,
            62,2,                  //zhuang
            63,3,                  //timer
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        FirstDistNonZhuang_t dist;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(dist);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_DIST_BEGINCARDS_OTHER );
        assert( aMsg.GetLevel()==7 );
        assert( dist.seat==0 );
        assert( dist.remain==1 );
        assert( dist.zhuang==2 );
        assert( dist.timer==3 );
 
        return 0;
    }
};

class TestRecvActionDecisionNotif : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,59,               //request code/*下发最终动作(下行) REQ_GAME_DIST_DECISION*/
            7,                     //package level
            0x00,45,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            5,
            60,0,                  //seat
            61,1,                  //whoGive
            62,2,                  //next
            131,0,4,0,0,0,1,       //action:  碰   
            132,0,1,0              //card:    1条
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        DecisionNotif_t decision;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(decision);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_DIST_DECISION );
        assert( aMsg.GetLevel()==7 );
        assert( decision.seat==0 );
        assert( decision.whoGive==1 );
        assert( decision.next==2 );
        assert( decision.actions[0]==a_PENG );
		assert( decision.card==TIAO_1);
 
        return 0;
    }
};

class TestSendGetTingInfo : public CTestCase {
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
            0x00,50,               //request code(获取停牌信息 REQ_GAME_GET_TINGINFO)
            0x00,54,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            4,
            131,0,4,0,1,2,3,         //roomPath:0x00010203
            132,0,4,4,5,6,7,         //roomId:  0x04050607
            133,0,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                    //site:    1
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        SeatInfo *seat = SeatInfo::getInstance();
        seat->Set(0x00010203,0x04050607,0x08090a0b,1);

        RequestTingInfo aMsg;
        aMsg.Set();
        len = aMsg.Serialize(buf);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestRecvTingInfoResponse : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,50,               //request code
            7,                     //package level
            0x00,28,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            1,
            131,0,4,1,2,0,3,        //ting info : 胡2条，剩2张，赢3番
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg aMsg;
        MsgTingInfo_t ting;

        len = aMsg.Deserialize(msgInNetwork);
        aMsg.Construct(ting);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg.GetRequestCode()==REQ_GAME_GET_TINGINFO );
        assert( aMsg.GetLevel()==7 );
        assert( ting.cards[0].kind==TIAO_2);
        assert( ting.cards[0].remain==2 );
        assert( ting.cards[0].fan==3 );

        return 0;
    }
};

void testRequests() {
	CTestCase *aCase;

    aCase = new TestSendGameStart();
    aCase->Execute();
    aCase = new TestRecvGameStartResponse();
    aCase->Execute();
    aCase = new TestRecvOthersGameStart();
    aCase->Execute();


    aCase = new TestSendShowCard();
    aCase->Execute();
    aCase = new TestRecvShowCardResponse();
    aCase->Execute();
    aCase = new TestRecvOthersShowCard();
    aCase->Execute();


    aCase = new TestSendAction_peng3tiao();
    aCase->Execute();
    aCase = new TestRecvActionResponse_waitRight();
    aCase->Execute();
    aCase = new TestRecvActionNotif();
    aCase->Execute();

    
    aCase = new TestRecvDistCardNotif();
    aCase->Execute();
    aCase = new TestRecvDistCard_MingOut2Ting3();
    aCase->Execute();
    aCase = new TestRecvDistCard_MingGang4tiao();
    aCase->Execute();
    aCase = new TestRecvDistCard_MingOut1Ting2();
    aCase->Execute();

    aCase = new TestRecvScoreNotif();
    aCase->Execute();

    aCase = new TestRecvRemindNotif();
    aCase->Execute();
    
    aCase = new TestRecvFirstDistZhuangNotif();
    aCase->Execute();

    aCase = new TestRecvFirstDistNonZhuangNotif();
    aCase->Execute();

    aCase = new TestRecvActionDecisionNotif();
    aCase->Execute();

    aCase = new TestSendGetTingInfo();
    aCase->Execute();
    
    aCase = new TestRecvTingInfoResponse();
    aCase->Execute();
}
