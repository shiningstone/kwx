
#include "GameType.h"

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
    DESC(aMING_CANCEL),
    DESC(aMING_CONFIRM),
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

