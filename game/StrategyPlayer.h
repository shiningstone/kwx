
#ifndef __STRATEGY_PLAYER__
#define __STRATEGY_PLAYER__

#include "utils/BasicType.h"

class Player;
class RoundManager;

class StrategyPlayer {
public:
    StrategyPlayer(Player *aPlayer);
    void assign(RoundManager *context);

    long calc_score(Card_t kind,bool isNewDistributed,bool isLastOne,ActionId_t last_action_WithGold,unsigned int continue_gang_times,bool isGangHua);//·ÖÊý¼ÆËã
    void check_task(unsigned int flag);

protected:
    Player       *_employer;
    RoundManager *_ctx;
};

#endif

