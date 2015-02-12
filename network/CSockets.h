
#ifndef __SOCKET_H__
#define __SOCKET_H__

#pragma once   
  
#ifdef WIN32   
#include <windows.h>   
#include <WinSock.h>   
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

#define BLOCKSECONDS  30              // INIT��������ʱ��   
#define SOCK_BUFF_LEN 128

class CSocket {
public:
    virtual void Start() = 0;
    void Stop();

	static void Init();
	static void Quit();
	static bool hasError();

    int Send(char *buf,int len);
    int Recv(char *buf,int *len);
protected:
    SOCKET _connection;
	bool   _keepAlive;
	int    _blockSecond;

	static bool gInited;

    int _send(char *buf,int len);
    int _recv(char *buf,int *len);
    void _log(const char *fmt,...);
    void _log(int dir,char *buf,int len);

    static const int SEND = 0;
    static const int RECV = 1;
	static const int SOCKET_PORT = 6000;
};

class ServerSocket : public CSocket {
public:
	virtual void Start();
};

#define _SERVER_IP "127.0.0.1"
class ClientSocket : public CSocket {
public:
	virtual void Start();
};


#endif