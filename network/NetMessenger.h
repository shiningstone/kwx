
#ifndef __NET_MESSENGER__
#define __NET_MESSENGER__

#include "CSockets.h"
#include "./../protocol/MsgFormats.h"

class NetMessenger {
public:
	static NetMessenger *getInstance();

	void Start();
	bool Recv(INT8U *buf,int *len);
	int  Send(const INT8U *buf,int len);
protected:
	NetMessenger();
	~NetMessenger();

	static NetMessenger *_instance;
	static CSocket      *_socket;
private:
	const static int BUFF_SIZE = SOCKET_BUFF_SIZE;

	bool _keepListen;
	void _listen();

	INT8U _pkgBuf[BUFF_SIZE];   /* 环形接收缓冲  */
	int   _usedStart;          /* 数据起始偏移  */
	int   _usedLen;            /* 数据长度      */

	bool  _is_pkg_exists();
	bool  _get_pkg_from_buff(INT8U *pkg,int *pkgLen);
};

#endif
