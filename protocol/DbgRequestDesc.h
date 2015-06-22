
#include "./../game/GameType.h"
#include "KwxMsgBasic.h"

#ifndef __REQUEST_DESC__
#define __REQUEST_DESC__

#ifdef WIN32

    typedef struct _RequestDesc {
        RequestId_t id;
        char        desc[32];
    }RequestDesc;

    typedef struct _ErrDesc {
        FailureCode_t id;
        char          desc[32];
    }ErrDesc;

    extern char *DescReq(RequestId_t id);
    extern char *DescErr(FailureCode_t id);

#else

    char *DescReq(RequestId_t id);
    char *DescErr(FailureCode_t id);

#endif

#endif

