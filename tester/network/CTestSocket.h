
#ifndef __TEST_SOCKET__
#define __TEST_SOCKET__

#include "./../CTestCase.h"

#include "./../../network/CSockets.h"
#include "./../../network/GameSocket.h"

class CTestSocket : public CTestCase {
public:
	virtual void Start();
	virtual void Stop();
	virtual int  Execute();

	char _clientBuff[SOCKET_BUFF_SIZE];
	int  _clientBuffLen;
	char _serverBuff[SOCKET_BUFF_SIZE];
	int  _serverBuffLen;

	void ClearClientBuf();
	void ClearServerBuf();
protected:
	ServerSocket SERVER;
	ClientSocket CLIENT;
	CGameSocket  GAME_CLIENT;

	virtual void ServerActions() = 0;
	virtual void ClientActions() = 0;
};

#include "./../../network/NetMessenger.h"
class CTestMessenger : public CTestSocket {
public:
    virtual void Start();
	virtual void Stop();

protected:
	NetMessenger    *MESSENGER;

	virtual void ServerActions() = 0;
	virtual void ClientActions() = 0;
};

#endif