
#ifndef __RM_STRATEGY__
#define __RM_STRATEGY__

#include "./../utils/BasicType.h"

class RoundManager;
class RmStrategy {
public:
    virtual void update_gold_account();
/***************************************
        singleton
***************************************/
public:
    static RmStrategy *getInstance(RoundManager *rm);
    static void  destroyInstance();
private:
    RoundManager *_rm;
protected:
    RmStrategy(RoundManager *rm);
    ~RmStrategy();

    static RmStrategy *_instance;
};

#endif

