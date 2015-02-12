#include "./../CSockets.h"
#include "./../GameSocket.h"

class CTestCase {
public:
	CTestCase();

	void Start();
	void Stop();
	int Execute();

	char _buff[SOCK_BUFF_LEN];
	int  _buffLen;

protected:
	ServerSocket SERVER;
	ClientSocket CLIENT;
	CGameSocket  GAME_CLIENT;

	virtual void ServerActions() = 0;
	virtual void ClientActions() = 0;
};
