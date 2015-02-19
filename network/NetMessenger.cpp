
#include "NetMessenger.h"

NetMessenger::NetMessenger() {
	_socket = new ClientSocket();
	_keepListen = false;

	ClearRecvBuf();
}

NetMessenger::~NetMessenger() {
	_socket->Stop();
	delete _socket;
    _socket = 0;

    _keepListen = false;
}

#include <thread>
void NetMessenger::Start() {
	_socket->Start();
    if(!_keepListen) {
	    std::thread listener(&NetMessenger::_listen,this);
	    listener.detach();
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
	if ( _is_pkg_header_exists() ) {
		pkgLen = _ntohs((*(INT16U *)(_pkgBuf+_outStart+DnHeader::SIZE)));

		if ( _is_pkg_body_completed(pkgLen) ) {
			_get_pkg_from_buffer(pkg,pkgLen);
			return true;
		}
	}

	return false;
}

bool NetMessenger::Recv(INT8U *pkg,int &pkgLen,RequestId_t request) {
	if ( _is_pkg_header_exists() ) {
		pkgLen = _ntohs((*(INT16U *)(_pkgBuf+_outStart+DnHeader::SIZE)));

		if ( _is_pkg_body_completed(pkgLen) ) {
			_get_pkg_from_buffer(pkg,pkgLen);
            if( request==_ntohs(*(INT16U *)(pkg+DnHeader::REQUEST_CODE)) ) {
    			return true;
            } else {
                return Recv(pkg,pkgLen,request);
            }
		}
	}

	return false;
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

void NetMessenger::_listen() {
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
        /* 这里最好能够使用互斥量 */
        if ( _socket->Recv((char *)(_pkgBuf + _inStart), &inLen, availLen)>0 ) {
            _inStart = (_inStart+inLen) % BUFF_SIZE;
        }
	}
}

NetMessenger * NetMessenger::_instance = 0;
bool           NetMessenger::_keepListen = false;
CSocket      * NetMessenger::_socket   = 0;
NetMessenger * NetMessenger::getInstance() {
	if(_instance==0) {
		_instance = new NetMessenger();
	}

	return _instance;
}

void NetMessenger::destroyInstance() {
    _keepListen = false;
    delete _instance;
    _instance = 0;
}

bool NetMessenger::_is_pkg_header_exists() {
	if ( strstr((char *)_pkgBuf+_outStart,"KWX") && _usedLen()>DnHeader::DN_HEADER_LEN ) {
		return true;
	} else {
		return false;
	}
}

int NetMessenger::_usedLen() {
    if (_inStart>=_outStart) {
        return (_inStart-_outStart);
    } else {
        return (_inStart+BUFF_SIZE-_outStart);
    }
}

bool NetMessenger::_is_pkg_body_completed(int pkgLen) {
	if ( _usedLen()>=pkgLen ) {
		return true;
	} else {
		return false;
	}
}
