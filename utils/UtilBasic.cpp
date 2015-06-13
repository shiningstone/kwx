
#include <string.h>
#include <stdio.h>

#include "UtilBasic.h"

/****************************************
    endian conversion
****************************************/
INT32U _ntohl(INT32U n) {
    char *p = (char *)&n;
    char ha[4] = {0};

    ha[0] = p[3];
    ha[1] = p[2];
    ha[2] = p[1];
    ha[3] = p[0];

    return *((INT32U *)ha);
}

INT32U _htonl(INT32U n) {
    return _ntohl(n);
}

INT16U _ntohs(INT16U n) {
    char *p = (char *)&n;
    char ha[2] = {0};

    ha[0] = p[1];
    ha[1] = p[0];

    return *((INT16U *)ha);
}

void _ntohs(const INT8U *buf,int len,INT8U *output) {
    memset(output,0,len+2);
    
    for(int i=0;i<len/2;i++) {
        output[2*i]     = buf[2*i + 1];
        output[2*i + 1] = buf[2*i];
    }
}

INT16U _htons(INT16U n) {
    return _ntohs(n);
}

/****************************************
    bit operation
****************************************/
void _set(INT32U &value,INT32U mask) {
    value |= mask;
}

void _clr(INT32U &value,INT32U mask) {
    value &= ~mask;
}

bool _is_active(INT32U value,INT32U mask) {
    return (value & mask);
}

/****************************************
string tranfer to bytes
    seperator should be ','
    byte's string could be 0,00,0x0,0x00
****************************************/
int ctoi(char c) {
    if( c>='0' && c <= '9' ) {
        return c-'0';
    } else if( c>='a' && c<='f' ) {
        return 10+c-'a';
    } else if( c>='A' &&c<='F' ) {
        return 10+c-'A';
    } else {
        return -1;
    }
}

int _getByteLen(const char *strByte) {
    const char * coma = strchr(strByte,',');
    if(coma!=NULL) {
        return strchr(strByte,',')-strByte;
    } else {
        return strlen(strByte)-1*(strByte[strlen(strByte)-1]==0xa);
    }
}

int _hexStringToHex(char *byte,const char *strByte) {
    const char *value = strByte+2;
    int valueLen      = _getByteLen(value);
    
    if(valueLen==1) {
        *byte = ctoi(*value);
    } else {
        *byte = ctoi(*value)*16 + ctoi(*(value+1));
    }

    return strlen("0x")+valueLen+strlen(",");
}

int _decStringToHex(char *byte,const char *strByte) {
    const char *value = strByte;
    int valueLen      = _getByteLen(value);
    
    if(valueLen==1) {
        *byte = ctoi(*value);
    } else if (valueLen==2){
        *byte = ctoi(*value)*10 + ctoi(*(value+1));
    } else {
        *byte = ctoi(*value)*100 + ctoi(*(value+1))*10 + ctoi(*(value+2));
    }

    return valueLen+strlen(",");
}

int _byte(char *byte,const char *strByte) {
    if(!strncmp(strByte,"0x",2)) {
        return _hexStringToHex(byte,strByte);
    } else {
        return _decStringToHex(byte,strByte);
    }
}

int _bytes(char *buf,const char *str) {
    memset(buf,0,512);
    
    int usedLen = 0;
    int i=0;

    while(usedLen<strlen(str)) {
        usedLen += _byte(buf+i,str+usedLen);
        i++;
    }

    return i;
}

/****************************************************************
    code
****************************************************************/
typedef struct _Range_t {
    int start;
    int end;
}Range_t;

const Range_t UTF16_MAPPER[3] = {
    {0x0001,0x007F},
    {0x0080,0x07FF},
    {0x0800,0xFFFF},
};

const Range_t UTF8_MAPPER[3] = {
    {0x00,0x7F},   /* 0bbbbbbb                      */
    {0xC0,0XDF},   /* 110bbbbb    10bbbbbb          */
    {0xE0,0xEF},   /* 1110bbbb    10bbbbbb 10bbbbbb */
};

int Utf16ToUtf8(const Utf16* pUtf16Start, int len, Utf8* pUtf8Start) {
    Utf16 buf[128] = {0};
    memcpy(buf,pUtf16Start+1,(len-1)*sizeof(Utf16));    /*Extract 0xfeff*/

    const Utf16* pUtf16End  = pUtf16Start + len;

    const Utf16* pTempUtf16 = pUtf16Start;
    Utf8*  pTempUtf8  = pUtf8Start;
 
    while(pTempUtf16<pUtf16End)
    {
        unsigned wchar_t byte = *pTempUtf16;
        
        if (byte <= UTF16_MAPPER[0].end)
        {//0000 - 007F  0xxxxxxx
            *pTempUtf8++ = (Utf8)byte;
        }
        else if(byte >= UTF16_MAPPER[1].start && byte <= UTF16_MAPPER[1].end) 
        {//0080 - 07FF 110xxxxx 10xxxxxx
            *pTempUtf8++ = (byte >> 6) | 0xC0;
            *pTempUtf8++ = (byte & 0x3F) | 0x80;
        }
        else if(byte >= UTF16_MAPPER[2].start && byte <= UTF16_MAPPER[2].end)
        {//0800 - FFFF 1110xxxx 10xxxxxx 10xxxxxx
            *pTempUtf8++ = (byte >> 12) | 0xE0;
            *pTempUtf8++ = ((byte >> 6) & 0x3F) | 0x80;
            *pTempUtf8++ = (byte & 0x3F) | 0x80;
        }
        else
        {
            break;
        }
        pTempUtf16++;
    }
    
    *pTempUtf8 = 0;

    return 2*(pTempUtf16-pUtf16Start);
}

int Utf8ToUtf16(const Utf8* pUtf8Start, Utf16* pUtf16Start)
{
    const Utf8*  pUtf8End = pUtf8Start + strlen(pUtf8Start);

    pUtf16Start[0] = UTF16_HEADER;
    
    Utf16* pTempUtf16 = pUtf16Start+1;
    const Utf8*  pTempUtf8 = pUtf8Start;
 
    while (pTempUtf8 < pUtf8End)
    {
        unsigned char byte = *pTempUtf8;
        
        if (byte >= UTF8_MAPPER[2].start && byte <= UTF8_MAPPER[2].end)
        {//0800 - FFFF 1110xxxx 10xxxxxx 10xxxxxx
            *pTempUtf16 |= ((*pTempUtf8++ & 0xEF) << 12);
            *pTempUtf16 |= ((*pTempUtf8++ & 0x3F) << 6);
            *pTempUtf16 |= (*pTempUtf8++ & 0x3F);
        }
        else if (byte >= UTF8_MAPPER[1].start && byte <= UTF8_MAPPER[1].end)
        {//0080 - 07FF 110xxxxx 10xxxxxx
            *pTempUtf16 |= ((*pTempUtf8++ & 0x1F) << 6);
            *pTempUtf16 |= (*pTempUtf8++ & 0x3F);
        }
        else if(byte >= UTF8_MAPPER[0].start && byte <= UTF8_MAPPER[0].end)
        {//0000 - 007F  0xxxxxxx
            *pTempUtf16 = *pTempUtf8++;
        }
        else
        {
            break;
        }
        pTempUtf16++;
    }
    
    *pTempUtf16 = 0;

    return (pTempUtf16-pUtf16Start)*2;
}

#define MIDDLE 1
void _split(INT8U strings[3][128],const INT8U *buf) {
    const char *SPLIT = "%@";
    int idx = 0;

    char *token = strtok((char *)buf,SPLIT);
    
    while(token!=NULL) {
        strcpy((char *)strings[(MIDDLE+idx)%3],token);
        idx++;
        
        token = strtok(NULL,SPLIT);
    }
}

/****************************************************************
    platform dependent
****************************************************************/
#ifdef WIN32
#include <windows.h>
#else
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

USING_NS_CC;
#endif

void _delay(int ms) {
    #ifdef WIN32
    Sleep(ms);
    #else
    /*android interface*/
    #endif
}
