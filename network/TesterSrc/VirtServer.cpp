#include "stdio.h"

#include "./../ServerSocket.h"

void VirtualServer() {
    ServerSocket *aSocket = new ServerSocket();
    aSocket->Send("this is a test",strlen("this is a test"));

    while(1) {
        char recvBuf[100] = {0};
        int recvLen = 0;
        aSocket->Recv(recvBuf,&recvLen);
    }
}

