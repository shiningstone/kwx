
#ifndef __RM_STRATEGY__
#define __RM_STRATEGY__

#include "GameType.h"

class RoundManager;
class StrategyRm {

public:
    virtual void update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive) = 0;

    void get_ending_gold(int gold[PLAYER_NUM]);
/***************************************
        singleton
***************************************/
public:
    static StrategyRm *getInstance(RoundManager *rm);
    static void  destroyInstance();

protected:
    int  _roundEndingGold[3];
    
    RoundManager *_rm;

    StrategyRm(RoundManager *rm);
    ~StrategyRm();

    static StrategyRm *_instance;
};

#endif

