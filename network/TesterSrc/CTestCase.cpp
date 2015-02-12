#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>

#include "./CTestCase.h"

void CTestCase::ClearClientBuf() {
	memset( _clientBuff, 0, SOCK_BUFF_LEN );
	_clientBuffLen = 0;
}

void CTestCase::ClearServerBuf() {
	memset( _serverBuff, 0, SOCK_BUFF_LEN );
	_serverBuffLen = 0;
}

CTestCase::CTestCase() {
	ClearClientBuf();
	ClearServerBuf();
}

void CTestCase::Start() {
	CSocket::Init();
}

void CTestCase::Stop() {
    CSocket::Quit();
}

int CTestCase::Execute() {
    std::thread Tserver( &CTestCase::ServerActions,this );
    std::thread Tclient( &CTestCase::ClientActions,this );

    Tserver.join();
    Tclient.join();

	return 0;
}
