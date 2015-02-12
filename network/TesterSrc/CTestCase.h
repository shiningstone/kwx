#include "./../CSockets.h"
#include "./../GameSocket.h"

class CTestCase {
public:
	CTestCase();

	void Start();
	void Stop();
	int Execute();

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
