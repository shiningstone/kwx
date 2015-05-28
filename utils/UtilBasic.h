
#ifndef __UTIL_BASIC__
#define __UTIL_BASIC__

#ifndef NULL
#define NULL 0
#endif

typedef unsigned char  INT8U;
typedef unsigned short INT16U;
typedef unsigned int   INT32U;
typedef int            INT32S;

extern INT32U _ntohl(INT32U n);
extern INT32U _htonl(INT32U n);
extern INT16U _ntohs(INT16U n);
extern INT16U _htons(INT16U n);

extern void _set(INT32U &value,INT32U mask);
extern void _clr(INT32U &value,INT32U mask);
extern bool _is_active(INT32U value,INT32U mask);
/****************************************
string tranfer to bytes
    seperator should be ','
    byte's string could be 0,00,0x0,0x00
****************************************/
extern int _bytes(char *buf,const char *str);

typedef int (*MsgHandler_t )(const INT8U *msg,int &len);

extern void _delay(int ms);

#endif
