#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>

#include "./../../protocol/KwxMsgBasic.h"
#include "./../../protocol/MsgFormats.h"
#include "./../../network/CSockets.h"
#include "./../../network/GameSocket.h"

#include "./CTestSocket.h"

#define DEF_SERVER_IP    "127.0.0.1"
#define DEF_SOCKET_PORT  60905

class TestSingleSendAndRecv : public CTestSocket {
	virtual void ServerActions() {
		SERVER.Start();

		SERVER.Send("to client",strlen("to client"));

		SERVER.Recv(_serverBuff,&_serverBuffLen);
		assert(!strcmp(_serverBuff,"to server"));
		ClearServerBuf();

		SERVER.Stop();
	}

	virtual void ClientActions() {
		CLIENT.Start();

		CLIENT.Send("to server",strlen("to server"));

		CLIENT.Recv(_clientBuff,&_clientBuffLen);
		assert( !strcmp(_clientBuff,"to client") );
		ClearClientBuf();

		CLIENT.Stop();
	}
};

class TestGameSocket : public CTestSocket {
	virtual void ServerActions() {
		SERVER.Start();

		_serverBuffLen = 10;
		memset(_serverBuff,0,8);
		SERVER.Send("to client",strlen("to client"));
		SERVER.Recv(_serverBuff,&_serverBuffLen);
		Sleep(DELAY);
		ClearServerBuf();

		SERVER.Stop();
	}

	virtual void ClientActions() {
		CGameSocket client;
		client.Create(DEF_SERVER_IP, DEF_SOCKET_PORT);
		
		client.ForceSend("to server",strlen("to client"));

		_clientBuffLen = 10;
		client.ForceRecv(_clientBuff,_clientBuffLen);
		Sleep(DELAY);
		ClearClientBuf();
	}
};

class TestByteOrder : public CTestSocket {
	#pragma pack(1)
	typedef struct {
		char a[3];
		int  b;
		char c;
		short d;
	} tStruct;
	#pragma pack()

	virtual void ServerActions() {
		SERVER.Start();

		int a = 0x01020304;
		SERVER.Recv(_serverBuff,&_serverBuffLen);
		assert(!memcmp(_serverBuff,(char *)&a,sizeof(int)));
		assert(_serverBuffLen==4);
		ClearServerBuf();

		tStruct t = {
			{0xa,0xb,0xc},
			0x01020304,
			0x05,
			0x06,
		};
		SERVER.Recv(_serverBuff,&_serverBuffLen);
		assert(!memcmp(_serverBuff,(char *)&t,sizeof(t)));
		assert(_serverBuffLen==10);
		ClearServerBuf();

		SERVER.Stop();
	}

	virtual void ClientActions() {
		CLIENT.Start();

		//test int
		int a = 0x01020304;
		CLIENT.Send((char *)&a,sizeof(int));

		//test struct
		tStruct t = {
			{0xa,0xb,0xc},
			0x01020304,
			0x05,
			0x06,
		};
		CLIENT.Send((char *)&t,sizeof(t));

		CLIENT.Stop();
	}
};

class TestMultipleSendAndRecv : public CTestSocket {
	virtual void ServerActions() {
		char buf[32] = {0};
		int i        = 0;

		SERVER.Start();

		sprintf_s(buf,"to client %d",i++);
		SERVER.Send(buf,strlen(buf));
		Sleep(DELAY);

		sprintf_s(buf,"to client %d",i++);
		SERVER.Send(buf,strlen(buf));

		SERVER.Recv(_serverBuff,&_serverBuffLen);
		if( (!strcmp(_serverBuff,"to server 0")) || (strstr(_serverBuff,"to server 0")!=0) ) {
			if ( strstr(_serverBuff,"to server 1")!=0 ) {
				SERVER.Stop();
				return;
			} else {
				ClearServerBuf();

				SERVER.Recv(_serverBuff,&_serverBuffLen);
				assert( !strcmp(_serverBuff,"to server 1") );
			}
		} else {
			assert("server : the first package missed\n");
		}

		SERVER.Stop();
	}

	virtual void ClientActions() {
		char buf[32] = {0};
		int i        = 0;

		CLIENT.Start();

		sprintf_s(buf,"to server %d",i++);
		CLIENT.Send(buf,strlen(buf));
		Sleep(DELAY);

		sprintf_s(buf,"to server %d",i++);
		CLIENT.Send(buf,strlen(buf));

		CLIENT.Recv(_clientBuff,&_clientBuffLen);
		if( (!strcmp(_clientBuff,"to client 0")) || (strstr(_clientBuff,"to client 0")!=0) ) {
			if ( strstr(_clientBuff,"to client 1")!=0 ) {
				CLIENT.Stop();
				return;
			} else {
				ClearClientBuf();

				CLIENT.Recv(_clientBuff,&_clientBuffLen);
				assert( !strcmp(_clientBuff,"to client 1") );
			}
		} else {
			assert("CLIENT : the first package missed\n");
		}

		CLIENT.Stop();
	}
};

class TestEmpty : public CTestSocket {
	virtual void ServerActions() {
	}

	virtual void ClientActions() {
	}
};

void testBasicSocket() {
	CTestCase *aCase;

	aCase = new TestSingleSendAndRecv();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	aCase = new TestByteOrder();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	aCase = new TestMultipleSendAndRecv();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

#if 0
	aCase = new TestGameSocket();   //这个case执行以后socket start失败
	aCase->Start();
	aCase->Execute();
	aCase->Stop();
#endif
}

/***************************************************************************
            NetMessenger测试用例
/***************************************************************************/
class TestNetMessengerRecvOneFullPackage : public CTestMessenger {
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
		MESSENGER->Start();

		while( !MESSENGER->Recv((INT8U *)_clientBuff,_clientBuffLen) ) {
			Sleep(DELAY);
		}

		assert( !memcmp(_clientBuff,MESSAGE,MESSAGE_LEN) );
	}
};

class TestNetMessengerRecvOneFullPackagePlusOneFrontPartialPackage : public CTestMessenger {
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

		    'K','W','X',           //KWX
            0x00,50,               //request code(发送发牌请求)
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
		MESSENGER->Start();

		while( !MESSENGER->Recv((INT8U *)_clientBuff,_clientBuffLen) ) {
			Sleep(DELAY);
		}

		assert( _clientBuffLen==(MESSAGE_LEN-5) );
		assert( !memcmp(_clientBuff,MESSAGE,_clientBuffLen) );
		assert( memcmp(_clientBuff,MESSAGE,MESSAGE_LEN) );
		ClearClientBuf();
	}
};


class TestNetMessengerRecvBehindPartialPackage : public CTestMessenger {
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
		SERVER.Send((char *)MESSAGE+5,MESSAGE_LEN-5);
		SERVER.Stop();
	}

	virtual void ClientActions() {
		MESSENGER->Start();

		while( !MESSENGER->Recv((INT8U *)_clientBuff,_clientBuffLen) ) {
			Sleep(DELAY);
		}

		assert( _clientBuffLen==MESSAGE_LEN );
		assert( !memcmp(_clientBuff,MESSAGE,_clientBuffLen) );
		ClearClientBuf();
	}
};

class TestNetMessengerRecvDesignatedPackage : public CTestMessenger {
	INT8U MESSAGE[MSG_MAX_LEN];
	int   MESSAGE_LEN;

	virtual void Start() {
		CTestMessenger::Start();

        INT8U msgInNetwork[] = {
            'K','W','X',           //KWX
            0x00,74,               //request code(下发其他玩家动作)
            7,                     //package level
            0x00,25,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)
            2,60,1,67,2,

            'K','W','X',           //KWX
            0x00,75,               //request code(下发其他玩家出牌)
            7,                     //package level
            0x00,25,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)
            2,60,1,65,2,
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
		MESSENGER->Start();
		while( !MESSENGER->Recv((INT8U *)_clientBuff,_clientBuffLen,75) ) {
			Sleep(DELAY);
		}
		assert( !memcmp(_clientBuff,MESSAGE+25,25) );
	}
};

class TestNetMessengerRecvOneFrontPartialPackage : public CTestMessenger {
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
            //60,1,                  //roomId
            //61,2,                  //seat
            //70,0,                  //card kind
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
		MESSENGER->Start();

		while( !MESSENGER->Recv((INT8U *)_clientBuff,_clientBuffLen) ) {
			Sleep(DELAY);
		}

		assert( false );
	}
};

class TestNetMessengerSendOutNotification : public CTestMessenger {
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
        NetMessenger::SetHandler(0);        //为了不影响后续的测试用例，是不是应该为TestCase增加Stop？
    }

	virtual void ClientActions() {
        memset(RecvBuf,0,256);

        MESSENGER->SetHandler((MsgHandler_t)&TestNetMessengerSendOutNotification::MsgHandler);
		MESSENGER->Start();
        //while( !MESSENGER->Recv((INT8U *)_clientBuff,_clientBuffLen) );
        Sleep(DELAY);

        assert( !memcmp(RecvBuf,MESSAGE,MESSAGE_LEN) );
    }
};

INT8U TestNetMessengerSendOutNotification::RecvBuf[256] = {0};

void testMessenger() {
	CTestCase *aCase;

	aCase = new TestNetMessengerRecvOneFullPackage();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	aCase = new TestNetMessengerRecvOneFullPackagePlusOneFrontPartialPackage();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	aCase = new TestNetMessengerRecvBehindPartialPackage();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

    aCase = new TestNetMessengerRecvDesignatedPackage();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

    aCase = new TestNetMessengerSendOutNotification();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

    /*因为始终收不到包，这个测试用例会导致阻塞，所以要放在最后*/
    aCase = new TestNetMessengerRecvOneFrontPartialPackage();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();
}

void startRun() {
	testBasicSocket();
	testMessenger();
}

