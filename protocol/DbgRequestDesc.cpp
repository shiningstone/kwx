
#ifdef WIN32

#include "DbgRequestDesc.h"

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

ActionDesc ActionDescTbl[] = {
    DESC(aQi),
    DESC(aPENG),
    DESC(aMING_GANG),
    DESC(aAN_GANG),
    DESC(aMING),
    DESC(aHU),
    DESC(aSHOU_GANG),
    DESC(aKOU),
    DESC(aKOU_CANCEL),
};

CardDesc CardDescTbl[] = {
    DESC(TIAO_1),
    DESC(TIAO_2),
    DESC(TIAO_3),
    DESC(TIAO_4),
    DESC(TIAO_5),
    DESC(TIAO_6),
    DESC(TIAO_7),
    DESC(TIAO_8),
    DESC(TIAO_9),
    DESC(TONG_1),
    DESC(TONG_2),
    DESC(TONG_3),
    DESC(TONG_4),
    DESC(TONG_5),
    DESC(TONG_6),
    DESC(TONG_7),
    DESC(TONG_8),
    DESC(TONG_9),
    DESC(ZHONG),
    DESC(FA),
    DESC(BAI),
};

PlayerDesc PlayerDescTbl[] = {
    DESC(LEFT),
    DESC(MIDDLE),
    DESC(RIGHT),
};

char *DescReq(RequestId_t id) {
    for(int i=0;i<sizeof(RequestDescTbl)/sizeof(RequestDescTbl[0]);i++) {
        if(id==RequestDescTbl[i].id) {
            return RequestDescTbl[i].desc;
        }
    }

    return "Desc undefined";
}

char *DescAct(ActionId_t id) {
    for(int i=0;i<sizeof(ActionDescTbl)/sizeof(ActionDescTbl[0]);i++) {
        if(id==ActionDescTbl[i].id) {
            return ActionDescTbl[i].desc;
        }
    }

    return "Action undefined";
}

char *DescCard(Card_t id) {
    for(int i=0;i<sizeof(CardDescTbl)/sizeof(CardDescTbl[0]);i++) {
        if(id==CardDescTbl[i].id) {
            return CardDescTbl[i].desc;
        }
    }

    return "Card undefined";
}

char *DescPlayer(PlayerDir_t id) {
    for(int i=0;i<sizeof(PlayerDescTbl)/sizeof(PlayerDescTbl[0]);i++) {
        if(id==PlayerDescTbl[i].id) {
            return PlayerDescTbl[i].desc;
        }
    }

    return "player undefined";
}

#endif

