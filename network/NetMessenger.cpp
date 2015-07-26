
#include <thread>

#include "CSockets.h"
#include "NetMessenger.h"

Logger *NetMessenger::_logger = 0;

NetMessenger::NetMessenger() {
	_socket = new ClientSocket();
    
	_keepListen = false;
    _running    = false;
    _handle_msg = 0;

	ClearRecvBuf();
    
    _logger = LOGGER_REGISTER("NetMessenger");
}

NetMessenger::~NetMessenger() {
    _keepListen = false;
    _running    = false;
    _handle_msg = 0;

	_socket->Stop();

    _socket = 0;
	delete _socket;

    LOGGER_DEREGISTER(_logger);
}

void NetMessenger::SetHandler(MsgHandler_t func) {
    _handle_msg = func;
}

void NetMessenger::Start(const char *serverIp,int port) {
	_socket->Start(serverIp,port);

    if(!_keepListen) {
        _keepListen = true;
	    std::thread t1(&NetMessenger::_collect_bytes,this);
	    t1.detach();
    }

    if(_handle_msg && !_running) {
        _running = true;
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
    _isEmpty   = true;

    _recvCnt = 0;
}

bool NetMessenger::Recv(INT8U *pkg,int &pkgLen) {
    if ( (pkgLen=_get_available_pkg_len()) > 0 ) {
        _get_pkg_from_buffer(pkg,pkgLen);
        return true;
    } else {
        _delay(100);
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

    if(_outStart==_inStart) {
        _isEmpty = true;
    }
    
	_outStart = (_outStart + pkgLen) % BUFF_SIZE;  
}

void NetMessenger::_collect_bytes() {
	_keepListen = true;

	while(_keepListen) {
		int availLen;

        if( _inStart==_outStart && _isEmpty ) {
            availLen   = BUFF_SIZE;
        } else if( _inStart < _outStart )  {
			availLen   = _outStart - _inStart;
		} else {  
			availLen   = BUFF_SIZE - _inStart;    /* BUG: it could be package loss when _inStart==_outStart */
		}  
  
        if(!_keepListen) {
            return;
        }

		int inLen = 0;
        if ( _socket->Recv((char *)(_pkgBuf + _inStart), &inLen, availLen)>0 ) {
            _inStart = (_inStart+inLen) % BUFF_SIZE;
            
            if(_inStart==_outStart) {
                _isEmpty = false;
            }
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
        报文格式处理 
*******************************************************/
#include "./../protocol/KwxMsgBasic.h"
#include "./../protocol/MsgFormats.h"

bool NetMessenger::_is_kwx_exist() {
    int len = BUFF_SIZE-_outStart;

    if(len>=3) {
        return (NULL!=strstr((char *)_pkgBuf+_outStart,"KWX"));
    } else {
        char temp[4] = {0};
        int  i = 0;

        for(i=0;i<len;i++) {
            temp[i] = _pkgBuf[_outStart+i];
        }
        for(i=len;i<3;i++) {
            temp[i] = _pkgBuf[i-len];
        }

        return (NULL!=strstr(temp,"KWX"));
    }
}

int NetMessenger::_get_available_pkg_len() {
    int usedLen = _usedLen();
    
	if ( _is_kwx_exist() && usedLen>DnHeader::DN_HEADER_LEN  ) {
		int pkgLen = _ntohs((*(INT16U *)(_pkgBuf + (_outStart+DnHeader::SIZE)%BUFF_SIZE)));
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

        while( _keepListen && _handle_msg && Recv(msg, msgLen) ) {
            if(_handle_msg!=0) {
                _handle_msg(msg,msgLen);
            }
        }
    }
}
