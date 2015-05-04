
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


	void   show();
	Card_t get_kind(unsigned int idx) const;
    CardStatus_t get_status(unsigned int idx) const;
    bool   canPlay(unsigned int idx) const;
    void   set_status(unsigned int idx,CardStatus_t status);

    int    generate_raw(Card_t *array);

protected:
    Logger *_logger;
};

class SmartList;
class CardInHand : public CardList {
public:
    CardInHand();
    
    void   init(Card_t *cards,int len);
    void   delete_card(int from,int len);
    void   insert_card(CardNode_t data);
    void   insert_card(CardNode_t data,int times=1);
    void   lock_all_cards(bool lock);

    bool   is_wait_handout() const;
    void   perform(ActionId_t act);

    int          FreeStart;
    bool         IsMing;
    ROBOT_TARGET aim;

    /* statistics , only used for calculating score */
    int statFreeCards;
    int statCouples;
    int statzhongFaBai[3];             /*zhong, fa, bai*/
    int statGroupSameNum;

    int statHuFanMask;

    /***************************************************
            kou cards info
    ***************************************************/
    int          kou_cards_num() const;

    int          kou_group_num() const;
    CardStatus_t kou_group_status(int gIdx) const;
    int          kou_card_index(int gIdx,int cIdx) const;
    void         switch_group_status(int gIdx);
    void         clear_kou_choices();
    int          get_kou_kinds(Card_t kouKind[]) const;
    Card_t       KouGroupKind(int gIdx) const;
    
    bool IsKouInclude(Card_t kind) const;
    void AddKouGroup(Card_t kind,int *idx);
    int  find_cards(int cardIdx[],Card_t card) const;
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

    /***************************************************
            antificial intelligence (for single-game only)
    ***************************************************/
    void update_statistics(Card_t huKind);
    void _JudgeDaXiaoSanYuan() ;
    void _JudgeKaWuXing(Card_t kind) ;
    void _JudgeQiDui() ;
    void _JudgePengPengHu();
    void _SetHu(INT32U hu);

    bool is_shou_gang(Card_t curActKind=CARD_UNKNOWN);
    bool can_hu(Card_t newCard) const;
    bool can_hu(int position, int newKind) const;
    bool can_kou(Card_t kouKind,PlayerDir_t dir=MIDDLE,Card_t otherHandedOut=CARD_UNKNOWN/*should provided when non-MIDDLE*/) const;
    
    ROBOT_TARGET assess_aim();
    bool is_aim_limit(unsigned int act, Card_t kind) const;
    bool PreferQingYiSe(int &targetColor) const;
    int GetCoupleNum() const;

    /*********************************
            ming info
    *********************************/
    MingInfo_t  _ming;
    TingInfo_t *_ting;
    long CalcTimes(Card_t kind);
    bool CollectTingInfo(int position,TingInfo_t &ting,const CardList *river);
    bool collect_ming_info(const CardList *river = NULL/*if don't care about remain number*/);
    void get_hu_cards(CARD_KIND cards[],int *len) const;
    bool can_handout(int idx) const;
    
    INT32U      _mingChoicesMask;             /* NOTE : this mask starts with FreeStart, not 0 */

private:
    int    _FindInsertPoint(CardNode_t data) const;
    Card_t _FindGangCard(int cardIdx[]) const;

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

