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
    err=WSAStartup(versionRequired,&wsaData);//Э���İ汾��Ϣ
    if (!err)
    {
        printf("�ͻ���Ƕ�����Ѿ���!\n");
    }
    else
    {
        printf("�ͻ��˵�Ƕ���ִ�ʧ��!\n");
        return;//����
    }

    ClientSocket *aSocket = new ClientSocket();

    char buf[128] = {0};
    int  len = 0;
    aSocket->Recv(buf,&len);
    aSocket->Send("got message from server",strlen("got message from server"));

    while(1);

    WSACleanup();
}