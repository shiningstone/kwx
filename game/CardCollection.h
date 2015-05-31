
#ifndef _CARD_COLLECTION_
#define _CARD_COLLECTION_

#include <vector>
using namespace std;

#include "./../RaceType.h"
#include "GameType.h"

#include "./../utils/LogManager.h"

class CardList : public vector<CardNode_t *> {
public:
    CardList();
	~CardList();

    virtual void push_back(CardNode_t *node);
	virtual void push_back(Card_t kind);
	virtual void pop_back();/*NOTE: this operation will DESTROY the memory*/
    virtual void clear();

    int  last() const;
    int  get_num(Card_t kind) const;
    int  get_idx_in_group(int idxInHand) const;

	void show();
	Card_t get_kind(unsigned int idx) const;
    CardStatus_t get_status(unsigned int idx) const;
    bool canPlay(unsigned int idx) const;
    void set_status(unsigned int idx,CardStatus_t status);

    int  find_cards(Card_t kind,int *idx=NULL,int start = 0) const;
protected:
    Logger *_logger;

};

class SmartList;
class RoundManager;
class CardInHand : public CardList {
public:
    CardInHand();
    ~CardInHand();
    
    void   init(Card_t *cards,int len);
    void   delete_card(int from,int len=1);
    void   insert_card(CardNode_t data,int times=1);
    void   lock_all_cards(bool lock);
    int    real_last()const;

    bool   is_wait_handout() const;
    int    find_free_cards(int cardIdx[],Card_t card) const;

    CartApperance_t get_appearance(int idx,PlayerDir_t dir,bool isMiddleMing) const;

    void   perform(ActionId_t act,bool isZimo=false);
    void   others_perform(bool isNewDistributed,ActionId_t act,Card_t kind);

    int          FreeStart;
    bool         IsMing;
    RobotTarget_t aim;

    Card_t find_an_gang_cards(int cardIdx[] = NULL) const;
    Card_t find_ming_gang_cards(int idx[],Card_t kind) const;
    /***************************************************
            Alternatives
    ***************************************************/
    friend class Alternatives;
    Alternatives *_alter;
private:
    void   _AnGang(Card_t kind = CARD_UNKNOWN);
    void   _MingGang(Card_t kind);
    void   _ShouGang();
    void   _Peng(Card_t kind);
    void   _Ming();
    void   _Hu(bool isNewDistributed);
    void   _AcceptNewCard();
    
    void   _Kou();
    void   _CancelKou();

public:
    bool   _IncludingOthersCard;

    /* statistics , only used for calculating score */
    int statFreeCards;
    int statCouples;
    int statzhongFaBai[3];             /*zhong, fa, bai*/
    int statGroupSameNum;

    INT32U statHuFanMask;

    SmartList _Exclude(Card_t kouKind) const;
    void set_ming(int handout);
    void cancel_ming();

    /***************************************************
            effect
    ***************************************************/
    void add_effect_card();
    void del_effect_card();

    static int rechoose_after_gang(int chosen,int gangIdx[3],bool isNewDistributed,int gang4=INVALID/* not necessary for MingGang*/);
    static int rechoose_after_peng(int chosen,int pengIdx[2]);
    /***************************************************
            strategy
    ***************************************************/
    ActionMask_t judge_action(Card_t newCard,bool isNewDistribute, bool isLast);
    
    /***************************************************
            antificial intelligence (for single-game only)
    ***************************************************/
    void update_statistics(Card_t huKind);
    void _JudgeDaXiaoSanYuan() ;
    void _JudgeKaWuXing(Card_t kind) ;
    void _JudgeQiDui() ;
    void _JudgePengPengHu();
    void _SetHu(INT32U hu);

    bool has_shou_gang() const;
    bool can_hu(Card_t newCard) const;
    bool can_hu(int position, int newKind) const;
    bool can_kou(Card_t kouKind,Card_t handingout=CARD_UNKNOWN) const;
    
    RobotTarget_t assess_aim();
    bool is_aim_limit(unsigned int act, Card_t kind) const;
    bool PreferQingYiSe(int &targetColor) const;
    int GetCoupleNum() const;

    static long sum_up_score(unsigned int fan);
    /*********************************
            ming info
    *********************************/
    MingInfo_t  _ming;
    TingInfo_t *_ting;
    long CalcTimes(Card_t kind);//∑¨–Õº∆À„
    int  CollectTingItem(TingItem_t *tingCard,Card_t kind,const CardList *river);
    bool CollectTingInfo(int position,TingInfo_t &ting,const CardList *river);

    bool collect_ming_info(const CardList *river = NULL/*if don't care about remain number*/);
    void set_ming_info(const MingInfo_t &info);/* for MIDDLE only*/
    void set_ting_info(const TingInfo_t &ting);/*for others only*/
    void update_ting_num(const CardList *river);

    int  get_ting_num(Card_t kind);
    void get_hu_cards(CARD_KIND cards[],int *len);
    bool can_handout(int idx) const;
    bool get_ming_info(MRES *res) const;
    TingInfo_t *get_ting_info(int idx);

    INT32U      _mingChoicesMask;             /* NOTE : this mask starts with FreeStart, not 0 */

private:
    static const bool ONLY_FREE = true;
    
    int    _FindInsertPoint(CardNode_t data) const;
};

/***************************************************
        Alternatives : 
            used to support choose card in some speicail situation
***************************************************/
class Alternatives {
public:
    Alternatives(CardInHand *cards);

    void scan(ActionId_t action,Card_t reference = CARD_IGNORE);

    void switch_status(int gIdx);
    void refresh();
    void active_all(Card_t handingout=CARD_IGNORE);
    void clear();

    int  get_activated_kinds(Card_t kinds[]) const;
    Card_t get_cards(int idx[],ActionId_t *action = NULL) const;

    bool is_activated(int gIdx) const;
    int  activated_cards_num() const;
    int  group_num() const;
    int  cards_num(int gIdx) const;
    int  get_card_idx(int gIdx,int cIdx) const;
    CardStatus_t get_status(int gIdx) const;

private:
    ActionId_t   _action;

    void   Init(ActionId_t action);
    void   ScanKouCards(Card_t handingout);
    void   ScanGangCards(Card_t newHandIn);
    void   AddGroup(int num,int *idx,CardStatus_t activeStatus,CardStatus_t freeStatus);

    Card_t GetKind(int gIdx) const;
    void   SetStatus(int gIdx,CardStatus_t status) const;
    bool   IsInclude(Card_t kind) const;

    
    typedef struct {
        CardStatus_t ACTIVE_STATUS;
        CardStatus_t FREE_STATUS;        

        int          cardNum;
        int          idx[4];
    }AlterGroup_t;

    int          _groupNum;
    AlterGroup_t _group[4];

    CardInHand   *_cards;
};


/***************************************************
        SmartList: for card kind judge
***************************************************/
class SmartList {
public:
    SmartList();
    SmartList(const SmartList &orig);
    SmartList(const CardInHand &cards,bool onlyFree=false);
    
	void remove(int num,int deletes[]);
    void insert(Card_t kind);
    void displace(int changeIdx, Card_t kind);
    
    bool is_ka_wu_xing(Card_t wuXing)const;
    bool can_hu()const;

protected:
    Card_t  kind[18];
    int     len;

    bool _IsFirstInGroupSame() const ;
    bool _IsFirstInGroupSequence(int seqIdx[3]) const ;
    bool _IsCharDismatched() const;
    int  _GetContinuousCoupleNum() const;
    bool _PatternMatch() const;

    friend class CardInHand;
    friend class TestSmartList;
    friend void test_can_hu();
};

#endif

