
#ifndef __RM_STRATEGY__
#define __RM_STRATEGY__

#include "GameType.h"
#include "DataBase.h"

class RoundManager;
class StrategyRm {

public:
    virtual void update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive) = 0;

    virtual void load_profiles(UserProfile_t profile[PLAYER_NUM]) = 0;
    virtual void load_card_sequence(Card_t cards[84]) = 0;

    void get_ending_gold(int gold[PLAYER_NUM]);

    int  _roundHuGold[3];
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

