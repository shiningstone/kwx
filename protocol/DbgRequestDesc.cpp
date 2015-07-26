
#include "DbgRequestDesc.h"

static char GeneralInfoBuf[128] = {0};

static char *GeneralInfo(const char *type,int code) {
    memset(GeneralInfoBuf,0,128);
    sprintf(GeneralInfoBuf,"%s(%d)",type,code);
    return GeneralInfoBuf;
}

RequestDesc RequestDescTbl[] = {
    DESC(REQ_GAME_SEND_START),
    DESC(REQ_GAME_RESTART),
    DESC(REQ_GAME_SEND_SHOWCARD),
    DESC(REQ_GAME_SEND_ACTION),
    DESC(REQ_GAME_GET_TINGINFO),
    DESC(REQ_GAME_DIST_CARD),
    DESC(REQ_GAME_SEND_CALSCORE),
    DESC(REQ_GAME_DIST_DAOJISHI),
    DESC(REQ_GAME_DIST_REMIND),
    DESC(REQ_GAME_DIST_BEGINCARDS),
    DESC(REQ_GAME_DIST_BEGINCARDS_OTHER),
    DESC(REQ_GAME_DIST_DECISION),
    DESC(REQ_GAME_DIST_HU_CALCULATE),
    DESC(REQ_GAME_DIST_CARD_TOOTHER),
    DESC(REQ_GAME_RECV_SHOWCARD),
    DESC(REQ_GAME_RECV_ACTION),
    DESC(REQ_GAME_RECV_START),
    DESC(HEART_BEAT),
    DESC(REQ_GAME_SEND_ENTER),
    DESC(REQ_GAME_RECV_ENTER),
    DESC(REQ_LOGIN),
    DESC(REQ_DAILY_LOGIN),
    DESC(REQ_VER_UPDATE),
    DESC(REQ_GAME_SEND_RECONNECT),
    DESC(REQ_GAME_RECV_RECONNECT),
    DESC(REQ_GAME_SEND_LEAVE_ROOM),
    DESC(REQ_GAME_RECV_LEAVE_ROOM),
    DESC(REQ_GAME_DIST_DUALLOGIN),
    DESC(REQ_GAME_SEND_XIA_PIAO),
    DESC(REQ_GET_DAILY_PRIZE),
    DESC(REQ_GAME_SEND_LEAVE_ROOM),
    DESC(REQ_LOGOUT),
};

char *DescReq(RequestId_t id) {
    for(int i=0;i<sizeof(RequestDescTbl)/sizeof(RequestDescTbl[0]);i++) {
        if(id==RequestDescTbl[i].id) {
            return RequestDescTbl[i].desc;
        }
    }

    return GeneralInfo("request id",id);
}

ErrDesc ErrDescTbl[] = {
    DESC(DATAGRAM_ERROR),
    DESC(L2_ROOM_NOT_EXIST),
    DESC(L3_ROOM_NOT_EXIST),
    DESC(TABLE_NOT_EXIST),
    DESC(SEAT_ERROR),
    DESC(PIAO_ERROR),
    DESC(IN_GAME_ALREADY),
    DESC(ROOM_FULL),
    DESC(TIMEOUT),
    DESC(IN_GAME),
    DESC(USER_NOT_EXIST),
    DESC(LACK_OF_GOLD),
    DESC(USER_UNREGISTERED),
    DESC(USER_STATUS_DISMATCH),
    DESC(ROOM_UNINITIALIZED),
    DESC(RECONNECT_REQUIRED),
    DESC(SERVER_BOOTING),
    DESC(DATA_SYNC_ERROR),
    DESC(LOGIN_DUPLICATED),
    DESC(SERVER_DATA_ERROR),
};

char *DescErr(FailureCode_t id) {
    for(int i=0;i<sizeof(ErrDescTbl)/sizeof(ErrDescTbl[0]);i++) {
        if(id==ErrDescTbl[i].id) {
            return ErrDescTbl[i].desc;
        }
    }

    return GeneralInfo("failure code",id);
}

