
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
	std::thread listener(&NetMessenger::_listen,this);
	listener.detach();
}

int NetMessenger::Send(const INT8U *buf,int len) {
	return _socket->Send((char *)buf,len);
}

void NetMessenger::ClearRecvBuf() {
	memset(_pkgBuf,0,BUFF_SIZE);
	_usedStart = 0;
	_usedLen   = 0;
}

void NetMessenger::_get_pkg_from_buffer(INT8U *pkg,int pkgLen) {
	if(_usedStart + pkgLen > BUFF_SIZE) {  
		int copylen = BUFF_SIZE - _usedStart;  
		memcpy(pkg, _pkgBuf + _usedStart, copylen);  
		memcpy((INT8U *)(pkg+copylen), _pkgBuf, pkgLen-copylen);  
	} else {  
		memcpy(pkg, _pkgBuf+_usedStart, pkgLen);  
	}
  
	_usedStart = (_usedStart + pkgLen) % BUFF_SIZE;  
	_usedLen   -= pkgLen;
    printf("decrease: start %d - len %d\n",_usedStart,_usedLen);
}

bool NetMessenger::Recv(INT8U *pkg,int &pkgLen) {
	if ( _is_pkg_header_exists() ) {
		pkgLen = _ntohs((*(INT16U *)(_pkgBuf+DnHeader::SIZE)));

		if ( _is_pkg_body_completed(pkgLen) ) {
			_get_pkg_from_buffer(pkg,pkgLen);
			return true;
		}
	}

	return false;
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

void NetMessenger::_listen() {
	_keepListen = true;

	while(_keepListen) {
		int availLen, availStart;

		if( _usedStart + _usedLen < BUFF_SIZE )  {                  // INBUF中的剩余空间有回绕   
			availStart = _usedStart + _usedLen;
			availLen   = BUFF_SIZE - availStart;                    // 后部空间长度，最大接收数据的长度
		} else {  
			availStart = (_usedStart + _usedLen) % BUFF_SIZE;
			availLen   = BUFF_SIZE - _usedLen;
		}  
  
        if(!_keepListen) {
            return;
        }
		int inLen = 0;
		if ( _socket->Recv((char *)_pkgBuf+availStart, &inLen, availLen)>0 ) {
			_usedLen += inLen;
            printf("increase: start %d(new start %d) - len %d(new len %d)\n",_usedStart,availStart,_usedLen,inLen);
        }
	}
}

bool NetMessenger::_is_pkg_header_exists() {
	if ( strstr((char *)_pkgBuf+_usedStart,"KWX") && _usedLen>DnHeader::DN_HEADER_LEN ) {
		return true;
	} else {
		return false;
	}
}

bool NetMessenger::_is_pkg_body_completed(int pkgLen) {
	if ( _usedLen>=pkgLen ) {
		return true;
	} else {
		return false;
	}
}
