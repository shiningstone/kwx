
#include "ClientSocket.h"
#include "stdio.h"

ClientSocket::ClientSocket() {
    _socket = socket(AF_INET,SOCK_STREAM,0);

    SOCKADDR_IN _client;
    _client.sin_addr.S_un.S_addr=inet_addr(_SERVER_IP);
    _client.sin_family=AF_INET;
    _client.sin_port=htons(6000);

    connect( _socket, (SOCKADDR*)&_client, sizeof(SOCKADDR) );
}

ClientSocket::~ClientSocket() {
    closesocket(_socket);
}

int ClientSocket::Send(char *buf,int len) {
    int actLen = _send( buf, len );
    _log(SEND,buf,len);
    return actLen;
}

int ClientSocket::Recv(char *buf,int *len) {
    return _recv(buf, len);
}

int ClientSocket::_send(char *buf,int len) {
    int actLen = send(_socket, buf, strlen(buf)+1, 0 );
    return actLen;
}

int ClientSocket::_recv(char *buf,int *len) {
    if ( (*len = recv(_socket, buf, 100, 0))!=SOCKET_ERROR ) {
        _log(RECV,buf,*len);
    }
    return *len;
}

void ClientSocket::_log(int dir,char *buf,int len) {
    printf("%s : %s\n", (dir==SEND)?"SEND":"RECV", buf);
}
