
#include "NetMessenger.h"

NetMessenger::NetMessenger() {
	_socket = new ClientSocket();
	_keepListen = false;

	memset(_pktBuf,0,BUFF_SIZE);
	_usedStart = 0;
	_usedLen   = 0;
}

NetMessenger::~NetMessenger() {
	_keepListen = false;
	_socket->Stop();
	delete _socket;
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

int NetMessenger::Recv(INT8U *pkg,int *pkgLen) {
    if(_usedStart + _usedLen > BUFF_SIZE) {  
        int copylen = BUFF_SIZE - _usedStart;  
        memcpy(pkg, _pktBuf + _usedStart, copylen);  
        memcpy((INT8U *)(pkg+copylen), _pktBuf, _usedLen-copylen);  
    } else {  
        memcpy(pkg, _pktBuf+_usedStart, _usedLen);  
    }
	*pkgLen = _usedLen;
  
    _usedStart = (_usedStart + _usedLen) % BUFF_SIZE;  
    _usedLen   = 0;

	return *pkgLen;
}

NetMessenger * NetMessenger::_instance = 0;
CSocket      * NetMessenger::_socket   = 0;
NetMessenger * NetMessenger::getInstance() {
	if(_instance==0) {
		_instance = new NetMessenger();
	}

	return _instance;
}

#include <stdio.h>
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
  
		int inLen = 0;
		if ( _socket->Recv((char *)_pktBuf+availStart, &inLen, availLen)>0 ) {
			_usedLen += inLen;
		}
	}
}