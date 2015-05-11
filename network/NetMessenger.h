
#ifndef __NET_MESSENGER__
#define __NET_MESSENGER__

/*******************************************************
    NetMessenger

        Recv接口：  
            用于直接接收message供调用者使用；
        SetHandler接口：   
            用于启动一个进程，收到message时触发回调函数。
*******************************************************/


#include "./../utils/UtilBasic.h"

class CSocket;
#define SOCKET_BUFF_SIZE 128

class NetMessenger {
public:
	static NetMessenger *getInstance();
	static void          destroyInstance();
    static void          SetHandler(MsgHandler_t func);

	void Start();
	bool Recv(INT8U *buf,int &len);
    bool Recv(INT8U *buf,int &len,INT16U request);
	int  Send(const INT8U *buf,int len);

    void ClearRecvBuf();
protected:
	NetMessenger();
	~NetMessenger();

	static NetMessenger *_instance;
	static CSocket      *_socket;
	static bool         _keepListen;
    static MsgHandler_t _handle_msg;
private:
    /*** 报文自动接收   ***/
    bool _is_kwx_exist();
    void _collect_packages();

    /*** 报文格式识别   ***/
    int   _get_available_pkg_len();
    INT16U _get_request_id(const INT8U *pkg);

    /*** 环形接收缓冲区 ***/
	void _collect_bytes();

    const static int BUFF_SIZE = SOCKET_BUFF_SIZE;

	INT8U _pkgBuf[BUFF_SIZE];
	int   _inStart;             /* 数据输入起始偏移  */
	int   _outStart;            /* 数据存储起始偏移  */
	int   _usedLen();           /* 已有数据长度      */
	void  _get_pkg_from_buffer(INT8U *start,int len);
};

#endif

