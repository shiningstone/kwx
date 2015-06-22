
#include "SeatInfo.h"

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


int SeatInfo::Set(const SeatInfo_t &info) {
    _roomPath = info.roomPath;
    _roomId   = info.roomId;
    _tableId  = info.tableId;
    Set(info.seatId);

    return 0;
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


