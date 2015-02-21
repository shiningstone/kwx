#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>

#include "./../../protocol/MsgFormats.h"
#include "./../../protocol/KwxMsg.h"

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
    static void MsgHandler(const INT8U *msg,int &len) {
        memcpy(RecvBuf,msg,len);
    }

	virtual void ClientActions() {
        memset(RecvBuf,0,256);

        KwxMsg aMsg(DOWN_STREAM);
        aMsg.StartReceiving((MsgHandler_t)&TestKwxAutoRecv::MsgHandler);
        Sleep(DELAY);

        assert( !memcmp(RecvBuf,MESSAGE,MESSAGE_LEN) );

        aMsg.StopReceiving();        //为了不影响后续的测试用例，是不是应该为TestCase增加Stop？
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
        KwxMsg aMsg(DOWN_STREAM);
        aMsg.StartReceiving();
        Sleep(DELAY);

        aMsg.StopReceiving();        //为了不影响后续的测试用例，是不是应该为TestCase增加Stop？
    }
};


void testKwxMsgUsingSocket() {
	CTestCase *aCase;

	aCase = new TestKwxAutoRecv();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	aCase = new TestKwxAutoHandleMsg();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

    while(1);
}