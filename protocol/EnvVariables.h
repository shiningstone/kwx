
#ifndef _ENV_VARIABLES_
#define _ENV_VARIABLES_

#include "./../utils/UtilBasic.h"

#include "KwxRequestTypes.h"

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

/*******************************************************************************
	NOTE:
        if a player is allowed to join more than one table in the meantime,
        getInstance should be called with the param "threadInfo"
********************************************************************************/
class SeatInfo {
public:
    static SeatInfo *getInstance(int threadInfo=0);
    int Set(RoomPath_t path,RoomId_t room,TableId_t table,SeatId_t seat);

    RoomPath_t _roomPath;
    RoomId_t   _roomId;
    TableId_t  _tableId;
    SeatId_t   _seatId;
private:
    static const int MAX_JOINABLE_TABLE = 1;

    SeatInfo();
    static SeatInfo *_instance[MAX_JOINABLE_TABLE];
};

#endif
