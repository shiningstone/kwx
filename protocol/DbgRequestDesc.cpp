
#include "DbgRequestDesc.h"

#ifdef WIN32

RequestDesc RequestDescTbl[] = {
    DESC(REQ_GAME_SEND_START),
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
};

char *DescReq(RequestId_t id) {
    for(int i=0;i<sizeof(RequestDescTbl)/sizeof(RequestDescTbl[0]);i++) {
        if(id==RequestDescTbl[i].id) {
            return RequestDescTbl[i].desc;
        }
    }

    return "Desc undefined";
}

#else 

char *DescReq(RequestId_t id) { return 0; }

#endif

