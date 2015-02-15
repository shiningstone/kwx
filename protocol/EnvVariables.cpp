
#include "EnvVariables.h"

EnvVariable::EnvVariable() {
#ifdef WIN32
	_protocol = 16;                
	_userId   = 0x01020304;        
	_language = 5;                 
	_platform = 6;                 
	_buildNo  = 7;                 
	_customerId = 0x0809;          
	_productId  = 0x0a0b;          
#else
	_protocol = 16;                //const value in terms of version
	_userId   = 0x01020304;        //set by device Id
	_language = 5;                 //set by user configuration
	_platform = 6;                 //set by device type
	_buildNo  = 7;                 //const value in terms of version
	_customerId = 0x0809;          //???
	_productId  = 0x0a0b;          //???
#endif
}

EnvVariable *EnvVariable::_instance = 0;

EnvVariable *EnvVariable::getInstance() {
	if(_instance==0) {
		_instance = new EnvVariable();
	}

	return _instance;
}
