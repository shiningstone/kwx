#include <WINSOCK2.H>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

#include "./../ClientSocket.h"

void VirtualClient()
{
    int err;
    WORD versionRequired;
    WSADATA wsaData;
    versionRequired=MAKEWORD(1,1);
    err=WSAStartup(versionRequired,&wsaData);//协议库的版本信息
    if (!err)
    {
        printf("客户端嵌套字已经打开!\n");
    }
    else
    {
        printf("客户端的嵌套字打开失败!\n");
        return;//结束
    }

    ClientSocket *aSocket = new ClientSocket();

    char buf[128] = {0};
    int  len = 0;
    aSocket->Recv(buf,&len);
    aSocket->Send("got message from server",strlen("got message from server"));

    while(1);

    WSACleanup();
}