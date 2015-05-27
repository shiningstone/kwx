
#include "MsgQueue.h"

MsgQueue::MsgQueue(MsgHandle *employer){
    _queue    = new std::queue<void *>();
    _listener = employer;

    this->onEnter();
    this->onEnterTransitionDidFinish();
    this->scheduleUpdate();
}

MsgQueue::~MsgQueue(){
    this->unscheduleUpdate();
    delete _queue;
}

void MsgQueue::update(float fDelta){
    _waitMutex();

    if (!_queue->empty()) {
        if(_listener!=NULL) {
            void *msg = _queue->front();
            _queue->pop();
            
            _listener->HandleMsg(msg);
        }
    }

    _clearMutex();
}

 
 
void MsgQueue::pop(){
    _waitMutex();

    _queue->pop();

    _clearMutex();
}

void MsgQueue::push(void * netPackage){
    _waitMutex();

    _queue->push(netPackage);

    _clearMutex();
}

/*************************************
        message handle
*************************************/
MsgHandle::MsgHandle():
_msgQueue(this) {
}

void MsgHandle::RecvMsg(void *val) {
    _msgQueue.push(val);
}

