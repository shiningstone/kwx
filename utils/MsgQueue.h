
#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

#include "cocos2d.h"
USING_NS_CC;

class MsgHandle;
class MsgQueue:public cocos2d::CCNode {
public:
    MsgQueue(MsgHandle *employer);
    ~MsgQueue();

    void pop();
    void push(void *netPackage);
    
    virtual void update(float fDelta);
private:
    std::queue<void*>*  _queue;
    std::mutex          _mutex;
    MsgHandle        * _listener;
    /*pid_t               _owner;*/
    
    void _waitMutex()  { _mutex.lock(); /*_owner = getpid();*/ }
    void _clearMutex() { _mutex.unlock(); }
};

class MsgHandle {
public:
    friend class ErrorManager;
    
    virtual void RecvMsg(void *val);
    virtual void HandleMsg(void *aMsg) = 0;

    bool _readyToReceive;/* used to control time-consuming actions */
protected:
    MsgHandle();
    
    MsgQueue     _msgQueue;
};

#endif

