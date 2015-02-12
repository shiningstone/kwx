
#include "stdio.h"
#include "CSockets.h"

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

void CSocket::_log(const char *fmt,...) {
    va_list arg;

    va_start(arg,fmt);
    printf(fmt,arg);
    va_end(arg);
}

void CSocket::_log(int dir,char *buf,int len) {
    printf("%s (%d): %s\n", (dir==SEND)?"SEND":"RECV", len, buf);
}

/******************************************************
	ServerSocket
******************************************************/
void ServerSocket::Start() {
    SOCKET serSocket=socket(AF_INET,SOCK_STREAM,0);

    SOCKADDR_IN local;
    local.sin_family=AF_INET;
    local.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    local.sin_port=htons(6000);
  
	if ( bind( serSocket, (SOCKADDR*)&local, sizeof(SOCKADDR) ) == 0 ) {
	    listen( serSocket, 5 );
	} else {
		_log("%s : bind() error!\n",__FUNCTION__);
	}
  
	SOCKADDR client = {0};
    int      len=sizeof(SOCKADDR);
    _connection = accept( serSocket, (SOCKADDR*)&client, &len );
	if( _connection==INVALID_SOCKET ) {
		_log("%s : accept() error!\n",__FUNCTION__);
	}
}

/******************************************************
	ClientSocket
******************************************************/
void ClientSocket::Start() {
    _connection = socket(AF_INET,SOCK_STREAM,0);

    SOCKADDR_IN server;
    server.sin_addr.S_un.S_addr=inet_addr(_SERVER_IP);
    server.sin_family=AF_INET;
    server.sin_port=htons(6000);

	if ( connect( _connection, (SOCKADDR*)&server, sizeof(SOCKADDR) )== 0 ) {
		return;
	} else {
		_log("%s : connect() error!\n",__FUNCTION__);
  	}
}
