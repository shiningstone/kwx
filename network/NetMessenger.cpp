
#include "NetMessenger.h"

NetMessenger::NetMessenger() {
	_socket = new ClientSocket();
	_keepListen = false;
    _handle_msg = 0;

	ClearRecvBuf();
}

NetMessenger::~NetMessenger() {
	_socket->Stop();
	delete _socket;
    _socket = 0;

    _keepListen = false;
    _handle_msg = 0;
}

void NetMessenger::SetHandler(MsgHandler_t func) {
    _handle_msg = func;
}

#include <thread>
void NetMessenger::Start() {
	_socket->Start();

    if(!_keepListen) {
	    std::thread t1(&NetMessenger::_collect_bytes,this);
	    t1.detach();
    }

    if(_handle_msg) {
	    std::thread autoDetect(&NetMessenger::_collect_packages,this);
	    autoDetect.detach();
    }
}

int NetMessenger::Send(const INT8U *buf,int len) {
	return _socket->Send((char *)buf,len);
}

void NetMessenger::ClearRecvBuf() {
	memset(_pkgBuf,0,BUFF_SIZE);
	_outStart  = 0;
    _inStart   = 0;
}

bool NetMessenger::Recv(INT8U *pkg,int &pkgLen) {
    if ( (pkgLen=_get_available_pkg_len()) > 0 ) {
        _get_pkg_from_buffer(pkg,pkgLen);
        return true;
    } else {
        return false;
    }
}

bool NetMessenger::Recv(INT8U *pkg,int &pkgLen,INT16U request) {
    if ( (pkgLen=_get_available_pkg_len()) > 0 ) {
        _get_pkg_from_buffer(pkg,pkgLen);
        if( request==_get_request_id(pkg) ) {
            return true;
        } else {
            return Recv(pkg,pkgLen,request);
        }
    } else {
        return false;
    }
}

void NetMessenger::_get_pkg_from_buffer(INT8U *pkg,int pkgLen) {
	if(_outStart+pkgLen > BUFF_SIZE) {  
		int copylen = BUFF_SIZE - _outStart;  
		memcpy(pkg, _pkgBuf+_outStart, copylen);  
		memcpy((INT8U *)(pkg+copylen), _pkgBuf, pkgLen-copylen);  

        memset(_pkgBuf+_outStart,0,copylen);
        memset(_pkgBuf,0,pkgLen-copylen);
	} else {  
		memcpy(pkg, _pkgBuf+_outStart, pkgLen);  
        memset(_pkgBuf+_outStart,0,pkgLen);
	}
  
	_outStart = (_outStart + pkgLen) % BUFF_SIZE;  
}

void NetMessenger::_collect_bytes() {
	_keepListen = true;

	while(_keepListen) {
		int availLen;

		if( _inStart < _outStart )  {
			availLen   = _outStart - _inStart;
		} else {  
			availLen   = BUFF_SIZE - _inStart;
		}  
  
        if(!_keepListen) {
            return;
        }

		int inLen = 0;
        if ( _socket->Recv((char *)(_pkgBuf + _inStart), &inLen, availLen)>0 ) {
            _inStart = (_inStart+inLen) % BUFF_SIZE;
        }
	}
}

int NetMessenger::_usedLen() {
    if (_inStart>=_outStart) {
        return (_inStart-_outStart);
    } else {
        return (_inStart+BUFF_SIZE-_outStart);
    }
}

/*******************************************************
        单例模式 
*******************************************************/
NetMessenger * NetMessenger::_instance   = 0;
bool           NetMessenger::_keepListen = false;
CSocket      * NetMessenger::_socket     = 0;
MsgHandler_t   NetMessenger::_handle_msg = 0;

NetMessenger * NetMessenger::getInstance() {
	if(_instance==0) {
		_instance = new NetMessenger();
	}

	return _instance;
}

void NetMessenger::destroyInstance() {
    _keepListen = false;
    _handle_msg = false;
    delete _instance;
    _instance = 0;
}

/*******************************************************
        报文格式处理 
*******************************************************/
#include "./../protocol/MsgFormats.h"

int NetMessenger::_get_available_pkg_len() {
    int usedLen = _usedLen();
    
	if ( strstr((char *)_pkgBuf+_outStart,"KWX") && usedLen>DnHeader::DN_HEADER_LEN  ) {
		int pkgLen = _ntohs((*(INT16U *)(_pkgBuf+_outStart+DnHeader::SIZE)));
		if ( usedLen>=pkgLen ) {
			return pkgLen;
		}
	}

	return 0;
}

INT16U NetMessenger::_get_request_id(const INT8U *pkg) {
    return (_ntohs(*(INT16U *)(pkg+DnHeader::REQUEST_CODE)));
}

void NetMessenger::_collect_packages() {
    while(_keepListen && _handle_msg) {
        INT8U msg[MSG_MAX_LEN] = {0};
        int   msgLen = 0;

        while( Recv(msg, msgLen) ) {
            if(_handle_msg!=0) {
                _handle_msg(msg,msgLen);
            }
        }
    }
}
