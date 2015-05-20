
#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

#include "cocos2d.h"
USING_NS_CC;

class MsgHandle;
class MsgQueue:public cocos2d::CCNode {
public:
    static MsgQueue *getInstance(MsgHandle *employer);
    static void  destroyInstance();

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
    
    /***************************************
            singleton
    ***************************************/
protected:
    MsgQueue(MsgHandle *employer);
    ~MsgQueue();

    static MsgQueue *_instance;
};

class MsgHandle {
public:
    virtual void RecvMsg(void *val);
    virtual void HandleMsg(void *aMsg) = 0;
protected:
    MsgHandle();
    
    MsgQueue     *_msgQueue;
};

#endif

