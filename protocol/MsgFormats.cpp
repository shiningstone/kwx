
#include <string.h>
#include "MsgFormats.h"
#include "EnvVariables.h"

const INT8U Header::PCHC[3] = {'K','W','X'};

UpHeader::UpHeader() {
	EnvVariable *env = EnvVariable::getInstance();

	_protocol   = env->_protocol;
	_userId     = env->_userId;
	_language   = env->_language;
	_platform   = env->_platform;
	_buildNo    = env->_buildNo;
	_customerId = env->_customerId;
	_productId  = env->_productId;
}

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

Item::Item() {
}

Item::Item(Item_t itemId, INT8U value) {
	_id       = itemId;
	_value    = value;
}

Item::Item(Item_t itemId, INT8U bufLen,INT8U *buf) {
	_id       = itemId;
	_bufLen   = bufLen;
	memcpy(_buf,buf,bufLen);
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
		default:
			return -1;
    }
}

int Item::Deserialize(const INT8U *inMsg) {
    _id = (Item_t)inMsg[0];

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
		default:
			return -1;
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
