#include "./../CSockets.h"

class CTestCase {
public:
	void Start();
	void Stop();
	int Execute();

protected:
	ServerSocket SERVER;
	ClientSocket CLIENT;

	virtual void ServerActions() = 0;
	virtual void ClientActions() = 0;
};
