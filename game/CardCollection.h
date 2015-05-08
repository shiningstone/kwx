
#ifndef _CARD_COLLECTION_
#define _CARD_COLLECTION_

#include <vector>
using namespace std;

#include "./../RaceType.h"

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

typedef enum {
	sFREE = 0,
	sPENG,
	sMING_GANG,
	sAN_GANG,
	sMING_KOU,
	sKOU_ENABLE,
	sUNDEFINDED=0xff,
}CardStatus_t;

typedef struct _CardNode_t {
	Card_t       kind;
	CardStatus_t status;
	bool         canPlay;
}CardNode_t;

class CardList : public vector<CardNode_t *> {
public:
    CardList();
	~CardList();

    virtual void push_back(CardNode_t *node);
	virtual void push_back(Card_t kind);
	virtual void pop_back();/*NOTE: this operation will DESTROY the memory*/

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
    
    void   init(Card_t *cards,int len);
    void   delete_card(int from,int len);
    void   insert_card(CardNode_t data);
    void   insert_card(CardNode_t data,int times=1);
    void   lock_all_cards(bool lock);

    bool   is_wait_handout() const;
    int    find_free_cards(int cardIdx[],Card_t card) const;

    void   perform(ActionId_t act,bool isZimo=false);
    void   others_perform(bool isNewDistributed,ActionId_t act,Card_t kind);

    int          FreeStart;
    bool         IsMing;
    ROBOT_TARGET aim;

    Card_t find_an_gang_cards(int cardIdx[]) const;
    Card_t find_ming_gang_cards(int idx[],Card_t kind) const;
    /***************************************************
            action
    ***************************************************/
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
    /* statistics , only used for calculating score */
    int statFreeCards;
    int statCouples;
    int statzhongFaBai[3];             /*zhong, fa, bai*/
    int statGroupSameNum;

    int statHuFanMask;

    /***************************************************
            robot interface
    ***************************************************/
    void         choose_all_kou_cards(Card_t handingout);
    
    /***************************************************
            kou cards info
    ***************************************************/
    void         scan_kou_cards(Card_t handingout = CARD_UNKNOWN/* it is not necessary for MIDDLE */);
    
    int          kou_cards_num() const;

    int          kou_group_num() const;
    CardStatus_t kou_group_status(int gIdx) const;
    int          kou_card_index(int gIdx,int cIdx) const;
    void         switch_group_status(int gIdx);
    void         refresh_kou_cards();
    void         clear_kou_choices();
    int          get_kou_kinds(Card_t kouKind[]) const;
    Card_t       KouGroupKind(int gIdx) const;
    
    bool IsKouInclude(Card_t kind) const;
    void AddKouGroup(Card_t kind,int *idx);
    void SetGroupStatus(int gIdx,CardStatus_t status);
    void ClearKouCardInfo();

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
    ActionMask_t judge_action(bool isNewDistribute, bool isLast) const;
    
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
    
    ROBOT_TARGET assess_aim();
    bool is_aim_limit(unsigned int act, Card_t kind) const;
    bool PreferQingYiSe(int &targetColor) const;
    int GetCoupleNum() const;

    static long sum_up_score(unsigned int fan);
    /*********************************
            ming info
    *********************************/
    MingInfo_t  _ming;
    TingInfo_t *_ting;
    long CalcTimes(Card_t kind);
    bool CollectTingInfo(int position,TingInfo_t &ting,const CardList *river);
    bool collect_ming_info(const CardList *river = NULL/*if don't care about remain number*/);
    void get_hu_cards(CARD_KIND cards[],int *len);
    bool can_handout(int idx) const;
    bool get_ming_info(MRES *res) const;
    
    INT32U      _mingChoicesMask;             /* NOTE : this mask starts with FreeStart, not 0 */

private:
    int    _FindInsertPoint(CardNode_t data) const;

    typedef struct {
        int    idx[3];
    }KouGroup_t;

    typedef struct {
        int        num;
        KouGroup_t group[4];
    }KouCards_t;

    KouCards_t _bufKouCards;
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
};

#endif

