
#ifndef __CTESTCASE__
#define __CTESTCASE__

typedef enum {
    OK,
    FAIL
};

class CTestCase {
public:
	void Start();
	void Stop();
	virtual int Execute() = 0;
};

#endif