#include "./../CTestCase.h"

#include "./../../network/CSockets.h"
#include "./../../network/GameSocket.h"

class CTestNetwork : public CTestCase {
public:
	CTestNetwork();

	virtual void Start();
	virtual void Stop();
	virtual int Execute();

	char _clientBuff[SOCK_BUFF_LEN];
	int  _clientBuffLen;
	char _serverBuff[SOCK_BUFF_LEN];
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
