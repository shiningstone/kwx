
#ifndef _ENV_VARIABLES__
#define _ENV_VARIABLES__

#include "MsgBasic.h"

/*******************************************************************************
	NOTE:
		the value of environment variables should either 
		get from OTHER MODULES or set to a CONST VALUE, 
		while they are set to keep accordance with test cases during unit test
********************************************************************************/
class EnvVariable {
public:
	static EnvVariable *getInstance();

	INT8U    _protocol;
	INT32U   _userId;
    INT8U    _language;
    INT8U    _platform;
    INT8U    _buildNo;
    INT16U   _customerId;
    INT16U   _productId;
private:
	EnvVariable();
	static EnvVariable *_instance;
};

#endif