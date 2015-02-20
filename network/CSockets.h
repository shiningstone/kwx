
#ifndef __SOCKET_H__
#define __SOCKET_H__

#pragma once   
  
#ifdef WIN32   
#include <windows.h>   
#include <WinSock.h>   
//#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#else   
#include <sys/socket.h>   
#include <fcntl.h>   
#include <errno.h>   
#include <netinet/in.h>   
#include <arpa/inet.h>   
  
#define SOCKET int   
#define SOCKET_ERROR -1   
#define INVALID_SOCKET -1   
#endif   

#define SERVER_IP    "127.0.0.1"
#define SOCKET_PORT  6000

#define BLOCKSECONDS  30              // INIT��������ʱ��   
#define SOCKET_BUFF_SIZE 128

class Logger;
class CSocket {
public:
    CSocket();
    ~CSocket();

    virtual void Start() = 0;
    void Stop();

	static void Init();
	static void Quit();
	static bool hasError();

    int Send(char *buf,int len);
    int Recv(char *buf,int *len,int bufSize=SOCKET_BUFF_SIZE);
protected:
    SOCKET _connection;
	bool   _keepAlive;
	int    _blockSecond;
	bool   _keepListen;

    Logger *_logger;

    void _log(const char *fmt,...);
    void _log(int dir,char *buf,int len);

    static const int SEND = 0;
    static const int RECV = 1;

#ifdef WIN32
	static bool gWSAInited;
#endif
};

class ServerSocket : public CSocket {
public:
	virtual void Start();
};

class ClientSocket : public CSocket {
public:
	virtual void Start();
};

#endif