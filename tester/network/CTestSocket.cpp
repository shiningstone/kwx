#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>

#include "./CTestSocket.h"

CTestSocket::CTestSocket() {
}

void CTestSocket::Start() {
	printf("test start *************************************************\n");
	ClearClientBuf();
	ClearServerBuf();

	CSocket::Init();
}

void CTestSocket::Stop() {
    CSocket::Quit();
	printf("*************************************************** test end\n\n");
}

void CTestSocket::ClearClientBuf() {
	memset( _clientBuff, 0, SOCKET_BUFF_SIZE );
	_clientBuffLen = 0;
}

void CTestSocket::ClearServerBuf() {
	memset( _serverBuff, 0, SOCKET_BUFF_SIZE );
	_serverBuffLen = 0;
}

int CTestSocket::Execute() {
    std::thread Tserver( &CTestSocket::ServerActions,this );
    std::thread Tclient( &CTestSocket::ClientActions,this );

    Tserver.join();
    Tclient.join();

	return 0;
}

CTestMessenger::CTestMessenger() {
	MESSENGER = NetMessenger::getInstance();
}