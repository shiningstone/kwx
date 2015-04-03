
#include "./../../utils/BasicType.h"

#include "./../../protocol/RequestStructs.h"
#include "./../../protocol/CommonMsg.h"
#include "./../../protocol/DsInstruction.h"
#include "./../../protocol/KwxMsgEnv.h"
#include "./../../protocol/KwxMessenger.h"

/********************************************************************
	TestCases
*********************************************************************/
#include <assert.h>
#include "./../../RaceType.h"
#include "./CTestProtocol.h"

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
            0x00,49,               //request code(������ҷ�Ӧ REQ_GAME_SEND_ACTION)
            0x00,65,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            6,
            131,0,4,0,1,2,3,         //roomPath:0x00010203
            132,0,4,4,5,6,7,         //roomId:  0x04050607
            133,0,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                    //site:    1
            134,0,4,0,0,0,1,         //act:     1(��)                 
            135,0,1,2                //card:    3��
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

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
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
            61,0,                  //who show card: 0 server
            62,2,                  //next player
            131,0,4,0,0,0,1,       //action : ��
            132,0,1,2,             //card :   3��
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        ActionNotif action;
        action.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_RECV_ACTION );
        assert( aMsg->GetLevel()==7 );
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
            0x00,43,               //request code(����ʼ REQ_GAME_SEND_START)
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

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
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

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
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
            0x07,                  //client build number
            0x08,0x09,             //customer id
            0x0a,0x0b,             //product id
            0x00,48,               //request code(������ҳ��� REQ_GAME_SEND_SHOWCARD)
            0x00,56,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            5,
            131,0,4,0,1,2,3,         //roomPath:0x00010203
            132,0,4,4,5,6,7,         //roomId:  0x04050607
            133,0,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                    //site:    1
            65,2,                    //show card : 3��
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
            131,0,4,1,2,0,3,       //ting info : ��2����ʣ2�ţ�Ӯ3��
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        HandoutResponse handoutInfo;
        handoutInfo.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_SEND_START );
        assert( aMsg->GetLevel()==7 );
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
            65,0,                  //show card:  1��
            131,0,4,9,2,0,3,       //ting info : ��2����ʣ2�ţ�Ӯ3��
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        HandoutNotif handoutInfo;
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

class TestRecvDistCard_MingGang4tiao : public CTestCase {
public:
    virtual int Execute() {
        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,51,               //request code/*����(����) REQ_GAME_DIST_CARD*/
            7,                     //package level
            0x00,46,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            8,
            60,0,                  //seat
            61,1,                  //timer
            62,2,                  //reserved card num
            63,3,                  //card:               ��4��
            64,2,                  //remind:             ����
            130,0,1,3,             //gang remind:        �ɸ�4��
            131,0,1,0xff,          //kou remind:         ���ɿ�
            132,0,4,0xff,0xff,0xff,0xff,  //ming remind: ������
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        DistCardInfo dist;
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_CARD );
        assert( aMsg->GetLevel()==7 );
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
            0x00,51,               //request code/*����(����) REQ_GAME_DIST_CARD*/
            7,                     //package level
            0x00,54,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            8,
            60,0,                  //seat
            61,1,                  //timer
            62,2,                  //reserved card num
            63,3,                  //card:               ��4��
            64,0x40,               //remind:             ��
            130,0,1,0xff,          //gang remind:        ���ɸ�
            131,0,1,0xff,          //kou remind:         ���ɿ�
            132,0,12,
                0,0,4,3,           //ming remind:        ��5���ɺ�3��
                5,1,0,2,           //                    ��6����ʣ1�ţ�Ӯ2��
                6,2,0,4,           //                    ��7����ʣ2�ţ�Ӯ4��
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        DistCardInfo dist;
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_CARD );
        assert( aMsg->GetLevel()==7 );
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
            0x00,51,               //request code/*����(����) REQ_GAME_DIST_CARD*/
            7,                     //package level
            0x00,62,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            8,
            60,0,                  //seat
            61,1,                  //timer
            62,2,                  //reserved card num
            63,3,                  //card:               ��4��
            64,0x40,               //remind:             ��
            130,0,1,0xff,          //gang remind:        ���ɸ�
            131,0,1,0xff,          //kou remind:         ���ɿ�
            132,0,20,
                0,0,4,3,           //ming remind:        ��5���ɺ�3��
                5,1,0,2,           //                    ��6����ʣ1�ţ�Ӯ2��
                6,2,0,4,           //                    ��7����ʣ2�ţ�Ӯ4��
                0,0,7,1,           //ming remind:        ��8���ɺ�1��
                8,1,0,9,           //                    ��9����ʣ1�ţ�Ӯ9��
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

        DistCardInfo dist;
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_CARD );
        assert( aMsg->GetLevel()==7 );
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
            0x00,52,               //request code/*����(����) REQ_GAME_DIST_CARD_TOOTHER*/
            7,                     //package level
            0x00,39,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            4,
            60,0,                  //seat
            61,1,                  //reserved card num
            62,2,                  //timer
            63,3,                  //card:               ��4��
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

		DistCardNotif dist;
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_CARD_TOOTHER );
        assert( aMsg->GetLevel()==7 );
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
            0x00,53,               //request code/*�·��Ʒֽ���(����) REQ_GAME_SEND_CALSCORE*/
            7,                     //package level
            0x00,42,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            2,
            131,0,3,1,2,3,                     //seat[]
            135,0,12,0,0,0,1,0,0,0,2,0,0,0,3   //score[]
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

		ScoreNotif score;
        score.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_SEND_CALSCORE );
        assert( aMsg->GetLevel()==7 );
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
            0x00,55,               //request code/*�·�����(����) REQ_GAME_DIST_REMIND*/
            7,                     //package level
            0x00,42,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            7,
            60,1,                        //seat
            61,2,                        //timer
            129,0,4,0,0,0,1,             //remind :      ��
            130,0,1,0xff,                //gang remind : ���ɸ�
            131,0,1,0xff,                //kou remind :  ���ɿ�
            132,0,4,0xff,0xff,0xff,0xff, //ming remind : ������
            63,0,                        //wait :        ����
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        len = aMsg->Deserialize(msgInNetwork);

		RemindInfo remind;
        remind.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_REMIND );
        assert( aMsg->GetLevel()==7 );
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
            0x00,56,               //request code/*���ַ���(����) REQ_GAME_DIST_BEGINCARDS*/
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
            129,0,4,0,0,0,4,       //remind:             ����
            130,0,1,10,             //gang remind:       �ɸ�2Ͳ
            131,0,1,0xff,          //kou remind:         ���ɿ�
            132,0,4,0xff,0xff,0xff,0xff,  //ming remind: ������
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        FirstDistZhuang dist;

        len = aMsg->Deserialize(msgInNetwork);
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_BEGINCARDS );
        assert( aMsg->GetLevel()==7 );
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
            0x00,57,               //request code/*��ׯ�ҿ��ַ���(����) REQ_GAME_DIST_BEGINCARDS_OTHER*/
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

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        FirstDistNonZhuang dist;

        len = aMsg->Deserialize(msgInNetwork);
        dist.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_BEGINCARDS_OTHER );
        assert( aMsg->GetLevel()==7 );
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
            0x00,59,               //request code/*�·����ն���(����) REQ_GAME_DIST_DECISION*/
            7,                     //package level
            0x00,45,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            5,
            60,0,                  //seat
            61,1,                  //whoGive
            62,2,                  //next
            131,0,4,0,0,0,1,       //action:  ��   
            132,0,1,0              //card:    1��
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
        DecisionNotif decision;

        len = aMsg->Deserialize(msgInNetwork);
        decision.Construct(*aMsg);

        assert(len==sizeof(msgInNetwork));
        assert( aMsg->GetRequestCode()==REQ_GAME_DIST_DECISION );
        assert( aMsg->GetLevel()==7 );
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
            0x00,50,               //request code(��ȡͣ����Ϣ REQ_GAME_GET_TINGINFO)
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
            131,0,4,1,2,0,3,        //ting info : ��2����ʣ2�ţ�Ӯ3��
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        KwxDsMsg *aMsg = KwxDsMsg::getInstance();
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
