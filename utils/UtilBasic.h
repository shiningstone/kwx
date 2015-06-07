
#ifndef __UTIL_BASIC__
#define __UTIL_BASIC__

#ifndef NULL
#define NULL 0
#endif

typedef unsigned char  INT8U;
typedef unsigned short INT16U;
typedef unsigned int   INT32U;
typedef int            INT32S;

/************************************
    byte order (host/network)
************************************/
extern INT32U _ntohl(INT32U n);
extern INT32U _htonl(INT32U n);
extern INT16U _ntohs(INT16U n);
extern INT16U _htons(INT16U n);

/************************************
    bit operation
************************************/
extern void _set(INT32U &value,INT32U mask);
extern void _clr(INT32U &value,INT32U mask);
extern bool _is_active(INT32U value,INT32U mask);

/************************************
    coding

    UCS-2编码    UTF-8 字节流(二进制) 
    0000 - 007F  0xxxxxxx
    0080 - 07FF  110xxxxx 10xxxxxx
    0800 - FFFF  1110xxxx 10xxxxxx 10xxxxxx
************************************/
typedef char  Utf8;
typedef wchar_t Utf16;

extern void Utf16ToUtf8(Utf16* pUtf16Start, Utf16* pUtf16End, Utf8* pUtf8Start, Utf8* pUtf8End);
extern void Utf8ToUtf16(Utf8* pUtf8Start, Utf8* pUtf8End, Utf16* pUtf16Start, Utf16* pUtf16End);

/****************************************
string tranfer to bytes
    seperator should be ','
    byte's string could be 0,00,0x0,0x00
****************************************/
extern int _bytes(char *buf,const char *str);

typedef int (*MsgHandler_t )(const INT8U *msg,int &len);

extern void _delay(int ms);

void _convert_from_utf16(INT8U *buf,const INT8U *utf16);
void _convert_to_utf16(INT8U *buf,const INT8U *utf16);

#endif
