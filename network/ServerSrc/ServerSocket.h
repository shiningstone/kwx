
#ifndef _SERVER_SOCKET_
#define _SERVER_SOCKET_

#include <WINSOCK2.H>

class ServerSocket {
public:
    ServerSocket();
    ~ServerSocket();

    int Send(char *buf,int len);
    int Recv(char *buf,int *len);
private:
    SOCKET _connection;
    SOCKADDR_IN _client;

    int _send(char *buf,int len);
    int _recv(char *buf,int *len);
    void _log(int dir,char *buf,int len);

    static const int SEND = 0;
    static const int RECV = 1;
};

#endif