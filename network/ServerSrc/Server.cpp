#include <WINSOCK2.H>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

#include "ServerSocket.h"

void main()
{
    //创建套接字
    WORD myVersionRequest;
    WSADATA wsaData;
    myVersionRequest=MAKEWORD(1,1);
    int err;
    err=WSAStartup(myVersionRequest,&wsaData);
    if (!err)
    {
        printf("已打开套接字\n");
    }
    else
    {
        //进一步绑定套接字
        printf("嵌套字未打开!");
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
    SOCKET serSocket=socket(AF_INET,SOCK_STREAM,0);//创建了可识别套接字

    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//ip地址
    addr.sin_port=htons(6000);//绑定端口
  
    bind(serSocket,(SOCKADDR*)&addr,sizeof(SOCKADDR));//绑定完成
    listen(serSocket,5);//其中第二个参数代表能够接收的最多的连接数
  
    SOCKADDR_IN clientsocket;
    int len=sizeof(SOCKADDR);
    SOCKET serConn=accept( serSocket, (SOCKADDR*)&clientsocket, &len );//如果这里不是accept而是conection的话。。就会不断的监听

    int i = 0;
    while (1)
    {
        char sendBuf[100];
        sprintf(sendBuf,"welcome %s to bejing(%d)",inet_ntoa(clientsocket.sin_addr),i);//找对对应的IP并且将这行字打印到那里
        send(serConn,sendBuf,strlen(sendBuf)+1,0);

        char receiveBuf[100] = {0};//接收
        if ( recv(serConn,receiveBuf,strlen(receiveBuf)+1,0)!=SOCKET_ERROR ) {
            printf("%s(%d)\n",receiveBuf,i++);
        }
    }

    closesocket(serConn);//关闭
#endif
    WSACleanup();//释放资源的操作
}