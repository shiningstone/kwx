
#include "KwxMsgEnv.h"

EnvVariable *EnvVariable::_instance = 0;

EnvVariable *EnvVariable::getInstance() {
	if(_instance==0) {
		_instance = new EnvVariable();
	}

	return _instance;
}

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

SeatInfo *SeatInfo::_instance[MAX_JOINABLE_TABLE] = {0};

SeatInfo *SeatInfo::getInstance(int threadId) {
    if(_instance[threadId]==0) {
        _instance[threadId] = new SeatInfo();
    }

    return _instance[threadId];
}

SeatInfo::SeatInfo() {
#ifdef WIN32
    _roomPath = 0x00010203;
    _roomId   = 0x04050607;
    _tableId  = 0x08090a0b;
    _seatId   = 1;
#else                          //these values should be initiated when a player join a table.
    _roomPath = 0x00010203;
    _roomId   = 0x04050607;
    _tableId  = 0x08090a0b;
    _seatId   = 1;
#endif
}

int SeatInfo::Set(RoomPath_t path,RoomId_t room,TableId_t table,SeatId_t seat) {
    _roomPath = path;
    _roomId   = room;
    _tableId  = table;
    _seatId   = seat;

    return 0;
}
