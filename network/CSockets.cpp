
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
    _connection = INVALID_SOCKET; 
}

int CSocket::Send(char *buf,int len) {
    if(_connection!=INVALID_SOCKET) {
        int actLen = send(_connection, buf, len, 0 );
        _log(SEND,buf,actLen);
        return actLen;
    } else {
        return 0;
    }
}

#include <thread>
int CSocket::Recv(char *buf,int *len,int bufSize) {
	while ( (*len = recv(_connection, buf, bufSize, 0))<=0 ) {
	}

	_log(RECV,buf,*len);
	return *len;
}

/******************************************************
	ServerSocket
******************************************************/
#ifdef __UNIT_TEST__
void ServerSocket::Start(const char *serverIp,int port) {
	Init();

    SOCKET serSocket=socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    local.sin_port=htons(port);
  
	if ( bind( serSocket, (sockaddr*)&local, sizeof(sockaddr) ) == 0 ) {
	    listen( serSocket, 5 );
	} else {
		LOGGER_WRITE("%s : bind() error!\n",__FUNCTION__);
	}
  
	sockaddr client = {0};
    int      len=sizeof(sockaddr);
    _connection = accept( serSocket, (sockaddr*)&client, &len );
	if( _connection==INVALID_SOCKET ) {
		LOGGER_WRITE("%s : accept() error!\n",__FUNCTION__);
	}
}
#endif
/******************************************************
	ClientSocket
******************************************************/
void ClientSocket::Start(const char *serverIp,int port) {
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

	sockaddr_in server;
    server.sin_addr.s_addr=inet_addr(serverIp);
    server.sin_family=AF_INET;
    server.sin_port=htons(port);

	int result = connect( _connection, (sockaddr*)&server, sizeof(sockaddr) );
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
		LOGGER_WRITE("%s : connect() error!\n",__FUNCTION__);
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
CSocket::CSocket() {
    _logger = LOGGER_REGISTER("CSocket");
}

CSocket::~CSocket() {
    LOGGER_DEREGISTER(_logger);
}

void CSocket::_log(int dir,char *buf,int len) {
#ifdef __DISABLE_LOGGER__
#else
    char temp[256] = {0};
    int usedBytes  = 0;

    sprintf(temp, "%s (%03d): ", (dir==0)?"SEND":"RECV", len);
    usedBytes = strlen(" (): ")+4+3;

    if( isalpha(buf[0]) && strncmp(buf,"KWX", 3) ) {
        sprintf(temp+usedBytes,"%s\n",buf);
        LOGGER_WRITE( temp );
    } else {
        sprintf(temp+usedBytes,"\n");
        LOGGER_WRITE( temp );
        _logger->WriteArray( buf, len );
    }
#endif
}
