#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>

#include "./../../protocol/MsgFormats.h"
#include "./../../protocol/CommonMsg.h"
#include "./../../protocol/KwxMessenger.h"
#include "./../../protocol/DsInstruction.h"
#include "./../../protocol/KwxMsgLogin.h"

#include "./../../network/CSockets.h"
#include "./../../network/GameSocket.h"

#include "./../network/CTestSocket.h"

class TestKwxAutoRecv : public CTestMessenger {
	INT8U MESSAGE[MSG_MAX_LEN];
	int   MESSAGE_LEN;

	virtual void Start() {
		CTestMessenger::Start();

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

		MESSAGE_LEN = sizeof(msgInNetwork);
		memcpy( MESSAGE,msgInNetwork,MESSAGE_LEN );
	}

	virtual void ServerActions() {
		SERVER.Start();

		SERVER.Send((char *)MESSAGE,MESSAGE_LEN);

		SERVER.Stop();
	}

    static INT8U RecvBuf[256];
    static int MsgHandler(const INT8U *msg,int &len) {
        memcpy(RecvBuf,msg,len);
        return 0;
    }

	virtual void ClientActions() {
        memset(RecvBuf,0,256);

		KwxMessenger aMessenger(MSG_GAME);

        aMessenger.StartReceiving((MsgHandler_t)&TestKwxAutoRecv::MsgHandler);
        Sleep(DELAY);

        assert( !memcmp(RecvBuf,MESSAGE,MESSAGE_LEN) );

        aMessenger.StopReceiving();        //为了不影响后续的测试用例，是不是应该为TestCase增加Stop？
    }
};

INT8U TestKwxAutoRecv::RecvBuf[256] = {0};
class TestKwxAutoHandleMsg : public CTestMessenger {
	INT8U MESSAGE[MSG_MAX_LEN];
	int   MESSAGE_LEN;

	virtual void Start() {
		CTestMessenger::Start();

        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,43,               //request code
            7,                     //package level
            0x00,28,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            1,
            131,0,4,0,0,0,1,       //score
        };

		MESSAGE_LEN = sizeof(msgInNetwork);
		memcpy( MESSAGE,msgInNetwork,MESSAGE_LEN );
	}

	virtual void ServerActions() {
		SERVER.Start();

		SERVER.Send((char *)MESSAGE,MESSAGE_LEN);

		SERVER.Stop();
	}

	virtual void ClientActions() {
		KwxMessenger aMessenger(MSG_GAME);

        aMessenger.StartReceiving();
        Sleep(DELAY);
        aMessenger.StopReceiving();        //为了不影响后续的测试用例，是不是应该为TestCase增加Stop？
    }
};

/**************************************************
        requests
**************************************************/
class TestRequest : public CTestMessenger {
protected:
    void SetExpValue(INT8U *buf,int len) {
        memcpy(ExpMessage,buf,len);
        ExpLen = len;
    }

    INT8U ExpMessage[MSG_MAX_LEN];
    int   ExpLen;

	virtual void ServerActions() {
        INT8U msgInNetwork[MSG_MAX_LEN] = {0};
        int   len = 0;

		SERVER.Start();

		SERVER.Recv((char *)msgInNetwork,&len);

        assert(len==ExpLen);
        assert(!memcmp(msgInNetwork,ExpMessage,len));

		SERVER.Stop();
	}
};

class TestSendAction : public TestRequest {
	virtual void ServerActions() {
        INT8U MESSAGE[] = {
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
            60,1,                  //site:    1
            134,0,4,0,0,0,1,         //act:     1(碰)                 
            135,0,1,2                //card:    3tiao
        };

        SetExpValue(MESSAGE,sizeof(MESSAGE));
        TestRequest::ServerActions();
	}

	virtual void ClientActions() {
        KwxMessenger aMessenger(MSG_GAME);
        aMessenger.StartReceiving();
        
        RequestSendAction aMsg;
        aMsg.Set(aPENG,TIAO_3);
        aMessenger.Send(aMsg);

        aMessenger.StopReceiving();
    }
};

static void testRequests() {
	CTestCase *aCase;
    
    aCase = new TestSendAction();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();

}

static void testAutoRecv() {
	CTestCase *aCase;

	aCase = new TestKwxAutoRecv();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	aCase = new TestKwxAutoHandleMsg();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();
}

#include "./../../protocol/KwxMessenger.h"
class TestHeartBeatOnce : public TestRequest {
	virtual void ServerActions() {
        INT8U MESSAGE[] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x05,                  //language id
            0x06,                  //client platform
            0x07,                  //client build number
            0x08,0x09,             //customer id
            0x0a,0x0b,             //product id
            0x00,40,               //request code(请求进入房间 REQ_GAME_SEND_ENTER)
            0x00,38,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            1,
            131,0,4,1,2,3,4,         //userId
        };

        SetExpValue(MESSAGE,sizeof(MESSAGE));
        TestRequest::ServerActions();
	}

	virtual void ClientActions() {
        KwxHeart *aHeart = KwxHeart::getInstance();
        Sleep(1);
        aHeart->destroyInstance();
    }
};

class TestHeartBeatContinuous : public TestRequest {
    static const int EXP_RECVS = 3;
    
	virtual void ServerActions() {
        INT8U msgInNetwork[MSG_MAX_LEN] = {0};
        int   len = 0;

        INT8U MESSAGE[] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x05,                  //language id
            0x06,                  //client platform
            0x07,                  //client build number
            0x08,0x09,             //customer id
            0x0a,0x0b,             //product id
            0x00,40,               //request code(请求进入房间 REQ_GAME_SEND_ENTER)
            0x00,38,               //package size
            0,0,0,0,0,0,0,0,0,0,0, //reserved(11)

            1,
            131,0,4,1,2,3,4,         //userId
        };

        SetExpValue(MESSAGE,sizeof(MESSAGE));

        int RecvCount = 0;
        
		SERVER.Start();

		while( SERVER.Recv((char *)msgInNetwork,&len) ) {
            if(++RecvCount>EXP_RECVS) {
                break;
            }

            assert(len==ExpLen);
            assert(!memcmp(msgInNetwork,ExpMessage,len));
        }

		SERVER.Stop();
	}

	virtual void ClientActions() {
        KwxHeart *aHeart = KwxHeart::getInstance();
        Sleep(1000*1.1*EXP_RECVS);
        aHeart->destroyInstance();
    }
};

static void testHeartBeat() {
	CTestCase *aCase;

	aCase = new TestHeartBeatOnce();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

    aCase = new TestHeartBeatContinuous();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();
}

void testKwxMsgUsingSocket() {
    testRequests();
    testAutoRecv();
    testHeartBeat();
    //while(1);
}
