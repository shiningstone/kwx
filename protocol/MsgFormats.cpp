
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

int Item::_IdType(INT8U id) {
    if( _id<50 ) {
        return PURE_ID ;
    } else if( _id>=50 && _id<128 ) {
        return ID_WITH_INT ;
    } else {
        return ID_WITH_BUF ;
    }
}

int Item::Serialize(INT8U *output) {
    output[0] = _id;
    
    switch( _IdType(_id) ) {
        case PURE_ID:
            return 1;
        case ID_WITH_INT:
            output[1] = _value;
            return 2;
        case ID_WITH_BUF:
            output[1] = _bufLen;
            memcpy(output+2, _buf, _bufLen);
            return 2+_bufLen;
    }
}

int Item::Deserialize(const INT8U *input) {
    _id = input[0];

    switch( _IdType(_id) ) {
        case PURE_ID:
            return 1;
        case ID_WITH_INT:
            _value = input[1];
            return 2;
        case ID_WITH_BUF:
            _bufLen = input[1];
            memcpy(_buf, input+2, _bufLen);
            return 2+_bufLen;
    }
}

MsgBody::MsgBody() {
    for(int i=0;i<ITEM_MAX_NUM;i++) {
        _items[i] = NULL;
    }
}

MsgBody::~MsgBody() {
    for(int i=0;i<_itemNum;i++) {
        delete _items[i];
    }
}

int MsgBody::Serialize(INT8U *output) {
    output[0] = _itemNum;

    int bodyLen = 1;

    for( int i=0; i<_itemNum; i++ ) {
        bodyLen += _items[i]->Serialize(output+bodyLen);
    }

    return bodyLen;
}

int MsgBody::Deserialize(const INT8U *input) {
    _itemNum = input[0];
    int bodyLen = 1;

    for( int i=0; i<_itemNum; i++ ) {
        _items[i] = new Item();
        bodyLen += _items[i]->Deserialize(input+bodyLen);
    }

    return bodyLen;
}
