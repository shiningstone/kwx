#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>
#include "./../CSockets.h"
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

class TestEmpty : public CTestCase {
	virtual void ServerActions() {
	}
	virtual void ClientActions() {
	}
};

void startRun() {
	TestSingleSendAndRecv aCase;
	aCase.Start();
	aCase.Execute();
	aCase.Stop();
}

