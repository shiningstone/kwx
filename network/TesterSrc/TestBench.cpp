#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>
#include "./../CSockets.h"
#include "./CTestCase.h"

class TestSingle : public CTestCase {
	virtual void ServerActions() ;
	virtual void ClientActions() ;
};

void TestSingle::ServerActions() {
	SERVER.Start();

	char recvBuf[100] = {0};
    int recvLen = 0;

    SERVER.Send("1",strlen("1"));

    SERVER.Recv(recvBuf,&recvLen);
	assert(!strcmp(recvBuf,"2"));

	SERVER.Stop();
}

void TestSingle::ClientActions() {
	CLIENT.Start();

	char buf[128] = {0};
    int  len = 0;

    CLIENT.Recv(buf,&len);
	assert( !strcmp(buf,"1") );

	CLIENT.Send("2",strlen("2"));

	CLIENT.Stop();
}

void startRun() {
	TestSingle aCase;

	aCase.Start();
	aCase.Execute();
	aCase.Stop();
}

