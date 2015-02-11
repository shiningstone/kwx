#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>

#include "./CTestCase.h"

void CTestCase::Start() {
    int err;
    WORD versionRequired;
    WSADATA wsaData;
    versionRequired=MAKEWORD(1,1);
    err=WSAStartup(versionRequired,&wsaData);
    if (!err)
    {
        printf("嵌套字已经打开!\n");
    }
    else
    {
        printf("嵌套字打开失败!\n");
        return;//结束
    }
}

void CTestCase::Stop() {
    WSACleanup();
}

int CTestCase::Execute() {
    std::thread Tserver( &CTestCase::ServerActions,this );
    std::thread Tclient( &CTestCase::ClientActions,this );

    Tserver.join();
    Tclient.join();

	return 0;
}
