
#include <stdio.h>
#include <string.h>

#include "KwxEnv.h"

#include "./../utils/DebugCtrl.h"

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
#ifdef DBG_REMOTE_SERVER
    #if (DBG_REMOTE_SERVER==1)
        strcpy(server.ipaddr,LOCAL_SERVER);
    #elif (DBG_REMOTE_SERVER==2)
        strcpy(server.ipaddr,LAN_SERVER);
    #else
        FILE * target = NULL;
        target = fopen("E:\\server_ip.txt","r");
        if(target!=NULL) {
            fgets(server.ipaddr,128,target);
        } else {
            strcpy(server.ipaddr,LOCAL_SERVER);
        }
    #endif
#else
    sprintf(server.ipaddr,"%s",ip);
#endif
}

void EnvVariable::LoginServerInit() {
    _loginServer.id = 0;
    sprintf(_loginServer.name,"loginServer");
    SetServerIp(_loginServer,LOGIN_SERVER_IP);
    _loginServer.port = DEF_SOCKET_PORT;
}

void EnvVariable::RoomServerInit(int roomId) {
    SetRoomServer(roomId,ROOM_SERVER_IP,DEF_SOCKET_PORT,0);
}

void EnvVariable::SetRoomServer(int roomId,const char *ip,int port,int voicePort) {
    _roomServer.id = roomId;
    sprintf(_roomServer.name,"roomServer%d",roomId);
    SetServerIp(_roomServer,ip);
    _roomServer.port = port;
    _roomServer.voicePort = voicePort;
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
    _reconnectRequired = false;
    _key      = 0x01020304;
    _roomPath = 11;

    LoginServerInit();
    RoomServerInit(0);
    
    _get_device_info(_device);
}

void EnvVariable::SetReconnect(bool flag) {
    _reconnectRequired = flag;
}

bool EnvVariable::IsReconnectRequired() {
    return _reconnectRequired;
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

void EnvVariable::SetRoomPath(RoomPath_t id) {
    _roomPath = id;
}

RoomPath_t EnvVariable::GetRoomPath() {
    return _roomPath;
}

