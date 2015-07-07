
#ifndef __UTIL_BASIC__
#define __UTIL_BASIC__

#include <string>
#include <vector>
using namespace std;

#ifndef NULL
#define NULL 0
#endif

#define RET_OK     0
#define RET_ERROR  -1

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

extern void _ntohs(const INT8U *buf,int len,INT8U *output);
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
typedef INT8U   Utf8;
typedef INT16U  Utf16;

static Utf16 UTF16_HEADER = 0xfeff;
#define UTF16_HEADER_LEN 2

/* the original Utf16 string always include header 0xfeff */
extern int Utf16ToUtf8(const Utf16* pUtf16Start, int len, Utf8* pUtf8Start);
/* the generated Utf16 string will include header 0xfeff */
extern int Utf8ToUtf16(const Utf8* pUtf8Start, Utf16* pUtf16Start);

void _split(INT8U strings[3][128],const INT8U *buf);
void _split(vector<string> &strings,const INT8U *buf,const char *spliter);
/****************************************
string tranfer to bytes
    seperator should be ','
    byte's string could be 0,00,0x0,0x00
****************************************/
extern int _bytes(char *buf,const char *str);

typedef int (*MsgHandler_t )(const INT8U *msg,int &len);

extern void _delay(int ms);

typedef struct {
    int  id;
    char name[32];
    char ipaddr[32];
    int  port;
    int  voicePort;
}Server_t;

typedef struct _DeviceInfo_t {
    INT8U mac[128];    
    INT8U imsi[128];    
    INT8U resolution[128];    
    INT8U protoType[128];    
    INT8U osVer[128];
}DeviceInfo_t;

typedef INT32U VersionCode_t;

typedef struct _VerInfo_t {
    VersionCode_t   code;
    string          name;
    string          content;
    string          size;/*Mbytes*/
    string          url;
}VerInfo_t;

#endif

