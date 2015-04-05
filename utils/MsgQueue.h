
#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

#include "cocos2d.h"
USING_NS_CC;

class NetRoundManager;
class MsgQueue:public cocos2d::CCNode {
public:
    static MsgQueue *getInstance();
    static void  destroyInstance();

    void setListener(NetRoundManager *listener);
    void pop();
    void push(void *netPackage);
    
    virtual void update(float fDelta);
private:
    std::queue<void*>*  _queue;
    std::mutex          _mutex;
    NetRoundManager *   _listener;
    /*pid_t               _owner;*/
    
    void _waitMutex()  { _mutex.lock(); /*_owner = getpid();*/ }
    void _clearMutex() { _mutex.unlock(); }
    
    /***************************************
            singleton
    ***************************************/
    protected:
        MsgQueue();
        ~MsgQueue();
    
        static MsgQueue *_instance;
};

#endif

