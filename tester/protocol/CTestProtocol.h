
#ifndef __CTESTCASE__
#define __CTESTCASE__

#include "./../CTestCase.h"

typedef enum {
    OK,
    FAIL
};

class CTestProtocol : public CTestCase {
public:
	virtual void Start();
	virtual void Stop();
	virtual int Execute() = 0;
};

#endif