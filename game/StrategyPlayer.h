
#ifndef __STRATEGY_PLAYER__
#define __STRATEGY_PLAYER__

#include "GameType.h"

class Player;
class RoundManager;

typedef struct _KindPosition {
    int           num;
    int           position[4];
}KindPosition;

typedef struct _ChooseContext_t {
    /* global */
    CardList     *river;
    int           remain;
    /* others */
    TingInfo_t   *OthersTing[2];
    /* self */
    KindPosition  cards[TOTAL_CARD_KIND];
    int           huNum;
    RobotTarget_t aim;
}ChooseContext_t;

typedef struct _ScoreContext_t {
    bool          isNewDistributed;
    bool          isLastOne;
    ActionId_t    action;
    int           continueGang;
    bool          isGangHua;
}ScoreContext_t;

class StrategyPlayer {
public:
    StrategyPlayer(Player *aPlayer);
    void assign(RoundManager *context);

    long calc_score(Card_t kind);
    void check_task(unsigned int flag);
    int  choose_card(ActionId_t &ming,bool &canKou);

protected:
    Player            *_employer;
    RoundManager      *_rm;
    ChooseContext_t    _chooseCtx;
    ScoreContext_t     _scoreCtx;

private:
    void SetScoreContext();
    long CalcScore(Card_t kind,const ScoreContext_t &ctx);

    void SetChooseContext();
    int  ChooseForMing(ActionId_t &ming,bool &canKou);
    int  ChooseCard();
    
private:/*选择要出的牌*/
    bool IsInStrictSequences(Card_t kind,int seqIdx) const;
    bool IsInSequences(Card_t kind) const;
    bool IsStable(Card_t kind) const;
    int  PickupForSameColor(int reservedColor);
    int  PickupForSevenCouples();
    int  PickupForFourPeng();
    int  PickupForPiHu();

    int  _FindSingleChar();
    int  _FindSingleAndNonSequence(Card_t HeadKind,Card_t TailKind);
    int  _FindSingleAndUnstable(Card_t HeadKind,Card_t TailKind);

    void _CollectPosition(KindPosition *info);
    
    bool OthersCanHu(Card_t kind) const;
    int  AvailNum(Card_t kind) const;

    int Robot_check_pickup_card(CARD_KIND kind,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
    int river_reserved_card(HAH *hash_table,int card);//
    int Robot_check_card_stable(HAH *card_array,CARD_KIND card);//
    int Robot_picup_single_for_samecolor(int color,HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
    int Robot_pickup_for_sevencouples(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
    int Robot_pickup_for_fourpeng(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
    int Robot_pickup_for_lowwin(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
    int Robot_pickup_single(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
};

#endif

