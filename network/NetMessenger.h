
#ifndef __NET_MESSENGER__
#define __NET_MESSENGER__

#include "CSockets.h"
#include "./../protocol/MsgFormats.h"

class NetMessenger {
public:
	static NetMessenger *getInstance();

	void Start();
	int  Recv(INT8U *buf,int *len);
	int  Send(const INT8U *buf,int len);
protected:
	NetMessenger();
	~NetMessenger();

	static NetMessenger *_instance;
	static CSocket      *_socket;
};

#endif
