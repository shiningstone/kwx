#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>
#include "./../../network/CSockets.h"
#include "./../../network/GameSocket.h"
#include "./CTestNetwork.h"

class TestSingleSendAndRecv : public CTestNetwork {
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

		CLIENT.Recv(_clientBuff,&_clientBuffLen);
		assert( !strcmp(_clientBuff,"to client") );
		ClearClientBuf();

		CLIENT.Send("to server",strlen("to server"));

		CLIENT.Stop();
	}
};

class TestGameSocket : public CTestNetwork {
	virtual void ServerActions() {
		SERVER.Start();

		_serverBuffLen = 10;
		memset(_serverBuff,0,8);
		SERVER.Send("to client",strlen("to client"));
		SERVER.Recv(_serverBuff,&_serverBuffLen);
		ClearServerBuf();

		SERVER.Stop();
	}

	virtual void ClientActions() {
		CGameSocket client;
		client.Create(_SERVER_IP, 6000);
		
		client.ForceSend("to server",strlen("to client"));

		_clientBuffLen = 10;
		client.ForceRecv(_clientBuff,_clientBuffLen);
		ClearClientBuf();
	}
};

class TestByteOrder : public CTestNetwork {
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

class TestEmpty : public CTestNetwork {
	virtual void ServerActions() {
	}

	virtual void ClientActions() {
	}
};

void startRun() {
	CTestCase *aCase;

	aCase = new TestSingleSendAndRecv();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	aCase = new TestByteOrder();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	aCase = new TestGameSocket();   //这个case执行以后socket start失败
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	while(1);
}

