
#include <string.h>
#include "MsgFormats.h"

INT32U _ntohl(INT32U n) {
    char *p = (char *)&n;
    char ha[4] = {0};

    ha[0] = p[3];
    ha[1] = p[2];
    ha[2] = p[1];
    ha[3] = p[0];

    return *((INT32U *)ha);
}

INT16U _ntohs(INT16U n) {
    char *p = (char *)&n;
    char ha[2] = {0};

    ha[0] = p[1];
    ha[1] = p[0];

    return *((INT16U *)ha);
}

const INT8U UpHeader::PCHC[3] = {'K','W','X',};

INT8U UpHeader::_reference[] = {
    'K','W','X',           //KWX
    0x10,                  //protocol version
    0x00,0x00,0x00,0x00,   //user id
    0x01,                  //language id
    0x01,                  //client platform
    0x00,                  //client build number
    0x00,0x00,             //customer id
    0x00,0x00,             //product id
    0x00,0x00,             //request code
    0x00,0x12,             //package size
};

int UpHeader::Serialize(INT8U *output) {
    memcpy(output,PCHC,3);
    output[PROTOCOL] = _protocol;
    *((INT32U *)(output+USER_ID)) = _ntohl(_userId);    
    output[LANGUAGE] = _language;
    output[PLATFORM] = _platform;
    output[BUILD_NO] = _buildNo;
    *((INT16U *)(output+CUSTOMER_ID)) = _ntohs(_customerId);
    *((INT16U *)(output+PRODUCT_ID)) = _ntohs(_productId);
    *((INT16U *)(output+REQUEST_CODE)) = _ntohs(_requestCode);
    *((INT16U *)(output+SIZE)) = _ntohs(_size);

    memset(output+19,0,UP_HEADER_LEN-19);

    return UP_HEADER_LEN;
}

int UpHeader::Deserialize(const INT8U *input) {
    memcpy(_pchc,input,3);

	_protocol     = input[PROTOCOL];
	_userId       = _ntohl( *(INT32U *)(input+USER_ID) );
    _language     = *(input+LANGUAGE);
    _platform     = *(input+PLATFORM);
    _buildNo      = *(input+BUILD_NO);
    _customerId   = _ntohs( *(INT16U *)(input+CUSTOMER_ID) );
    _productId    = _ntohs( *(INT16U *)(input+PRODUCT_ID) );
    _requestCode  = _ntohs( *(INT16U *)(input+REQUEST_CODE) );
    _size         = _ntohs( *(INT16U *)(input+SIZE) );

    return UP_HEADER_LEN; 
}

const INT8U DnHeader::PCHC[3] = {'K','W','X',};

int DnHeader::Serialize(INT8U *output) {
    memcpy(output,PCHC,3);
    *((INT16U *)(output+REQUEST_CODE)) = _ntohs(_requestCode);
    output[LEVEL] = _level;
    *((INT16U *)(output+SIZE)) = _ntohs(_size);

    memset(output+8,0,DN_HEADER_LEN-8);

    return DN_HEADER_LEN;
}

int DnHeader::Deserialize(const INT8U *input) {
    memcpy(_pchc,input,3);

    _requestCode  = _ntohs( *(INT16U *)(input+REQUEST_CODE) );
    _level        = *(input+LEVEL);
    _size         = _ntohs( *(INT16U *)(input+SIZE) );

    return DN_HEADER_LEN; 
}

