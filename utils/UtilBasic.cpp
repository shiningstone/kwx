
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
