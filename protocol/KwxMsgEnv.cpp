
#include <stdio.h>
#include <string.h>

#include "KwxMsgEnv.h"

#define LOGIN_SERVER_IP  "127.0.0.1"
#define ROOM_SERVER_IP   "127.0.0.1"
#define LOCAL_SERVER     "127.0.0.1"
#define DEF_SOCKET_PORT  60905


EnvVariable *EnvVariable::_instance = 0;

EnvVariable *EnvVariable::getInstance() {
	if(_instance==0) {
		_instance = new EnvVariable();
	}

	return _instance;
}

void EnvVariable::SetServerIp(Server_t &server,const char *ip) {
#ifdef WIN32
    FILE * target = NULL;
        
    #ifdef USE_REMOTE_SERVER
    target = fopen("E:\\server_ip.txt","r");
    #endif
        
    if(target!=NULL) {
        fgets(server.ipaddr,128,target);
    } else {
        strcpy(server.ipaddr,LOCAL_SERVER);
    }
#else
    sprintf(server.ipaddr,ip);
#endif
}

void EnvVariable::LoginServerInit() {
    _loginServer.id = 0;
    sprintf(_loginServer.name,"loginServer");
    SetServerIp(_loginServer,LOGIN_SERVER_IP);
    _loginServer.port = DEF_SOCKET_PORT;
}

void EnvVariable::RoomServerInit(int roomId) {
    _roomServer.id = roomId;
    sprintf(_roomServer.name,"roomServer%d",roomId);
    SetServerIp(_roomServer,ROOM_SERVER_IP);
    _roomServer.port = DEF_SOCKET_PORT;
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

    _key = 0x01020304;
#else
	_protocol = 16;                //const value in terms of version
	_userId   = 0x01020304;        //set by device Id
	_language = 5;                 //set by user configuration
	_platform = 6;                 //set by device type
	_buildNo  = 7;                 //const value in terms of version
	_customerId = 0x0809;          //???
	_productId  = 0x0a0b;          //???

    _key = 0x01020304;
#endif
    LoginServerInit();
    RoomServerInit(0);
    
    _get_device_info(_device);
}


void EnvVariable::SetKey(Key_t key) {
    _key = key;
}

Key_t EnvVariable::GetKey() {
    return _key;
}

void EnvVariable::SetUserId(INT32U id) {
    _userId = id;
}

INT32U EnvVariable::GetUserId() {
    return _userId;
}

RoomPath_t EnvVariable::GetRoomPath(int id) {
    return 11;
}

/*******************************************************************************
********************************************************************************/
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
        return SERVER;
    } else {
        return _seatToDir[seat-1];
    }
}

SeatId_t SeatInfo::GetSeatId(PlayerDir_t dir)const {
    if(dir==SERVER) {
        return 0;
    } else {
        return _dirToSeat[dir];
    }
}

