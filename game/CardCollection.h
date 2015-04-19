
#ifndef _CARD_COLLECTION_
#define _CARD_COLLECTION_

#include <vector>
using namespace std;

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

	void   show();
	Card_t get_kind(unsigned int idx) const;
    CardStatus_t get_status(unsigned int idx) const;
    bool   canPlay(unsigned int idx) const;
    void   set_status(unsigned int idx,CardStatus_t status);
    void   lock_all_cards();

    int    generate_raw(Card_t *array);

protected:
    Logger *_logger;
};

class SmartList;
class CardInHand : public CardList {
public:
    void   init(Card_t *cards,int len);
    void   delete_card(int from,int len);
    void   insert_card(CardNode_t data);
    void   insert_card(CardNode_t data,int times=1);

    void   perform(ActionId_t act);
    
    int    FreeStart;

    /* statistics , only used for calculating score */
    int statFreeCards;
    int statCouples;
    int statzhongFaBai[3];             /*zhong, fa, bai*/
    int statGroupSameNum;
    int statSameAsLastCard;
    int statFanMask;

    /***************************************************
            kou cards info
    ***************************************************/
    int          kou_group_num() const;
    CardStatus_t kou_group_status(int gIdx) const;
    int          kou_card_index(int gIdx,int cIdx) const;
    void         switch_group_status(int gIdx);
    void         clear_kou_choices();
    int          get_kou_kinds(Card_t kouKind[]) const;
    Card_t       KouGroupKind(int gIdx) const;
    
    bool IsKouInclude(Card_t kind) const;
    void AddKouGroup(Card_t kind,int *idx);
    int  FindCards(int cardIdx[],Card_t card) const;
    void SetGroupStatus(int gIdx,CardStatus_t status);
    void ClearKouCardInfo();

    SmartList _Exclude(Card_t kouKind) const;

    void cancel_ming();
    /***************************************************
            antificial intelligence (for single-game only)
    ***************************************************/
    bool IsKaWuXing(Card_t kind)const;
    void update_statistics(Card_t huKind);
    
    /***************************************************
            effect
    ***************************************************/
    void add_effect_card();
    void del_effect_card();
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

    friend class NetRRound;       /*temporary*/
    friend class TestSmartList;
};

#endif

