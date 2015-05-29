
#ifndef _RM_STRATEGY_CONCRETE_
#define _RM_STRATEGY_CONCRETE_

/*************************************
        local strategy
*************************************/
class StrategyLocalRM : public StrategyRm {
public:
    virtual void update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive);

    virtual void load_profiles(UserProfile_t profiles[PLAYER_NUM]);
    virtual void load_card_sequence(Card_t cards[TOTAL_CARD_NUM]);

    StrategyLocalRM(RoundManager *rm);
    ~StrategyLocalRM();
private:
    int  PREMIUM_LEAST;

    void GenerateId(int IDs[PLAYER_NUM]);

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
class StrategyNetworkRM : public StrategyRm {
public:
    virtual void update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive) {}

    virtual void load_profiles(UserProfile_t profiles[PLAYER_NUM]);
    virtual void load_card_sequence(Card_t cards[TOTAL_CARD_NUM]);

    StrategyNetworkRM(RoundManager *rm);
    ~StrategyNetworkRM();
};

#endif

