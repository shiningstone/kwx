
#ifndef __RM_STRATEGY__
#define __RM_STRATEGY__

#include "./../utils/BasicType.h"

class RoundManager;
class RmStrategy {
public:
    virtual void show_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive);
/***************************************
        singleton
***************************************/
public:
    static RmStrategy *getInstance(RoundManager *rm);
    static void  destroyInstance();
private:
    RoundManager *_rm;

    int PREMIUM_LEAST;

private:
    void CalcAnGangGold(int winner,int gold[3]);
    void CalcMingGangGold(int winner,int giver,int gold[3]);
    void CalcSingleWinGold(int gold[3], int winner,int whoGive);
    void CalcDoubleWinGold(int gold[3], int giver);
    void CalcNoneWinGold(int gold[3], int giver);
    void CalcHuGold(int gold[3]);
    void CalculateGold(int gold[3],PlayerDir_t GoldWinner,GoldKind_t goldKind,PlayerDir_t whoGive);

protected:
    RmStrategy(RoundManager *rm);
    ~RmStrategy();

    static RmStrategy *_instance;
};

#endif

