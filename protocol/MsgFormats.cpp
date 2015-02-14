
#include <string.h>
#include "KwxMsg.h"

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

const INT8U Header::PCHC[3] = {'K','W','X'};

int UpHeader::Serialize(INT8U *outMsg) {
    memcpy(outMsg,PCHC,3);
    outMsg[PROTOCOL] = _protocol;
    *((INT32U *)(outMsg+USER_ID)) = _ntohl(_userId);    
    outMsg[LANGUAGE] = _language;
    outMsg[PLATFORM] = _platform;
    outMsg[BUILD_NO] = _buildNo;
    *((INT16U *)(outMsg+CUSTOMER_ID)) = _ntohs(_customerId);
    *((INT16U *)(outMsg+PRODUCT_ID)) = _ntohs(_productId);
    *((INT16U *)(outMsg+REQUEST_CODE)) = _ntohs(_requestCode);
    *((INT16U *)(outMsg+SIZE)) = _ntohs(_size);

    memset(outMsg+19,0,UP_HEADER_LEN-19);

    return UP_HEADER_LEN;
}

int UpHeader::Deserialize(const INT8U *inMsg) {
    memcpy(_pchc,inMsg,3);

	_protocol     = inMsg[PROTOCOL];
	_userId       = _ntohl( *(INT32U *)(inMsg+USER_ID) );
    _language     = *(inMsg+LANGUAGE);
    _platform     = *(inMsg+PLATFORM);
    _buildNo      = *(inMsg+BUILD_NO);
    _customerId   = _ntohs( *(INT16U *)(inMsg+CUSTOMER_ID) );
    _productId    = _ntohs( *(INT16U *)(inMsg+PRODUCT_ID) );
    _requestCode  = _ntohs( *(INT16U *)(inMsg+REQUEST_CODE) );
    _size         = _ntohs( *(INT16U *)(inMsg+SIZE) );

    return UP_HEADER_LEN; 
}

int DnHeader::Serialize(INT8U *outMsg) {
    memcpy(outMsg,PCHC,3);
    *((INT16U *)(outMsg+REQUEST_CODE)) = _ntohs(_requestCode);
    outMsg[LEVEL] = _level;
    *((INT16U *)(outMsg+SIZE)) = _ntohs(_size);

    memset(outMsg+8,0,DN_HEADER_LEN-8);

    return DN_HEADER_LEN;
}

int DnHeader::Deserialize(const INT8U *inMsg) {
    memcpy(_pchc,inMsg,3);

    _requestCode  = _ntohs( *(INT16U *)(inMsg+REQUEST_CODE) );
    _level        = *(inMsg+LEVEL);
    _size         = _ntohs( *(INT16U *)(inMsg+SIZE) );

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

int Item::Serialize(INT8U *outMsg) {
    outMsg[0] = _id;
    
    switch( _IdType(_id) ) {
        case PURE_ID:
            return 1;
        case ID_WITH_INT:
            outMsg[1] = _value;
            return 2;
        case ID_WITH_BUF:
            outMsg[1] = _bufLen;
            memcpy(outMsg+2, _buf, _bufLen);
            return 2+_bufLen;
    }
}

int Item::Deserialize(const INT8U *inMsg) {
    _id = inMsg[0];

    switch( _IdType(_id) ) {
        case PURE_ID:
            return 1;
        case ID_WITH_INT:
            _value = inMsg[1];
            return 2;
        case ID_WITH_BUF:
            _bufLen = inMsg[1];
            memcpy(_buf, inMsg+2, _bufLen);
            return 2+_bufLen;
    }
}

MsgBody::MsgBody() {
    _itemNum = 0;

    for(int i=0;i<ITEM_MAX_NUM;i++) {
        _items[i] = NULL;
    }
}

MsgBody::~MsgBody() {
    for(int i=0;i<_itemNum;i++) {
        delete _items[i];
    }
}

int MsgBody::Serialize(INT8U *outMsg) {
    outMsg[0] = _itemNum;

    int bodyLen = 1;

    for( int i=0; i<_itemNum; i++ ) {
        bodyLen += _items[i]->Serialize(outMsg+bodyLen);
    }

    return bodyLen;
}

int MsgBody::Deserialize(const INT8U *inMsg) {
    _itemNum = inMsg[0];
    int bodyLen = 1;

    for( int i=0; i<_itemNum; i++ ) {
        _items[i] = new Item();
        bodyLen += _items[i]->Deserialize(inMsg+bodyLen);
    }

    return bodyLen;
}

KwxMsg::KwxMsg(int dir) {
    if (dir==UP_STREAM) {
        _header = new UpHeader();
    } else {
        _header = new DnHeader();
    }

    _body = new MsgBody();
}

KwxMsg::~KwxMsg() {
    delete _header;
    delete _body;
}

int KwxMsg::Serialize(INT8U *outMsg) {
    int len = 0;

    len += _header->Serialize(outMsg);
    len += _body->Serialize(outMsg+len);

    return len;
}

int KwxMsg::Deserialize(const INT8U *inMsg) {
    int len = 0;

    if (memcmp(inMsg,Header::PCHC,3)) {
        return KWX_INVALID_PCHC;
    }

    len += _header->Deserialize(inMsg);
    len += _body->Deserialize(inMsg+len);

    return len;
}