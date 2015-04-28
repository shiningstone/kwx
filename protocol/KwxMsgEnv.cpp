
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


Key_t EnvVariable::GetKey() {
    return 0x01020304;
}

RoomPath_t EnvVariable::GetRoomPath(int id) {
    return 1;
}

INT32U EnvVariable::GetUserId() {
    return _userId;
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
    _roomPath = 1;
    _roomId   = 0x1001000;
    _tableId  = 1;
    _seatId   = 1;
#else                          //these values should be initiated when a player join a table.
    _roomPath = 1;
    _roomId   = 0x1001000;
    _tableId  = 1;
    _seatId   = 1;
#endif
}


int SeatInfo::Set(RoomPath_t path,RoomId_t room,TableId_t table,SeatId_t seat) {
    _roomPath = path;
    _roomId   = room;
    _tableId  = table;

    Set(seat);

    return 0;
}

int SeatInfo::Set(SeatId_t seat) {
    _seatId   = seat;
    _SetSeatDirMap();
    return 0;
}

void SeatInfo::_SetSeatDirMap() {
    const int SeatToDir[3][3] = {
        /*1     2       3*/
        {MIDDLE,RIGHT,  LEFT},  /*_seatId==1*/
        {LEFT,  MIDDLE, RIGHT}, /*_seatId==2*/
        {RIGHT, LEFT,   MIDDLE},/*_seatId==3*/
    };
    
    const int DirToSeat[3][3] = {
        /*LEFT MIDDLE RIGHT*/
        {3,     1,      2},/*_seatId==1*/
        {1,     2,      3},/*_seatId==2*/
        {2,     3,      1},/*_seatId==3*/
    };
    
    for(int i=0;i<PLAYER_NUM;i++) {
        _seatToDir[i] = (PlayerDir_t) SeatToDir[_seatId-1][i];
        _dirToSeat[i] = DirToSeat[_seatId-1][i];
    }
}

PlayerDir_t SeatInfo::GetPlayer(SeatId_t seat)const {
    if(seat==0) {
        return INVALID_DIR;
    } else {
        return _seatToDir[seat-1];
    }
}

SeatId_t SeatInfo::GetSeatId(PlayerDir_t dir)const {
    return _dirToSeat[dir];
}

