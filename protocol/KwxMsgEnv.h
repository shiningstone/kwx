
#ifndef __KWX_ENV_VARIABLES_H__
#define __KWX_ENV_VARIABLES_H__

#include "./../utils/UtilBasic.h"
#include "./../utils/UtilPlatform.h"

#include "KwxMsgBasic.h"
/*******************************************************************************
	NOTE:
		the value of environment variables should either 
		get from OTHER MODULES or set to a CONST VALUE, 
		while they are set to keep accordance with test cases during unit test
********************************************************************************/
class EnvVariable {
public:
	static EnvVariable *getInstance();

    void       SetRoomServer(int roomId,const char *ip,int port);
    void       SetKey(Key_t key);
    Key_t      GetKey();
    void       SetUserId(INT32U id);
    INT32U     GetUserId();
    void       SetRoomPath(RoomPath_t id);
    RoomPath_t GetRoomPath();

    Key_t      _key;
    RoomPath_t _roomPath;
    
    INT8U    _protocol;
	INT32U   _userId;
    INT8U    _language;
    INT8U    _platform;
    INT8U    _buildNo;
    INT16U   _customerId;
    INT16U   _productId;

    DeviceInfo_t _device;
    
    Server_t     _loginServer;
    Server_t     _roomServer;
    
private:
	EnvVariable();
	static EnvVariable *_instance;

    static void SetServerIp(Server_t &server,const char *ip);
    void LoginServerInit();
    void RoomServerInit(int roomId);
};

/*******************************************************************************
	NOTE:
        SeatInfo records the information in Server, used to convert local dir to server seatId.
        
        if a player is allowed to join more than one table in the meantime,
        getInstance should be called with the param "threadInfo"
********************************************************************************/
#include "./../game/GameType.h"
class SeatInfo {
public:
    friend class UsMsg;

    static SeatInfo *getInstance(int threadInfo=0);
    int Set(RoomPath_t path,RoomId_t room,TableId_t table,SeatId_t seat);
    int Set(SeatId_t seat);

    PlayerDir_t GetPlayer(SeatId_t seat) const;
    SeatId_t    GetSeatId(PlayerDir_t dir) const;
private:
    friend class TestRecvEnterResponse;
    
    RoomPath_t _roomPath;
    RoomId_t   _roomId;
    TableId_t  _tableId;
    SeatId_t   _seatId;

    void _SetSeatDirMap();
    PlayerDir_t _seatToDir[PLAYER_NUM];
    SeatId_t    _dirToSeat[PLAYER_NUM];

    static const int MAX_JOINABLE_TABLE = 1;

    SeatInfo();
    static SeatInfo *_instance[MAX_JOINABLE_TABLE];
};

#endif
