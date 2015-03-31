#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>

#include "./../../protocol/MsgFormats.h"
#include "./../../protocol/KwxMsg.h"
#include "./../../protocol/KwxMessenger.h"

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
            0x00,50,               //request code(发送发牌请求)
            7,                     //package level
            0x00,27,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            3,
            60,1,                  //roomId
            61,2,                  //seat
            70,0,                  //card kind
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

        KwxMessenger::StartReceiving((MsgHandler_t)&TestKwxAutoRecv::MsgHandler);
        Sleep(DELAY);

        assert( !memcmp(RecvBuf,MESSAGE,MESSAGE_LEN) );

        KwxMessenger::StopReceiving();        //为了不影响后续的测试用例，是不是应该为TestCase增加Stop？
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
            0x00,50,               //request code(发送发牌请求)
            7,                     //package level
            0x00,27,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            3,
            60,1,                  //roomId
            61,2,                  //seat
            70,0,                  //card kind
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
        KwxMessenger::StartReceiving();
        Sleep(DELAY);
        KwxMessenger::StopReceiving();        //为了不影响后续的测试用例，是不是应该为TestCase增加Stop？
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

class TestSetRaction : public TestRequest {
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

        SetExpValue(MESSAGE,sizeof(MESSAGE));
        TestRequest::ServerActions();
	}

	virtual void ClientActions() {
        KwxMessenger aMessenger;
        aMessenger.SetReaction(aGANG,TIAO_4);
    }
};

static void testRequests() {
	CTestCase *aCase;

	aCase = new TestSetRaction();
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

void testKwxMsgUsingSocket() {
    testRequests();
    testAutoRecv();
    while(1);
}
