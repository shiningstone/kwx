
#include "stdio.h"
#include "CSockets.h"

void CSocket::Start() {
    SOCKET serSocket=socket(AF_INET,SOCK_STREAM,0);

    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(6000);
  
    bind( serSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR) );
    listen( serSocket, 5 );
  
    int len=sizeof(SOCKADDR);
    _connection=accept( serSocket, (SOCKADDR*)&_client, &len );
}

void CSocket::Stop() {
    closesocket(_connection);
}

int CSocket::Send(char *buf,int len) {
    int actLen = _send( buf, len );
    _log(SEND,buf,len);
    return actLen;
}

int CSocket::Recv(char *buf,int *len) {
    return _recv(buf, len);
}

int CSocket::_send(char *buf,int len) {
    int actLen = send(_connection, buf, strlen(buf)+1, 0 );
    return actLen;
}

int CSocket::_recv(char *buf,int *len) {
    if ( (*len = recv(_connection, buf, 128, 0))!=SOCKET_ERROR ) {
        _log(RECV,buf,*len);
    }
    return *len;
}

void CSocket::_log(int dir,char *buf,int len) {
    printf("%s (%d): %s\n", (dir==SEND)?"SEND":"RECV", len, buf);
}


void ServerSocket::Start() {
    SOCKET serSocket=socket(AF_INET,SOCK_STREAM,0);

    SOCKADDR_IN addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(6000);
  
    bind( serSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR) );
    listen( serSocket, 5 );
  
    int len=sizeof(SOCKADDR);
    _connection=accept( serSocket, (SOCKADDR*)&_client, &len );
}

void ClientSocket::Start() {
    _connection = socket(AF_INET,SOCK_STREAM,0);

    SOCKADDR_IN _client;
    _client.sin_addr.S_un.S_addr=inet_addr(_SERVER_IP);
    _client.sin_family=AF_INET;
    _client.sin_port=htons(6000);

    connect( _connection, (SOCKADDR*)&_client, sizeof(SOCKADDR) );
}
