
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
    DOUBLE_PLAYER = 3,
}PlayerDir_t;

typedef enum {
    aQi        = 0,
    aPENG      = 0x1,
    aMING_GANG = 0x2,
    aAN_GANG   = 0x4,
    aMING      = 0x8,
    aHU        = 0x10,
    aSHOU_GANG = 0x20,
    aKOU       = 0x40,
    aKOU_CANCEL= 0x80,
}ActionId_t;/*ARRAY_ACTION???*/

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
    CARD_KIND_MAX,
    CARD_UNKNOWN = 0xff,
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
    PlayerDir_t  winner;
    PlayerDir_t  giver;
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

