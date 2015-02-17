
#include "stdio.h"
#include "CSockets.h"

/*
	windows下调用socket需要启动Windows Sockets Asynchronous
*/
#ifdef WIN32
bool CSocket::gWSAInited = false;
#endif

void CSocket::Init() {
#ifdef WIN32
	if(!gWSAInited) {
		WORD    versionRequired = MAKEWORD(2,0);
		WSADATA wsaData;

		if (!WSAStartup(versionRequired,&wsaData)) {
			printf("WSAStartup successfully!\n");
			gWSAInited = true;
		} else {
			printf("WSAStartup failed!\n");
		}
	}
#endif
}

void CSocket::Quit() {
#ifdef WIN32
    WSACleanup();
	gWSAInited = false;
#endif
}

void CSocket::Stop() {
#ifdef WIN32
	closesocket(_connection);
#else
	close(_connection);
#endif
}

int CSocket::Send(char *buf,int len) {
    int actLen = send(_connection, buf, len, 0 );;
    _log(SEND,buf,actLen);
    return actLen;
}

#include <thread>
int CSocket::Recv(char *buf,int *len) {
	while ( (*len = recv(_connection, buf, SOCK_BUFF_LEN, 0))<=0 ) {
	}
	_log(RECV,buf,*len);

	return *len;
}

/******************************************************
	ServerSocket
******************************************************/
void ServerSocket::Start() {
	Init();

    SOCKET serSocket=socket(AF_INET,SOCK_STREAM,0);

    SOCKADDR_IN local;
    local.sin_family=AF_INET;
    local.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    local.sin_port=htons(SOCKET_PORT);
  
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
	Init();

	_keepAlive   = false;
	_blockSecond = BLOCKSECONDS;

	_connection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
    if(_connection == INVALID_SOCKET) {  
        Stop();  
    }  

	// 设置SOCKET为KEEPALIVE
    if(_keepAlive) {
        int optval=1;  
        if(setsockopt(_connection, SOL_SOCKET, SO_KEEPALIVE, (char *) &optval, sizeof(optval))) {  
            Stop();  
        }  
    }  

	// 设置为非阻塞方式   
#ifdef WIN32   
    DWORD nMode = 1;  
    if (ioctlsocket(_connection, FIONBIO, &nMode) == SOCKET_ERROR) {  
        Stop();  
    }  
#else   
    fcntl(_connection, F_SETFL, O_NONBLOCK);  
#endif   

	SOCKADDR_IN server;
    server.sin_addr.S_un.S_addr=inet_addr(SERVER_IP);
    server.sin_family=AF_INET;
    server.sin_port=htons(SOCKET_PORT);

	int result = connect( _connection, (SOCKADDR*)&server, sizeof(SOCKADDR) );
	if ( result==SOCKET_ERROR ) {
        if (hasError()) {  
            Stop();  
        }  else  {// WSAWOLDBLOCK   
            timeval timeout;  
            timeout.tv_sec  = _blockSecond;  
            timeout.tv_usec = 0;  
            fd_set writeset, exceptset;  
            FD_ZERO(&writeset);  
            FD_ZERO(&exceptset);  
            FD_SET(_connection, &writeset);  
            FD_SET(_connection, &exceptset);  
  
            int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);  
            if (ret == 0 || ret < 0) {  
                Stop();  
            } else  {  
                if( FD_ISSET(_connection, &exceptset) ) {    // or (!FD_ISSET(m_sockClient, &writeset)   
                    Stop();  
                }  
            }  
        }  
	} else {
		_log("%s : connect() error!\n",__FUNCTION__);
  	}
}

bool CSocket::hasError()  
{  
#ifdef WIN32   
    if(WSAGetLastError() != WSAEWOULDBLOCK) {  
#else   
    if(errno != EINPROGRESS && errno != EAGAIN) {  
#endif   
        return true;  
    }  
  
    return false;  
}  
  
/***************************************************
				辅助函数
***************************************************/
void CSocket::_log(const char *fmt,...) {
    va_list arg;

    va_start(arg,fmt);
    printf(fmt,arg);
    va_end(arg);
}

#include <ctype.h>
void CSocket::_log(int dir,char *buf,int len) {
	printf("%s (%d): ", (dir==SEND)?"SEND":"RECV", len);

	if( isalnum(buf[0]) ) {
	    printf("%s\n", buf);
	} else {
		printf("\n\t");
		for(int i=0; i<len; i++) {
			printf("%02x ",buf[i]);
			if( (i+1)%16==0 ) {
				printf("\n\t");
			}
		}
		printf("\n");
	}
}
