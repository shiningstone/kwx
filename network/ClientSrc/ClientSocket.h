
#ifndef _CLIENT_SOCKET_
#define _CLIENT_SOCKET_

#include <WINSOCK2.H>

#define _SERVER_IP "127.0.0.1"

class ClientSocket {
public:
    ClientSocket();
    ~ClientSocket();

    int Send(char *buf,int len);
    int Recv(char *buf,int *len);
private:
    SOCKET      _socket;
    SOCKADDR_IN _client;

    int _send(char *buf,int len);
    int _recv(char *buf,int *len);
    void _log(int dir,char *buf,int len);

    static const int SEND = 0;
    static const int RECV = 1;
};

#endif
