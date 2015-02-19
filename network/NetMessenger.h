
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
	const static int BUFF_SIZE = SOCKET_BUFF_SIZE;

	void _listen();

	INT8U _pkgBuf[BUFF_SIZE];   /* 环形接收缓冲  */
	int   _inStart;             /* 数据输入起始偏移  */
	int   _outStart;            /* 数据存储起始偏移  */
	int   _usedLen();           /* 已有数据长度      */

	bool  _is_pkg_header_exists();    /* NetMessenger目前只支持下行接收 */
	bool  _is_pkg_body_completed(int pkgLen);
	void  _get_pkg_from_buffer(INT8U *start,int len);
};

#endif
