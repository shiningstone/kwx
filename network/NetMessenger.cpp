
#include "NetMessenger.h"

NetMessenger::NetMessenger() {
	_socket = new ClientSocket();
}

NetMessenger::~NetMessenger() {
	_socket->Stop();
	delete _socket;
}

void NetMessenger::Start() {
	_socket->Start();
}

int NetMessenger::Send(const INT8U *buf,int len) {
	return _socket->Send((char *)buf,len);
}

int NetMessenger::Recv(INT8U *buf,int *len) {
	_socket->Recv((char *)buf,len);
	return *len;
}

NetMessenger * NetMessenger::_instance = 0;
CSocket      * NetMessenger::_socket   = 0;
NetMessenger * NetMessenger::getInstance() {
	if(_instance==0) {
		_instance = new NetMessenger();
	}

	return _instance;
}
