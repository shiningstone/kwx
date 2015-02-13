#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>

#include "./CTestNetwork.h"

CTestNetwork::CTestNetwork() {
}

void CTestNetwork::Start() {
	ClearClientBuf();
	ClearServerBuf();

	CSocket::Init();
}

void CTestNetwork::Stop() {
    CSocket::Quit();
}

void CTestNetwork::ClearClientBuf() {
	memset( _clientBuff, 0, SOCK_BUFF_LEN );
	_clientBuffLen = 0;
}

void CTestNetwork::ClearServerBuf() {
	memset( _serverBuff, 0, SOCK_BUFF_LEN );
	_serverBuffLen = 0;
}

int CTestNetwork::Execute() {
    std::thread Tserver( &CTestNetwork::ServerActions,this );
    std::thread Tclient( &CTestNetwork::ClientActions,this );

    Tserver.join();
    Tclient.join();

	return 0;
}
