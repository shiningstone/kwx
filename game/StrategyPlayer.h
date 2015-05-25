
#ifndef __STRATEGY_PLAYER__
#define __STRATEGY_PLAYER__

#include "GameType.h"

class Player;
class RoundManager;

class StrategyPlayer {
public:
    StrategyPlayer(Player *aPlayer);
    void assign(RoundManager *context);

    long calc_score(Card_t kind);
    void check_task(unsigned int flag);

protected:
    Player       *_employer;
    RoundManager *_ctx;
};

#endif

