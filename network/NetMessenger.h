
#ifndef __NET_MESSENGER__
#define __NET_MESSENGER__

/*******************************************************
    NetMessenger

        Recv�ӿڣ�  
            ����ֱ�ӽ���message��������ʹ�ã�
        SetHandler�ӿڣ�   
            ��������һ�����̣��յ�messageʱ�����ص�������
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
    /*** �����Զ�����   ***/
    bool _is_kwx_exist();
    void _collect_packages();

    /*** ���ĸ�ʽʶ��   ***/
    int   _get_available_pkg_len();
    INT16U _get_request_id(const INT8U *pkg);

    /*** ���ν��ջ����� ***/
	void _collect_bytes();

    const static int BUFF_SIZE = SOCKET_BUFF_SIZE;

	INT8U _pkgBuf[BUFF_SIZE];
	int   _inStart;             /* ����������ʼƫ��  */
	int   _outStart;            /* ���ݴ洢��ʼƫ��  */
	int   _usedLen();           /* �������ݳ���      */
	void  _get_pkg_from_buffer(INT8U *start,int len);
};

#endif

