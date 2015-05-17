
#include "KwxMsgBasic.h"

#ifdef WIN32
    #ifndef __REQUEST_DESC__
    #define __REQUEST_DESC__

    typedef struct _RequestDesc {
        RequestId_t id;
        char        desc[32];
    }RequestDesc;

    #define DESC(x) { x,#x }

    extern char *Desc(RequestId_t id);

    #endif

#else

    char *Desc(RequestId_t id) {return 0;}

#endif

