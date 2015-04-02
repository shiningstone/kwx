
#ifndef __CTESTCASE__
#define __CTESTCASE__

#include "./../CTestCase.h"

class CTestProtocol : public CTestCase {
public:
	virtual void Start();
	virtual void Stop();
	virtual int Execute() = 0;
};

#endif