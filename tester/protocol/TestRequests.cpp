
#include "./../../game/GameType.h"
#include "./../../network/KwxEnv.h"
#include "./../../protocol/RequestStructs.h"
#include "./../../protocol/CommonMsg.h"
#include "./../../protocol/DsInstruction.h"
#include "./../../protocol/UsRequest.h"
#include "./../../protocol/KwxMsgLogin.h"
#include "./../../network/KwxMessenger.h"

/********************************************************************
	TestCases
*********************************************************************/
#include <assert.h>
#include "./../../RaceType.h"
#include "./../CTestCase.h"

/*********************************************
    牌局动作REQUEST
*********************************************/
class TestSendGameStart : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x05,                  //language id
            0x06,                  //client platform
            BUILD_NUMBER,          //client build number
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

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        GameStartResponse startInfo;
		startInfo.Construct(*aMsg);
        
        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_SEND_START );
        assert( aMsg->GetLevel()==7 );
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

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);
        
        GameStartNotif startInfo;
		startInfo.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_RECV_START );
        assert( aMsg->GetLevel()==7 );
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
            BUILD_NUMBER,          //client build number
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

        RequestShowCard aMsg;
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
            0x00,48,               //request code
            7,                     //package level
            0x00,30,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            2,
            60,0,                  //status
            131,0,4,1,2,0,3,       //ting info : 胡2条，剩2张，赢3番
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        ShowCardResponse handoutInfo;
        handoutInfo.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_SEND_SHOWCARD );
        assert( aMsg->GetLevel()==7 );
        assert( handoutInfo.status==SUCCESS );
        assert( handoutInfo.ting.cards[0].kind==TIAO_2);
        assert( handoutInfo.ting.cards[0].remain==2 );
        assert( handoutInfo.ting.cards[0].fan==3 );

        return 0;
    }
};

class TestRecvShowCardNotif : public CTestCase {
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

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        ShowCardNotif handoutInfo;
        handoutInfo.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_RECV_SHOWCARD );
        assert( aMsg->GetLevel()==7 );
        assert( handoutInfo.seat==2 );
        assert( handoutInfo.kind==TIAO_1);
        assert( handoutInfo.ting.cards[0].kind==TONG_1);
        assert( handoutInfo.ting.cards[0].remain==2 );
        assert( handoutInfo.ting.cards[0].fan==3 );

        return 0;
    }
};

class TestSendAction_peng3tiao : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x05,                  //language id
            0x06,                  //client platform
            BUILD_NUMBER,          //client build number
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

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        ActionResponse waitInfo;
        waitInfo.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_SEND_ACTION );
        assert( aMsg->GetLevel()==7 );
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
            61,2,                  //who show card
            62,2,                  //next player
            131,0,4,0,0,0,1,       //action : 碰
            132,0,1,2,             //card :   3条
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        ActionNotif action;
        action.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_RECV_ACTION );
        assert( aMsg->GetLevel()==7 );
        assert( action.seat==1 );
        assert( action.whoGive==2 );
        assert( action.next==2 );
        assert( action.actions==a_PENG );
        assert( action.card[0].kind==TIAO_3 );

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
            0x00,51,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            8,
            60,1,                  //seat/*1,2,3,0-invalid*/
            61,1,                  //timer
            62,2,                  //reserved card num
            63,3,                  //card:               发4条
            129,0,4,0,0,0,2,       //remind:             明杠
            130,0,1,3,             //gang remind:        可杠4条
            131,0,1,0xff,          //kou remind:         不可扣
            132,0,4,0xff,0xff,0xff,0xff,  //ming remind: 不可明
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        DistCardInfo dist;
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_CARD );
        assert( aMsg->GetLevel()==7 );
        assert( dist.seat==MIDDLE );
        assert( dist.timer==1 );
        assert( dist.remain==2 );
        assert( dist.kind==TIAO_4 );
        assert( dist.remind.actions==a_MING_GANG );
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
            0x00,59,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            8,
            60,1,                  //seat
            61,1,                  //timer
            62,2,                  //reserved card num
            63,3,                  //card:               发4条
            129,0,4,0,0,0,8,       //remind:             明
            130,0,1,0xff,          //gang remind:        不可杠
            131,0,1,0xff,          //kou remind:         不可扣
            132,0,12,
                0,0,4,3,           //ming remind:        出5条可胡3张
                5,1,0,2,           //                    胡6条，剩1张，赢2番
                6,2,0,4,           //                    胡7条，剩2张，赢4番
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        DistCardInfo dist;
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_CARD );
        assert( aMsg->GetLevel()==7 );
        assert( dist.seat==MIDDLE );
        assert( dist.timer==1 );
        assert( dist.remain==2 );
        assert( dist.kind==TIAO_4 );
        assert( dist.remind.actions==a_MING );
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
            0x00,75,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            8,
            60,1,                  //seat
            61,1,                  //timer
            62,2,                  //reserved card num
            63,3,                  //card:               发4条
            129,0,4,0,0,0,8,       //remind:             明
            130,0,1,0xff,          //gang remind:        不可杠
            131,0,1,0xff,          //kou remind:         不可扣
            132,0,28,
                0,0,4,3,           //ming remind:        出5条可胡3张
                5,1,0,2,           //                    胡6条，剩1张，赢2番
                6,2,0,4,           //                    胡7条，剩2张，赢4番
                0xff,0xff,0xff,0xff,
                0,0,7,1,           //ming remind:        出8条可胡1张
                8,1,0,9,           //                    胡9条，剩1张，赢9番
                0xff,0xff,0xff,0xff,
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        DistCardInfo dist;
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_CARD );
        assert( aMsg->GetLevel()==7 );
        assert( dist.seat==MIDDLE );
        assert( dist.timer==1 );
        assert( dist.remain==2 );
        assert( dist.kind==TIAO_4 );
        assert( dist.remind.actions==a_MING );
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
            0x00,29,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            4,
            60,2,                  //seat
            61,1,                  //reserved card num
            62,2,                  //timer
            63,3,                  //card:               发4条
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

		DistCardNotif dist;
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_CARD_TOOTHER );
        assert( aMsg->GetLevel()==7 );
        assert( dist.seat==RIGHT );
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

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

		ScoreNotif score;
        score.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_SEND_CALSCORE );
        assert( aMsg->GetLevel()==7 );
        assert( score.seat[0]==MIDDLE );
        assert( score.seat[1]==RIGHT );
        assert( score.seat[2]==LEFT );
        assert( score.val[MIDDLE]==1 );
        assert( score.val[RIGHT]==2 );
        assert( score.val[LEFT]==3 );

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
            0x00,53,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            9,
            60,1,                        //seat
            61,2,                        //timer
            62,3,                        //kind :        4条
            63,3,                        //whogive
            129,0,4,0,0,0,1,             //remind :      碰
            130,0,1,0xff,                //gang remind : 不可杠
            131,0,1,0xff,                //kou remind :  不可扣
            132,0,4,0xff,0xff,0xff,0xff, //ming remind : 不可明
            64,2,                        //wait
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

		RemindInfo remind;
        remind.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_REMIND );
        assert( aMsg->GetLevel()==7 );
        assert( remind.seat==1 );
        assert( remind.timer==2 );
        assert( remind.kind==TIAO_4 );
        assert( remind.whoGive==LEFT );
        assert( remind.remind.actions==a_PENG );
        assert( remind.remind.gangCard[0]==CARD_UNKNOWN );
        assert( remind.remind.kouCard[0]==CARD_UNKNOWN );
        assert( remind.remind.ming.choiceNum==0 );
        assert( remind.wait==RIGHT );

        return 0;
    }
};

class TestRecvRemindNotif_Handout1tiaoTing2tiao : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,55,               //request code/*下发提醒(下行) REQ_GAME_DIST_REMIND*/
            7,                     //package level
            0x00,61,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            9,
            60,1,                        //seat
            61,2,                        //timer
            62,3,                        //kind :        4条
            63,3,                        //whogive
            129,0,4,0,0,0,0x38,          //remind :      
            130,0,1,0xff,                //gang remind : 不可杠
            131,0,1,0xff,                //kou remind :  不可扣
            132,0,12,
                0x00,0x00,0x03,0x02,
                0x03,0x02,0x10,0x00,
                0xff,0xff,0xff,0xff,
            64,2,                        //wait
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

		RemindInfo remind;
        remind.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_REMIND );
        assert( aMsg->GetLevel()==7 );
        assert( remind.seat==1 );
        assert( remind.timer==2 );
        assert( remind.kind==TIAO_4 );
        assert( remind.whoGive==LEFT );
        assert( remind.remind.actions&aMING );
        assert( remind.remind.gangCard[0]==CARD_UNKNOWN );
        assert( remind.remind.kouCard[0]==CARD_UNKNOWN );
        assert( remind.remind.ming.choiceNum==1 );
        assert( remind.remind.ming.handouts[0].kind==TIAO_4 );
        assert( remind.remind.ming.handouts[0].ting.kindNum==1 );
        assert( remind.remind.ming.handouts[0].ting.cardNum==2 );
        assert( remind.remind.ming.handouts[0].ting.cards[0].kind==TIAO_4 );
        assert( remind.remind.ming.handouts[0].ting.cards[0].remain==2 );
        assert( remind.remind.ming.handouts[0].ting.cards[0].fan==0x1000 );
        assert( remind.wait==RIGHT );

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
            60,1,                  //seat
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

        DsMsg *aMsg = DsMsg::getInstance();
        FirstDistZhuang dist;

        len = aMsg->Deserialize(msgInNetwork);
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_BEGINCARDS );
        assert( aMsg->GetLevel()==7 );
        assert( dist.seat==MIDDLE );
        assert( dist.remain==1 );
        assert( dist.timer==2 );
        assert( dist.remind.actions==a_AN_GANG );
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
            60,1,                  //seat
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

        DsMsg *aMsg = DsMsg::getInstance();
        FirstDistNonZhuang dist;

        len = aMsg->Deserialize(msgInNetwork);
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_BEGINCARDS_OTHER );
        assert( aMsg->GetLevel()==7 );
        assert( dist.seat==MIDDLE );
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
            60,1,                  //seat
            61,1,                  //whoGive
            62,2,                  //next
            131,0,4,0,0,0,1,       //action:  碰   
            132,0,1,0              //card:    1条
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        DecisionNotif decision;

        len = aMsg->Deserialize(msgInNetwork);
        decision.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_DECISION );
        assert( aMsg->GetLevel()==7 );
        assert( decision.seat==MIDDLE );
        assert( decision.whoGive==1 );
        assert( decision.next==2 );
        assert( decision.actions==a_PENG );
		assert( decision.card[0].kind==TIAO_1);
 
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
            BUILD_NUMBER,          //client build number
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

        DsMsg *aMsg = DsMsg::getInstance();
        TingInfoResponse ting;

        len = aMsg->Deserialize(msgInNetwork);
        ting.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_GET_TINGINFO );
        assert( aMsg->GetLevel()==7 );
        assert( ting.info.cards[0].kind==TIAO_2);
        assert( ting.info.cards[0].remain==2 );
        assert( ting.info.cards[0].fan==3 );

        return 0;
    }
};

class TestActionRsp_Reconnect : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,50,               //request code
            7,                     //package level
            0x00,28,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            2,
            60,RECONNECT_REQUIRED, //error code:    1
            131,0,16,0xfe,0xff,
                0,0x31,0,',',0,0x33,0,',',0,0x35,0,',',0,0x37,         //roomPath:0x00010203
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        TingInfoResponse ting;

        len = aMsg->Deserialize(msgInNetwork);
        ting.Construct(*aMsg);

        assert(ting.failure==RECONNECT_REQUIRED);
        assert(ting.reconnectInfo.roomPath==1);
        assert(ting.reconnectInfo.roomId==3);
        assert(ting.reconnectInfo.tableId==5);
        assert(ting.reconnectInfo.seatId==7);
        
        return 0;
    }
};

class TestRecvHuInfoNotif_MeWin : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',
            0x00,0x3c,            //request code  REQ_GAME_DIST_HU_CALCULATE 60
            0x01,
            0x00,104,              //package size
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            
            0x05,
            0x3d,0x01,                                                      /* zhuang :MIDDLE*/
            0x81,0x00,0x0c,
                0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/* fan :   MIDDLE*/
            0x82,0x00,0x03,0x01,0x00,0x00,                                  /* winner: MIDDLE*/
            0x83,0x00,0x0c,
                0x10,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x30,0x00,0x00,0x00,/* score: MIDDLE*/
            0x87,0x00,42,
                0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x09,0x09,0x0a,0x0a,0x0a,0xff,/*MIDDLE 14*/
                0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x01,0x02,0x04,0xff,     /*RIGHT  13*/
                0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x01,0x02,0x04,          /*LEFT   13*/
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        HuInfoNotif info;

        len = aMsg->Deserialize(msgInNetwork);
        info.Construct(*aMsg);

        assert(msgInNetwork[7]==sizeof(msgInNetwork));
        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_HU_CALCULATE );
        assert( aMsg->GetLevel()==1 );
        
        assert( info.zhuang==MIDDLE );

        assert( info.hu[MIDDLE].status==HU_PAI );

        assert( info.hu[MIDDLE].fan==0x4000 );
        assert( info.hu[MIDDLE].score==0x10000000 );
        assert( info.hu[MIDDLE].cardNum==14 );
        
        assert( info.hu[RIGHT].fan==0 );
        assert( info.hu[RIGHT].score==0x20000000 );
        assert( info.hu[RIGHT].cardNum==13 );
        
        assert( info.hu[LEFT].fan==0 );
        assert( info.hu[LEFT].score==0x30000000 );
        assert( info.hu[LEFT].cardNum==13 );
        
        return 0;
    }
};

class TestRecvHuInfoNotif_DoubleWin : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',
            0x00,0x3c,            //request code  REQ_GAME_DIST_HU_CALCULATE 60
            0x01,
            0x00,100,             //package size
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            
            0x05,
            0x3d,0x02,
            0x81,0x00,0x08,0x00,0x00,0x40,0x00,0x00,0x00,0x80,0x00,           /* fan :   MIDDLE & LEFT*/
            0x82,0x00,0x03,0x01,0x00,0x01,                                    /* winner: MIDDLE & LEFT*/
            0x83,0x00,0x0c,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,
            0x87,0x00,43,
                0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x09,0x09,0x0a,0x0a,0x0a,0xff,/*MIDDLE 14*/
                0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x01,0x02,0x04,0xff,     /*RIGHT  13*/
                0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x01,0x02,0x04,0x06,     /*LEFT   14*/
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        HuInfoNotif info;

        len = aMsg->Deserialize(msgInNetwork);
        info.Construct(*aMsg);

        assert(msgInNetwork[7]==sizeof(msgInNetwork));
        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_HU_CALCULATE );
        assert( aMsg->GetLevel()==1 );
        
        assert( info.zhuang==RIGHT );

        assert( info.hu[MIDDLE].fan==0x4000 );
        assert( info.hu[MIDDLE].score==0x10000000 );
        assert( info.hu[MIDDLE].cardNum==14 );

        assert( info.hu[LEFT].fan==0x8000 );
        assert( info.hu[LEFT].score==0x20000000 );
        assert( info.hu[LEFT].cardNum==14 );

        assert( info.hu[RIGHT].cardNum==13 );
        
        return 0;
    }
};

void testGameActions() {
	CTestCase *aCase;
    
    SeatInfo *seat = SeatInfo::getInstance();
    seat->Set(0x00010203,0x04050607,0x08090a0b,1);

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
    aCase = new TestRecvShowCardNotif();
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
    
    aCase = new TestRecvRemindNotif_Handout1tiaoTing2tiao();
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

    aCase = new TestRecvHuInfoNotif_MeWin();
    aCase->Execute();
}

/*********************************************
    牌局无关REQUEST
*********************************************/
class TestSendEnter : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x05,                  //language id
            0x06,                  //client platform
            BUILD_NUMBER,          //client build number
            0x08,0x09,             //customer id
            0x0a,0x0b,             //product id
            0x00,44,               //request code(请求进入房间 REQ_GAME_SEND_ENTER)
            0x00,38,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            1,
            131,0,4,0,0,0,11,         //roomPath
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        RequestEnterRoom aMsg;
        aMsg.Set(0);
        len = aMsg.Serialize(buf);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestRecvEnterResponse : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,44,               //request code
            7,                     //package level
            0x00,110,              //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            9,
            131,0,4,0,1,2,3,         //roomPath:0x00010203
            132,0,4,4,5,6,7,         //roomId:  0x04050607
            133,0,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                    //site:    1
            134,0,4,0,0,0,1,         //底分 base score
            135,0,3,1,1,1,           //player status
            136,0,12,                //player score
                0,0,0,100,
                0,0,0,200,
                0,0,0,300,
            137,0,16,0xfe,0xff,0,0x31,0,'@',0,'%',0,0x32,0,'@',0,'%',0,0x30,           //player name, UTF-16
            138,0,16,0xfe,0xff,0,0x31,0,'@',0,'%',0,0x32,0,'@',0,'%',0,0x30            //player image, UTF-16
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        EnterRoomResponse room;

        len = aMsg->Deserialize(msgInNetwork);
        room.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_SEND_ENTER );
        assert( aMsg->GetLevel()==7 );

        SeatInfo *seat = SeatInfo::getInstance();
        assert(seat->_roomPath==0x00010203);
        assert(seat->_roomId==0x04050607);
        assert(seat->_tableId==0x08090a0b);
        assert(seat->_seatId==1);

        assert(!strcmp((char *)room.name[MIDDLE],"1"));
        assert(!strcmp((char *)room.image[MIDDLE],"1"));
        
        return 0;
    }
};

class TestSelfSeatEquals2 : public CTestCase {
public:
    void SetSelfSeatEquals2() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,44,               //request code
            7,                     //package level
            0x00,90,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            9,
            131,0,4,0,1,2,3,         //roomPath:0x00010203
            132,0,4,4,5,6,7,         //roomId:  0x04050607
            133,0,4,8,9,10,11,       //tableId: 0x08090a0b
            60,2,                    //site:    1
            134,0,4,0,0,0,1,         //底分 base score
            135,0,3,1,1,1,           //player status
            136,0,12,                //player score
                0,0,0,100,
                0,0,0,200,
                0,0,0,300,
            137,0,6,0,1,0,2,0,0,           //player name, UTF-16
            138,0,6,0,1,0,2,0,0            //player image, UTF-16
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        EnterRoomResponse room;

        len = aMsg->Deserialize(msgInNetwork);
        room.Construct(*aMsg);
    }
    
    virtual int Execute() {
        SetSelfSeatEquals2();
        
        INT8U RemindPkg[] = {
            'K','W','X',           //KWX
            0x00,55,               //request code/*下发提醒(下行) REQ_GAME_DIST_REMIND*/
            7,                     //package level
            0x00,53,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            9,
            60,2,                        //seat
            61,4,                        //timer
            62,5,                        //kind :        6条
            63,1,                        //whogive
            129,0,4,0,0,0,1,             //remind :      碰
            130,0,1,0xff,                //gang remind : 不可杠
            131,0,1,0xff,                //kou remind :  不可扣
            132,0,4,0xff,0xff,0xff,0xff, //ming remind : 不可明
            64,3,                        //wait
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(RemindPkg);

		RemindInfo remind;
        remind.Construct(*aMsg);

        assert( remind.seat==MIDDLE );
        assert( remind.whoGive==LEFT );
        assert( remind.wait==RIGHT );

        return 0;
    }
};

class TestSelfSeatEquals3 : public CTestCase {
public:
    void SetSelfSeatEquals3() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,44,               //request code
            7,                     //package level
            0x00,90,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            9,
            131,0,4,0,1,2,3,         //roomPath:0x00010203
            132,0,4,4,5,6,7,         //roomId:  0x04050607
            133,0,4,8,9,10,11,       //tableId: 0x08090a0b
            60,3,                    //site:    1
            134,0,4,0,0,0,1,         //底分 base score
            135,0,3,1,1,1,           //player status
            136,0,12,                //player score
                0,0,0,100,
                0,0,0,200,
                0,0,0,300,
            137,0,6,0,1,0,2,0,0,           //player name, UTF-16
            138,0,6,0,1,0,2,0,0            //player image, UTF-16
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        EnterRoomResponse room;

        len = aMsg->Deserialize(msgInNetwork);
        room.Construct(*aMsg);
    }
    
    virtual int Execute() {
        SetSelfSeatEquals3();
        
        INT8U RemindPkg[] = {
            'K','W','X',           //KWX
            0x00,55,               //request code/*下发提醒(下行) REQ_GAME_DIST_REMIND*/
            7,                     //package level
            0x00,53,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            9,
            60,3,                        //seat
            61,4,                        //timer
            62,5,                        //kind :        6条
            63,1,                        //whogive
            129,0,4,0,0,0,1,             //remind :      碰
            130,0,1,0xff,                //gang remind : 不可杠
            131,0,1,0xff,                //kou remind :  不可扣
            132,0,4,0xff,0xff,0xff,0xff, //ming remind : 不可明
            64,2,                        //wait
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        len = aMsg->Deserialize(RemindPkg);

		RemindInfo remind;
        remind.Construct(*aMsg);

        assert( remind.seat==MIDDLE );
        assert( remind.whoGive==RIGHT );
        assert( remind.wait==LEFT );

        return 0;
    }
};

class TestRecvEnterNotif : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,71,               //request code
            7,                     //package level
            0x00,52,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            6,
            61,2,                    //new comer
            131,0,4,0,0,0,1,         //底分 base score
            62,2,                    //player status
            132,0,4,0,0,0,1,         //player score
            133,0,4,0,1,0,2,         //player name
            134,0,4,0,3,0,4,         //player image
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        EnterRoomNotif player;

        len = aMsg->Deserialize(msgInNetwork);
        player.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_RECV_ENTER );
        assert( aMsg->GetLevel()==7 );

        assert(player.seat==2);
        
        return 0;
    }
};

class TestHeartBeat : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x05,                  //language id
            0x06,                  //client platform
            BUILD_NUMBER,          //client build number
            0x08,0x09,             //customer id
            0x0a,0x0b,             //product id
            0x00,40,               //request code(请求进入房间 REQ_GAME_SEND_ENTER)
            0x00,38,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            1,
            131,0,4,1,2,3,4,         //userId
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        RequestHeartBeat aMsg;
        aMsg.Set();
        len = aMsg.Serialize(buf);

        assert(len==sizeof(msgInNetwork));
        assert(!memcmp(buf,msgInNetwork,len));

        return 0;
    }
};

class TestRecvReconnectResponse : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,42,               //request code
            7,                     //package level
            0x00,193,              //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            13,
            61,1,                    //site:    1
            62,2,                    //current player
            63,3,                    //zhuang
            129,0,4,0,0,0,1,         //剩余牌数
            130,0,4,0,0,1,0,         //倒计时
            131,0,4,0,0,0,1,         //底分 base score
            132,0,3,1,1,1,           //player status
            133,0,12,                //player score
                0,0,0,100,
                0,0,0,200,
                0,0,0,300,
            134,0x00,41,             //手牌
                0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x09,0x09,0x0a,0x0a,0xff,     /*MIDDLE 14*/
                0x10,0x11,0x12,0x13,0x14,0x10,0x11,0x06,0x07,0x08,0x01,0x02,0x04,0xff,     /*RIGHT  13*/
                0x10,0x11,0x12,0x13,0x14,0x10,0x11,0x06,0x07,0x08,0x01,0x02,0x04,          /*LEFT   13*/
            135,0,11,0,1,0xff,0,2,3,0xff,0,4,5,6,           //明状态
            136,0x00,5,              //出牌
                0x01,0xff,
                0x02,0xff,
                0x03,     
            137,0,36,                //名字
                0xfe,0xff,0x00,0x6a,0x00,0x69,0x00,0x61,0x00,0x6e,0x00,0x67,0x00,0x62,0x00,0x6f,0,'@',0,'%',
                0x00,0x6a,0x00,0x69,0x00,0x61,0,'@',0,'%',
                0x00,0x6e,0x00,0x67,0x00,0x62,
            138,0,16,                //头像
                0xfe,0xff,0x00,0x31,0,'@',0,'%',
                0x00,0x31,0,'@',0,'%',
                0x00,0x31,
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        ReconnectResponse info;

        len = aMsg->Deserialize(msgInNetwork);
        info.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_SEND_RECONNECT );
        assert( aMsg->GetLevel()==7 );

        assert(info.isMing[MIDDLE]);
        assert(info.tingNum[MIDDLE]==1);
        assert(info.tingCards[MIDDLE][0]==TIAO_2);
        
        return 0;
    }
};

class TestRecvLoginResponse_Normal : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,1,                //request code
            7,                     //package level
            0x00,88,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            10,
            60,0,                    //status
            61,0,                    //user type
            62,0,                    //user activated
            129,0,4,0,0,0,0,         //reconnect info
            130,0,4,0,0,0,1,         //user id
            131,0,4,0,0,0,2,         //key
            135,0,4,0,0,0,3,         //room server id
            136,0,16,                //room server ip
                0xfe,0xff,0,'1',0,'.',0,'2',0,'.',0,'3',0,'.',0,'4',
            137,0,4,0,0,0,5,         //room server port
            138,0,4,0,0,0,6,         //room server voice port
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        LoginResponse login;

        len = aMsg->Deserialize(msgInNetwork);
        login.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_LOGIN );
        assert( aMsg->GetLevel()==7 );

        assert(login._userType==0);
        assert(login._userActivated==0);

        EnvVariable *env = EnvVariable::getInstance();
//        assert(env->_key==2);
        assert(env->_roomServer.id==3);
        assert(env->_roomServer.port==5);
        assert(env->_roomServer.voicePort==6);
        //IP of server is controlled by NETWORK_GAME_DEBUG
        //assert(!strcmp(env->_roomServer.ipaddr,"1.2.3.4")); 
        assert(!env->IsReconnectRequired());

        return 0;
    }
};

class TestRecvLoginResponse_Reconnect : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,44,               //request code
            7,                     //package level
            0x00,104,              //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            10,
            60,0,                    //status
            61,0,                    //user type
            62,0,                    //user activated
            129,0,20,                //reconnect info
                0,0,0,1,         
                0,0,0,1,         
                0,0,0,2,         
                0,0,0,3,         
                0,0,0,4,         
            130,0,4,0,0,0,1,         //user id
            131,0,4,0,0,0,2,         //key
            135,0,4,0,0,0,3,         //room server id
            136,0,16,                //room server ip
                0xfe,0xff,0,'1',0,'.',0,'2',0,'.',0,'3',0,'.',0,'4',
            137,0,4,0,0,0,5,         //room server port
            138,0,4,0,0,0,6,         //room server voice port
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        LoginResponse login;

        len = aMsg->Deserialize(msgInNetwork);
        login.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_SEND_ENTER );
        assert( aMsg->GetLevel()==7 );

        assert(login._userType==0);
        assert(login._userActivated==0);

        EnvVariable *env = EnvVariable::getInstance();
//        assert(env->_key==2);
        assert(env->_roomServer.id==3);
        assert(env->_roomServer.port==5);
        assert(env->_roomServer.voicePort==6);
        assert(env->IsReconnectRequired());

        SeatInfo *seat = SeatInfo::getInstance();
        assert(seat->_roomPath==1);
        assert(seat->_roomId==2);
        assert(seat->_tableId==3);
        assert(seat->_seatId==4);
        
        return 0;
    }
};


class TestRecvDailyLoginResponse : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            0x4b,0x57,0x58,
            0x00,0x25,
            0x00,
            0x00,0x38,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

            0x05,
            0x81,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
            0x82,0x00,0x04,0xfe,0xff,0x00,0x30,
            0x83,0x00,0x00,
            0x84,0x00,0x04,0x00,0x00,0x27,0x10,
            0x85,0x00,0x04,0x00,0x00,0x00,0x00,
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DsMsg *aMsg = DsMsg::getInstance();
        DailyLoginResponse room;

        len = aMsg->Deserialize(msgInNetwork);
        room.Construct(*aMsg);
       
        return 0;
    }
};


#include "./../../protocol/KwxMsgLogin.h"

void testOtherRequests() {
	CTestCase *aCase;

    aCase = new TestRecvDailyLoginResponse();
    aCase->Execute();
    
    aCase = new TestSendEnter();
    aCase->Execute();
    
    aCase = new TestRecvEnterResponse();
    aCase->Execute();

    aCase = new TestSelfSeatEquals2();
    aCase->Execute();

    aCase = new TestSelfSeatEquals3();
    aCase->Execute();
    
    aCase = new TestRecvEnterResponse();/* TO MAKE SELF SEAT EQUALS 1*/
    aCase->Execute();
    
    aCase = new TestRecvEnterNotif();
    aCase->Execute();
    
    aCase = new TestHeartBeat();
    aCase->Execute();

    aCase = new TestActionRsp_Reconnect();
    aCase->Execute();

    aCase = new TestRecvReconnectResponse();
    aCase->Execute();

    aCase = new TestRecvLoginResponse_Normal();
    aCase->Execute();

    aCase = new TestRecvLoginResponse_Reconnect();
    aCase->Execute();
}

void testRequests() {
    testGameActions();
    testOtherRequests();
}

