
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

bool NetMessenger::Recv(INT8U *pkg,int &pkgLen) {
    if ( (pkgLen=_get_available_pkg_len()) > 0 ) {
        _get_pkg_from_buffer(pkg,pkgLen);
        return true;
    } else {
        return false;
    }
}

bool NetMessenger::Recv(INT8U *pkg,int &pkgLen,RequestId_t request) {
    if ( (pkgLen=_get_available_pkg_len()) > 0 ) {
        _get_pkg_from_buffer(pkg,pkgLen);
        if( request==_ntohs(*(INT16U *)(pkg+DnHeader::REQUEST_CODE)) ) {
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

int NetMessenger::_usedLen() {
    if (_inStart>=_outStart) {
        return (_inStart-_outStart);
    } else {
        return (_inStart+BUFF_SIZE-_outStart);
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

