#include <WINSOCK2.H>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

#include "ServerSocket.h"

void main()
{
    //�����׽���
    WORD myVersionRequest;
    WSADATA wsaData;
    myVersionRequest=MAKEWORD(1,1);
    int err;
    err=WSAStartup(myVersionRequest,&wsaData);
    if (!err)
    {
        printf("�Ѵ��׽���\n");
    }
    else
    {
        //��һ�����׽���
        printf("Ƕ����δ��!");
        return;
    }

    ServerSocket *aSocket = new ServerSocket();
    aSocket->Send("this is a test",strlen("this is a test"));

    while(1) {
        char recvBuf[100] = {0};
        int recvLen = 0;
        aSocket->Recv(recvBuf,&recvLen);
    }

#if 0
    SOCKET serSocket=socket(AF_INET,SOCK_STREAM,0);//�����˿�ʶ���׽���

    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//ip��ַ
    addr.sin_port=htons(6000);//�󶨶˿�
  
    bind(serSocket,(SOCKADDR*)&addr,sizeof(SOCKADDR));//�����
    listen(serSocket,5);//���еڶ������������ܹ����յ�����������
  
    SOCKADDR_IN clientsocket;
    int len=sizeof(SOCKADDR);
    SOCKET serConn=accept( serSocket, (SOCKADDR*)&clientsocket, &len );//������ﲻ��accept����conection�Ļ������ͻ᲻�ϵļ���

    int i = 0;
    while (1)
    {
        char sendBuf[100];
        sprintf(sendBuf,"welcome %s to bejing(%d)",inet_ntoa(clientsocket.sin_addr),i);//�ҶԶ�Ӧ��IP���ҽ������ִ�ӡ������
        send(serConn,sendBuf,strlen(sendBuf)+1,0);

        char receiveBuf[100] = {0};//����
        if ( recv(serConn,receiveBuf,strlen(receiveBuf)+1,0)!=SOCKET_ERROR ) {
            printf("%s(%d)\n",receiveBuf,i++);
        }
    }

    closesocket(serConn);//�ر�
#endif
    WSACleanup();//�ͷ���Դ�Ĳ���
}