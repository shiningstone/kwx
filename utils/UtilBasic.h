
#ifndef __UTIL_BASIC__
#define __UTIL_BASIC__

#ifndef NULL
#define NULL 0
#endif

typedef unsigned char  INT8U;
typedef unsigned short INT16U;
typedef int            INT32U;

extern INT32U _ntohl(INT32U n);
extern INT32U _htonl(INT32U n);
extern INT16U _ntohs(INT16U n);
extern INT16U _htons(INT16U n);

/****************************************
string tranfer to bytes
    seperator should be ','
    byte's string could be 0,00,0x0,0x00
****************************************/
extern int _bytes(char *buf,const char *str);

typedef int (*MsgHandler_t )(const INT8U *msg,int &len);

extern void _delay(int seconds);

#endif