
#ifndef __BASIC_TYPE__
#define __BASIC_TYPE__

#define INVALID -1
#define PLAYER_NUM  3

typedef enum {
    LOCAL_GAME,
    NETWORK_GAME,
}GameMode_t;

typedef enum {
    LEFT,
    MIDDLE,
    RIGHT,
    INVALID_DIR,
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
    CARD_UNKNONW = CARD_MAX,
}Card_t;

typedef enum {
    BOY,
    GIRL,
    SEX_MAX,
}Sex_t;

typedef enum {
    SINGLE_WIN,
    DOUBLE_WIN,
    NONE_WIN,
}WinKind_t;

typedef struct {
    WinKind_t    kind;
    PlayerDir_t  player;
}WinInfo_t;

typedef enum {
    LAYDOWN_HIDE,
    LAYDOWN_SHOW,
    NORMAL_APPERANCE,
}CartApperance_t;

typedef struct {
    Card_t       kind;
    int          tingNum;
}TingInfoItem;

typedef struct {
    int          kindNum;
    TingInfoItem card[9];/* why 9??? */
}TingInfo_t;

typedef enum {
    AN_GANG = 1,
    MING_GANG,
    HU_WIN,
}GoldKind_t;

typedef int TagId_t;

#endif

