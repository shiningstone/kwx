
#ifndef _SEAT_INFO_
#define _SEAT_INFO_

/*******************************************************************************
	NOTE:
        SeatInfo records the information in Server, used to convert local dir to server seatId.
        
        if a player is allowed to join more than one table in the meantime,
        getInstance should be called with the param "threadInfo"
********************************************************************************/
#include "./../game/GameType.h"
#include "./../LoginType.h"

#include "KwxMsgBasic.h"

class SeatInfo {
public:
    friend class UsMsg;

    static SeatInfo *getInstance(int threadInfo=0);
    int Set(const SeatInfo_t &info);
    int Set(RoomPath_t path,RoomId_t room,TableId_t table,SeatId_t seat);
    int Set(SeatId_t seat);

    PlayerDir_t GetPlayer(SeatId_t seat) const;
    SeatId_t    GetSeatId(PlayerDir_t dir) const;
private:
    friend class TestRecvEnterResponse;
    friend class TestRecvReconnectResponse;
    
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

