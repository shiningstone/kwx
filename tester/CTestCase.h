#include "./../network/CSockets.h"
#include "./../network/GameSocket.h"

class CTestCase {
public:
	static int count;

	CTestCase();

	virtual void Start();
	virtual void Stop();
	virtual int Execute() = 0;
};
