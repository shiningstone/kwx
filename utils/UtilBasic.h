
#ifndef __UTIL_BASIC__
#define __UTIL_BASIC__

typedef unsigned char  INT8U;
typedef unsigned short INT16U;
typedef int            INT32U;

extern INT32U _ntohl(INT32U n);
extern INT32U _htonl(INT32U n);
extern INT16U _ntohs(INT16U n);
extern INT16U _htons(INT16U n);

typedef void (*MsgHandler_t )(const INT8U *msg,int &len);

#endif