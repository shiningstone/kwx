
#ifndef __KWX_REQUEST_CONSTS__
#define __KWX_REQUEST_CONSTS__

typedef enum {
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
    
    /* Downstream : other's request notifications */
    REQ_GAME_DIST_CARD_TOOTHER  = 52,
    REQ_GAME_RECV_SHOWCARD      = 75,
    REQ_GAME_RECV_ACTION        = 76,
    REQ_GAME_RECV_START         = 77,
}RequestId_t;

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
    PURE_ID,
    ID_WITH_INT,
    ID_WITH_BUF,
}Item_t;

#endif
