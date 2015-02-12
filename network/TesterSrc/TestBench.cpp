#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>
#include "./../CSockets.h"
#include "./../GameSocket.h"
#include "./CTestCase.h"

class TestSingleSendAndRecv : public CTestCase {
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

class TestGameSocket : public CTestCase {
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

class TestEmpty : public CTestCase {
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

	aCase = new TestGameSocket();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	while(1);
}

