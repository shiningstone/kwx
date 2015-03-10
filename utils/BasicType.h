
#ifndef __BASIC_TYPE__
#define __BASIC_TYPE__

typedef enum {
    LOCAL_GAME,
    NETWORK_GAME,
}GameMode_t;

typedef enum {
    LEFT,
    MIDDLE,
    RIGHT,
}PlayerDir_t;

typedef enum {
    PENG,
    GANG,
    TING,
    HU,
    ACTION_MAX,
}Action_t;

typedef enum {
    TIAO_1,
    TIAO_2,
    TIAO_3,
    TIAO_4,
    TIAO_5,
    TIAO_6,
    TIAO_7,
    TIAO_8,
    TIAO_9,
    TONG_1,
    TONG_2,
    TONG_3,
    TONG_4,
    TONG_5,
    TONG_6,
    TONG_7,
    TONG_8,
    TONG_9,
    ZHONG,
    FA,
    BAI,        
    CARD_MAX,
    HIDE = CARD_MAX,
}Card_t;

typedef enum {
    BOY,
    GIRL,
    SEX_MAX,
}Sex_t;

#endif

