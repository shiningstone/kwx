
#include <string.h>
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
void _convert_from_utf16(INT8U *buf,const INT8U *utf16) {
    strcpy((char *)buf,(char *)utf16);
}

void _convert_to_utf16(INT8U *utf16,const INT8U *buf) {
    strcpy((char *)utf16,(char *)buf);
}

#define UTF8_1_START      (0xOOO1)
#define UTF8_1_END        (0x007F)
#define UTF8_2_START      (0x0080)
#define UTF8_2_END        (0x07FF)
#define UTF8_3_START      (0x0800)
#define UTF8_3_END        (0xFFFF)

void Utf16ToUtf8(Utf16* pUtf16Start, Utf16* pUtf16End, Utf8* pUtf8Start, Utf8* pUtf8End)
{
    Utf16* pTempUtf16 = pUtf16Start;
    Utf8*  pTempUtf8  = pUtf8Start;
 
    while (pTempUtf16 < pUtf16End)
    {
        if (*pTempUtf16 <= UTF8_1_END && pTempUtf8 + 1 < pUtf8End)
        {//0000 - 007F  0xxxxxxx
            *pTempUtf8++ = (Utf8)*pTempUtf16;
        }
        else if(*pTempUtf16 >= UTF8_2_START && *pTempUtf16 <= UTF8_2_END && pTempUtf8 + 2 < pUtf8End) 
        {//0080 - 07FF 110xxxxx 10xxxxxx
            *pTempUtf8++ = (*pTempUtf16 >> 6) | 0xC0;
            *pTempUtf8++ = (*pTempUtf16 & 0x3F) | 0x80;
        }
        else if(*pTempUtf16 >= UTF8_3_START && *pTempUtf16 <= UTF8_3_END && pTempUtf8 + 3 < pUtf8End)
        {//0800 - FFFF 1110xxxx 10xxxxxx 10xxxxxx
            *pTempUtf8++ = (*pTempUtf16 >> 12) | 0xE0;
            *pTempUtf8++ = ((*pTempUtf16 >> 6) & 0x3F) | 0x80;
            *pTempUtf8++ = (*pTempUtf16 & 0x3F) | 0x80;
        }
        else
        {
            break;
        }
        pTempUtf16++;
    }
    *pTempUtf8 = 0;
}

void Utf8ToUtf16(Utf8* pUtf8Start, Utf8* pUtf8End, Utf16* pUtf16Start, Utf16* pUtf16End)
{
    Utf16* pTempUtf16 = pUtf16Start;
    Utf8* pTempUtf8 = pUtf8Start;
 
    while (pTempUtf8 < pUtf8End && pTempUtf16+1 < pUtf16End)
    {
        if (*pTempUtf8 >= 0xE0 && *pTempUtf8 <= 0xEF)//是3个字节的格式
        {//0800 - FFFF 1110xxxx 10xxxxxx 10xxxxxx
            *pTempUtf16 |= ((*pTempUtf8++ & 0xEF) << 12);
            *pTempUtf16 |= ((*pTempUtf8++ & 0x3F) << 6);
            *pTempUtf16 |= (*pTempUtf8++ & 0x3F);
 
        }
        else if (*pTempUtf8 >= 0xC0 && *pTempUtf8 <= 0xDF)//是2个字节的格式
        {//0080 - 07FF 110xxxxx 10xxxxxx
            *pTempUtf16 |= ((*pTempUtf8++ & 0x1F) << 6);
            *pTempUtf16 |= (*pTempUtf8++ & 0x3F);
        }
        else if(*pTempUtf8 >= 0 && *pTempUtf8 <= 0x7F)//是1个字节的格式
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
}

/****************************************************************
    platform dependent
****************************************************************/
#ifdef WIN32
#include <windows.h>
#endif
void _delay(int ms) {
    #ifdef WIN32
    Sleep(ms);
    #else
    Sleep(ms);
    #endif
}
