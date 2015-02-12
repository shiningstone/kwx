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

		SERVER.Send("1",strlen("1"));

		SERVER.Recv(_buff,&_buffLen);
		assert(!strcmp(_buff,"2"));

		SERVER.Stop();
	}

	virtual void ClientActions() {
		CLIENT.Start();

		CLIENT.Recv(_buff,&_buffLen);
		assert( !strcmp(_buff,"1") );

		CLIENT.Send("2",strlen("2"));

		CLIENT.Stop();
	}
};

class TestGameSocket : public CTestCase {
	virtual void ServerActions() {
		SERVER.Start();

		SERVER.Send("ABCDEFGH",8);
		SERVER.Recv(_buff,&_buffLen);

		SERVER.Stop();
	}

	virtual void ClientActions() {
		CGameSocket client;
		client.Create(_SERVER_IP, 6000);
		
		client.ForceSend("abcdefgh",8);

		_buffLen = 8;
		memset(_buff,0,8);
		client.ForceRecv(_buff,_buffLen);
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
}

