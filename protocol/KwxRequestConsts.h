
#ifndef __KWX_REQUEST_CONSTS__
#define __KWX_REQUEST_CONSTS__

typedef enum {
    /*     登录、充值      */
    REQ_LOGIN_OTHER     = 1,
    REQ_LOGIN_RESOURCES = 2,
    REQ_VERSION         = 3,
    REQ_BASIC_INFOR     = 4,

    /*     个人资料      */
    REQ_PERSON_DATA_ALTER     = 30,
    REQ_PERSON_DATA_HEAD_NAME = 31,
    REQ_PERSON_DATA           = 32,

    /*     游戏模块      */
    REQ_HEART_BEAT      = 40,
    REQ_GAME_PIAO       = 41,
    REQ_GAME_QUICK_SERVER = 43,
    REQ_GAME_ENTER_ROOM = 44,
    REQ_GAME_SEND_SEAT_ACTION = 45,
    REQ_GAME_SEND_MSG = 46,
    REQ_GAME_SEND_ACTION = 47,
    REQ_GAME_SEND_SHOW_CARD = 48,      /*  THIS HAS NOT BEEN UNDETERMINED YET*/

    REQ_GAME_RECV_ACTION = 49,
    REQ_GAME_SEND_DIST = 50,           /*REQ_GAME_RECV_LEAVE = 50, THIS HAS NOT BEEN UNDETERMINED YET  */
    REQ_GAME_RECV_SEAT_DOWN = 51,
    REQ_GAME_SEND_UPDATELIST = 52,     /*REQ_GAME_RECV_CARDS = 52, THIS HAS NOT BEEN UNDETERMINED YET  */
    REQ_GAME_RECV_WINNER = 53,
    REQ_GAME_TIMEOUT_BACK = 54,
    REQ_GAME_RECV_READYTIME = 55,
    REQ_GAME_RECV_START_INFOR = 56,
    REQ_GAME__ADD_CHIPS = 57,
    REQ_GAME_RECV_MSG = 58,
    REQ_GAME_CONNECT_AGAIN = 59,
    REQ_GAME_SEARCH_ROOM = 60,
    REQ_GAME_RECV_OUT = 61,
    REQ_GAME_PLAYER_DATA = 62,
    REQ_GAME_USE_GIFT = 63,
    REQ_GET_COUNTDOWN_HONUS = 64,
}RequestId_t;

typedef INT32U RoomPath_t;
typedef INT32U RoomId_t;
typedef INT32U TableId_t;
typedef INT8U  SeatId_t;

typedef INT8U  CardType_t;/*this type could reference to other modules*/

typedef enum {
    PENG = 1,
    GANG,
    GUO,
    HU,
}ActionId_t;/*ARRAY_ACTION???*/

typedef enum {
	SeatId   = 60,
    CardKind = 65,
	ActionId = 67,
	RoomPath = 131,
	RoomId   = 132,
	TableId  = 133,
    CardList = 135,
}Item_t;

#endif