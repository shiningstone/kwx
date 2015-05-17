
#include "./../utils/BasicType.h"
#include "KwxMsgBasic.h"

#ifdef WIN32
    #ifndef __REQUEST_DESC__
    #define __REQUEST_DESC__

    typedef struct _RequestDesc {
        RequestId_t id;
        char        desc[32];
    }RequestDesc;

    typedef struct _ActionDesc {
        ActionId_t  id;
        char        desc[32];
    }ActionDesc;

    typedef struct _CardDesc {
        Card_t      id;
        char        desc[32];
    }CardDesc;

    typedef struct _PlayerDesc {
        PlayerDir_t id;
        char        desc[32];
    }PlayerDesc;
    
    #define DESC(x) { x,#x }

    extern char *DescReq(RequestId_t id);
    extern char *DescAct(ActionId_t act);
    extern char *DescCard(Card_t kind);
    extern char *DescPlayer(PlayerDir_t dir);

    #endif

#else

    char *DescReq(RequestId_t id) {return 0;}
    char *DescAct(ActionId_t act) {return 0;}
    char *DescCard(Card_t kind)    {return 0;}
    char *DescPlayer(PlayerDir_t dir) {return 0;}

#endif

