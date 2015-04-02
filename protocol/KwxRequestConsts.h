
#ifndef __KWX_REQUEST_CONSTS__
#define __KWX_REQUEST_CONSTS__

typedef enum {
    /* KWX-外部通信协议4.0 */
    REQ_GAME_SEND_START = 43,
    REQ_GAME_SEND_ACTION = 49,
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

typedef INT8U Item_t;

#endif
