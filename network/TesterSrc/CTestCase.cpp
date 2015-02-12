#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>

#include "./CTestCase.h"

CTestCase::CTestCase() {
	memset( _buff, 0, SOCK_BUFF_LEN );
	_buffLen = 0;
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
