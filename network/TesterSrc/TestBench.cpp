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
    err=WSAStartup(versionRequired,&wsaData);//Э���İ汾��Ϣ
    if (!err)
    {
        printf("Ƕ�����Ѿ���!\n");
    }
    else
    {
        printf("Ƕ���ִ�ʧ��!\n");
        return;//����
    }
}

void startRun() {
    enableSocket();

    std::thread server(VirtualServer);
    std::thread client(VirtualClient);

    server.join();
    client.join();
}
