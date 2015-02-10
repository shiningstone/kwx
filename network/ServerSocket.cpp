
#include "stdio.h"
#include "ServerSocket.h"


ServerSocket::ServerSocket() {
    SOCKET serSocket=socket(AF_INET,SOCK_STREAM,0);

    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(6000);
  
    bind( serSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR) );
    listen( serSocket, 5 );
  
    int len=sizeof(SOCKADDR);
    _connection=accept( serSocket, (SOCKADDR*)&_client, &len );//如果这里不是accept而是conection的话。。就会不断的监听
}

ServerSocket::~ServerSocket() {
    closesocket(_connection);
}

int ServerSocket::Send(char *buf,int len) {
    int actLen = _send( buf, len );
    _log(SEND,buf,len);
    return actLen;
}

int ServerSocket::Recv(char *buf,int *len) {
    return _recv(buf, len);
}

int ServerSocket::_send(char *buf,int len) {
    int actLen = send(_connection, buf, strlen(buf)+1, 0 );
    return actLen;
}

int ServerSocket::_recv(char *buf,int *len) {
    if ( (*len = recv(_connection, buf, 128, 0))!=SOCKET_ERROR ) {
        _log(RECV,buf,*len);
    }
    return *len;
}

void ServerSocket::_log(int dir,char *buf,int len) {
    printf("%s (%d): %s\n", (dir==SEND)?"SEND":"RECV", len, buf);
}
