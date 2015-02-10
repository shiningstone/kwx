#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")

extern void VirtualServer();
extern void VirtualClient();

void enableSocket() {
    int err;
    WORD versionRequired;
    WSADATA wsaData;
    versionRequired=MAKEWORD(1,1);
    err=WSAStartup(versionRequired,&wsaData);//协议库的版本信息
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

void startRun() {
    enableSocket();

    std::thread server(VirtualServer);
    std::thread client(VirtualClient);

    server.join();
    client.join();
}
