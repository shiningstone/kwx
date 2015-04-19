
#ifndef __KWX_MSG_BASIC_H__
#define __KWX_MSG_BASIC_H__

#include "./../utils/UtilBasic.h"

#define MSG_MAX_LEN    1024

#define UP_STREAM      0
#define DOWN_STREAM    1

typedef INT32U Key_t;
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
    REQ_GAME_SEND_SHOWCARD      = 48,
    REQ_GAME_SEND_ACTION        = 49,
    REQ_GAME_GET_TINGINFO       = 50,
    
    /* Downstream : start by server */
    REQ_GAME_DIST_CARD          = 51,

    REQ_GAME_SEND_CALSCORE      = 53,                
    REQ_GAME_DIST_REMIND        = 55,             
    REQ_GAME_DIST_BEGINCARDS    = 56,          
    REQ_GAME_DIST_BEGINCARDS_OTHER = 57,
    REQ_GAME_DIST_DECISION      = 59,
    REQ_GAME_DIST_HU_CALCULATE  = 60,
    REQ_GAME_DIST_HU_CALCULATE2 = 61,
    
    /* Downstream : other's request notifications */
    REQ_GAME_DIST_CARD_TOOTHER  = 52,
    REQ_GAME_RECV_SHOWCARD      = 75,
    REQ_GAME_RECV_ACTION        = 76,
    REQ_GAME_RECV_START         = 77,
    
    /*********************************************
        牌局无关REQUEST
    *********************************************/
    HEART_BEAT                  = 40,
    
    REQ_GAME_SEND_ENTER         = 44,
    REQ_GAME_RECV_ENTER         = 71,
    
}RequestId_t;

#endif

