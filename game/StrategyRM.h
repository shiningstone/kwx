
#ifndef __RM_STRATEGY__
#define __RM_STRATEGY__

#include "./../utils/BasicType.h"

class RoundManager;
class StrategyRm {

public:
    virtual void update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive) = 0;
    
/***************************************
        singleton
***************************************/
public:
    static StrategyRm *getInstance(RoundManager *rm);
    static void  destroyInstance();
protected:
    RoundManager *_rm;

    StrategyRm(RoundManager *rm);
    ~StrategyRm();

    static StrategyRm *_instance;
};

#endif

