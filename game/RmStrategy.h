
#ifndef __RM_STRATEGY__
#define __RM_STRATEGY__

#include "./../utils/BasicType.h"

class RoundManager;
class RmStrategy {

public:
    virtual void update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive);
    
/***************************************
        singleton
***************************************/
public:
    static RmStrategy *getInstance(RoundManager *rm);
    static void  destroyInstance();
protected:
    RoundManager *_rm;

    RmStrategy(RoundManager *rm);
    ~RmStrategy();

    static RmStrategy *_instance;
};

#endif

