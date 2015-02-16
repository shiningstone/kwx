#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>
#include "./../../network/CSockets.h"
#include "./../../network/GameSocket.h"
#include "./CTestNetwork.h"

#define DELAY 100

class TestSingleSendAndRecv : public CTestNetwork {
	virtual void ServerActions() {
		SERVER.Start();

		SERVER.Send("to client",strlen("to client"));

		SERVER.Recv(_serverBuff,&_serverBuffLen);
		Sleep(DELAY);
		assert(!strcmp(_serverBuff,"to server"));
		ClearServerBuf();

		SERVER.Stop();
	}

	virtual void ClientActions() {
		CLIENT.Start();

		CLIENT.Recv(_clientBuff,&_clientBuffLen);
		Sleep(DELAY);
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
		Sleep(DELAY);
		ClearServerBuf();

		SERVER.Stop();
	}

	virtual void ClientActions() {
		CGameSocket client;
		client.Create(_SERVER_IP, 6000);
		
		client.ForceSend("to server",strlen("to client"));

		_clientBuffLen = 10;
		client.ForceRecv(_clientBuff,_clientBuffLen);
		Sleep(DELAY);
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
		Sleep(DELAY);
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
		Sleep(DELAY);
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

class TestMultipleSendAndRecv : public CTestNetwork {
	virtual void ServerActions() {
		char buf[32] = {0};
		int i        = 0;

		SERVER.Start();

		SERVER.Recv(_serverBuff,&_serverBuffLen);
		Sleep(DELAY);
		//assert(!strcmp(_serverBuff,"to server"));
		ClearServerBuf();

		sprintf_s(buf,"to client %d",i++);
		SERVER.Send(buf,strlen(buf));

		sprintf_s(buf,"to client %d",i++);
		SERVER.Send(buf,strlen(buf));

		while(1);
		SERVER.Stop();
	}

	virtual void ClientActions() {
		char buf[32] = {0};
		int i        = 0;

		CLIENT.Start();

		CLIENT.Recv(_clientBuff,&_clientBuffLen);
		Sleep(DELAY);
		//assert( !strcmp(_clientBuff,"to client") );
		ClearClientBuf();

		sprintf_s(buf,"to server %d",i++);
		SERVER.Send(buf,strlen(buf));

		sprintf_s(buf,"to server %d",i++);
		SERVER.Send(buf,strlen(buf));

		while(1);
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
#if 1
	aCase = new TestSingleSendAndRecv();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	aCase = new TestByteOrder();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();
#endif
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
	printf("------------test end---------------\n");
	while(1);
}

