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

#ifndef CHECKF
#define CHECKF(x) 
#endif

#define _MAX_MSGSIZE 16 * 1024       // �ݶ�һ����Ϣ���Ϊ16k   
#define BLOCKSECONDS 30              // INIT��������ʱ��   
#define INBUFSIZE    (3)       //? ����ߴ�������汨�����   �������ݵĻ���   
#define OUTBUFSIZE   (4)        //? ����ߴ�������汨������� �������ݵĻ��棬��������8Kʱ��FLUSHֻ��ҪSENDһ��   
  
class CGameSocket {  
public:  
    CGameSocket(void);  
    bool    Create(const char* pszServerIP, int nServerPort, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);  
    bool    SendMsg(void* pBuf, int nSize);  
    bool    ReceiveMsg(void* pBuf, int* nSize);  
    bool    Flush(void);  
    bool    Check(void);  
    void    Destroy(void);  
    SOCKET  GetSocket(void) const { return m_sockClient; }  

    void    _log(int dir,char *buf,int len);
	void    ForceSend(void* pBuf, int nSize);
	void    ForceRecv(void* pBuf, int nSize);

    static const int SEND = 0;
    static const int RECV = 1;
private:  
    bool    recvFromSock(void);     // �������ж�ȡ�����ܶ������   
    bool    hasError();         // �Ƿ�������ע�⣬�첽ģʽδ��ɷǴ���   
    void    closeSocket();  
  
    SOCKET  m_sockClient;  
  
    // �������ݻ���   
    char    m_bufOutput[OUTBUFSIZE];    //? ���Ż�Ϊָ������   
    int     m_nOutbufLen;  
  
    // ���λ�����   
    char    m_bufInput[INBUFSIZE];  
    int     m_nInbufLen;  
    int     m_nInbufStart;              // INBUFʹ��ѭ��ʽ���У��ñ���Ϊ������㣬0 - (SIZE-1)   
};  
