#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>

#include "./CTestSocket.h"

void CTestSocket::Start() {
    CTestCase::Start();

    ClearClientBuf();
	ClearServerBuf();

	CSocket::Init();
}

void CTestSocket::Stop() {
    CSocket::Quit();

    CTestCase::Stop();
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

void CTestMessenger::Start() {
    CTestSocket::Start();

    MESSENGER = NetMessenger::getInstance();
}

void CTestMessenger::Stop() {
#if 0
    NetMessenger::destroyInstance();
    MESSENGER = 0;
#endif
    CTestSocket::Stop();
}