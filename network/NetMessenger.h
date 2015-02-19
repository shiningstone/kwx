
#ifndef __NET_MESSENGER__
#define __NET_MESSENGER__

#include "CSockets.h"
#include "./../protocol/MsgFormats.h"

class NetMessenger {
public:
	static NetMessenger *getInstance();
	static void          destroyInstance();

	void Start();
	bool Recv(INT8U *buf,int &len);
    bool Recv(INT8U *pkg,int &pkgLen,RequestId_t request);
	int  Send(const INT8U *buf,int len);

    void ClearRecvBuf();
protected:
	NetMessenger();
	~NetMessenger();

	static NetMessenger *_instance;
	static bool         _keepListen;
	static CSocket      *_socket;
private:
    /*** ���ĸ�ʽʶ��   ***/
    int   _get_available_pkg_len();      

    /*** ���ν��ջ����� ***/
	void _listen();

    const static int BUFF_SIZE = SOCKET_BUFF_SIZE;

	INT8U _pkgBuf[BUFF_SIZE];
	int   _inStart;             /* ����������ʼƫ��  */
	int   _outStart;            /* ���ݴ洢��ʼƫ��  */
	int   _usedLen();           /* �������ݳ���      */
	void  _get_pkg_from_buffer(INT8U *start,int len);
};

#endif
