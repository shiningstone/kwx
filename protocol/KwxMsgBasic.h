
#ifndef __KWX_MSG_BASIC_H__
#define __KWX_MSG_BASIC_H__

#include "./../utils/UtilBasic.h"

#define MSG_MAX_LEN    1024

#define UP_STREAM      0
#define DOWN_STREAM    1

typedef std::string Key_t;
typedef INT32U RoomPath_t;
typedef INT32U RoomId_t;
typedef INT32U TableId_t;
typedef INT8U  SeatId_t;

/*this type could reference to other modules*/
typedef INT8U  CardType_t;

typedef enum {
    SUCCESS,
    FAIL,
}Status_t;

typedef enum {
    /*********************************************
        牌局动作REQUEST
    *********************************************/
    /* Upstream & Downstream : request and its response */
    REQ_GAME_SEND_START         = 43,
    REQ_GAME_RESTART            = 29,
    REQ_GAME_SEND_SHOWCARD      = 48,
    REQ_GAME_SEND_ACTION        = 49,
    REQ_GAME_GET_TINGINFO       = 50,
    
    /* Downstream : start by server */
    REQ_GAME_DIST_CARD          = 51,

    REQ_GAME_SEND_CALSCORE      = 53,
    REQ_GAME_DIST_DAOJISHI      = 54,
    REQ_GAME_DIST_REMIND        = 55,             
    REQ_GAME_DIST_BEGINCARDS    = 56,          
    REQ_GAME_DIST_BEGINCARDS_OTHER = 57,
    REQ_GAME_DIST_DECISION      = 59,
    REQ_GAME_DIST_HU_CALCULATE  = 60,
    
    /* Downstream : other's request notifications */
    REQ_GAME_DIST_CARD_TOOTHER  = 52,
    REQ_GAME_RECV_SHOWCARD      = 75,
    REQ_GAME_RECV_ACTION        = 76,
    REQ_GAME_RECV_START         = 77,
    
    REQ_GAME_SEND_AGENT         = 47,
    REQ_GAME_RECV_AGENT         = 78,

    /*********************************************
        牌局无关REQUEST
    *********************************************/
    HEART_BEAT                  = 40,
    
    REQ_GAME_SEND_ENTER         = 44,
    REQ_GAME_RECV_ENTER         = 71,

    /*  login */
    REQ_LOGIN                   = 1,
    REQ_LOGOUT                  = 15,
    REQ_RES_UPDATE              = 2,
    REQ_VER_UPDATE              = 3,
    RRQ_BASIC_INFO              = 31,
    REQ_DAILY_LOGIN             = 37,
    REQ_GET_DAILY_PRIZE         = 14,
    REQ_GAME_SEND_RECONNECT     = 42,
    REQ_GAME_RECV_RECONNECT     = 74,
    REQ_GAME_SEND_LEAVE_ROOM    = 45,
    REQ_GAME_RECV_LEAVE_ROOM    = 72,
    REQ_GAME_DIST_DUALLOGIN     = 57,    

    /* gold */
    REQ_GAME_SEND_XIA_PIAO      = 41,

    /* chat */
    REQ_GAME_SEND_MSG           = 46,
    REQ_GAME_RECV_MSG           = 73,
    
    REQ_INVALID                 = 0xff,
}RequestId_t;

#define MAX_CHARS      128
#define MAX_UTF8_BUF   MAX_CHARS*3

typedef INT8U Item_t;

typedef enum {
    REQUEST_ACCEPTED,
    DATAGRAM_ERROR     = 56,
    L2_ROOM_NOT_EXIST,
    L3_ROOM_NOT_EXIST,
    TABLE_NOT_EXIST,
    SEAT_ERROR,
    PIAO_ERROR,
    IN_GAME_ALREADY,
    ROOM_FULL,
    TIMEOUT,
    IN_GAME,
    USER_NOT_EXIST,
    LACK_OF_GOLD,
    USER_UNREGISTERED,
    USER_STATUS_DISMATCH,
    ROOM_UNINITIALIZED,
    RECONNECT_REQUIRED,
    SERVER_BOOTING,
    DATA_SYNC_ERROR,
    LOGIN_DUPLICATED,
    VERSION_TOO_OLD,
    NEW_VERSION_AVAILABLE,
    NEW_RES_AVAILABLE,
    SERVER_DATA_ERROR,
    FAILURE_UNDEFINED = 250,
}FailureCode_t;

#endif

