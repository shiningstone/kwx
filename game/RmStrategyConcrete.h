
#ifndef _RM_STRATEGY_CONCRETE_
#define _RM_STRATEGY_CONCRETE_

/*************************************
        local strategy
*************************************/
class LocalStrategy : public RmStrategy {
public:
    virtual void update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive);

    LocalStrategy(RoundManager *rm);
    ~LocalStrategy();
private:
    int  PREMIUM_LEAST;

    void CalcAnGangGold(int winner,int gold[3],int continueGang = 1);
    void CalcMingGangGold(int winner,int giver,int gold[3],int continueGang = 1);
    void CalcSingleWinGold(int gold[3], int winner,int whoGive);
    void CalcDoubleWinGold(int gold[3], int giver);
    void CalcNoneWinGold(int gold[3], int giver);
    void CalcHuGold(int gold[3],const WinInfo_t &win);
    void CalculateGold(int gold[3],PlayerDir_t GoldWinner,GoldKind_t goldKind,PlayerDir_t whoGive);
};

/*************************************
        network strategy
*************************************/
class NetworkStrategy : public RmStrategy {
public:
    virtual void update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive) {}

    NetworkStrategy(RoundManager *rm);
    ~NetworkStrategy();
};

#endif

