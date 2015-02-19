
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
	int  Send(const INT8U *buf,int len);
protected:
	NetMessenger();
	~NetMessenger();

	static NetMessenger *_instance;
	static bool         _keepListen;
	static CSocket      *_socket;
private:
	const static int BUFF_SIZE = SOCKET_BUFF_SIZE;

	void _listen();

	INT8U _pkgBuf[BUFF_SIZE];   /* ���ν��ջ���  */
	int   _usedStart;           /* ������ʼƫ��  */
	int   _usedLen;             /* ���ݳ���      */

	bool  _is_pkg_header_exists();    /* NetMessengerĿǰֻ֧�����н��� */
	bool  _is_pkg_body_completed(int pkgLen);
	void  _get_pkg_from_buffer(INT8U *start,int len);
};

#endif
