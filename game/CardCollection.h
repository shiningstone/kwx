
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
    
typedef struct {
    Card_t  kind[18];
    int     len;
}SimpleList;

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

    int    generate_raw(Card_t *array);

protected:
    Logger *_logger;
};

class CardInHand : public CardList {
public:
    void   init(Card_t *cards,int len);
    void   delete_card(int from,int len);
    void   insert_card(CardNode_t data);
    void   insert_card(CardNode_t data,int times=1);

    void   perform(ActionId_t act);
    
    int active_place;

    int FreeStart;
    int Last;
    int Residue;

    /***************************************************
            kou cards info
    ***************************************************/
    int          kou_group_num() const;
    CardStatus_t kou_group_status(int gIdx) const;
    int          kou_card_index(int gIdx,int cIdx) const;
    void         switch_group_status(int gIdx);
    Card_t       KouGroupKind(int gIdx) const;
    
    bool IsKouInclude(Card_t kind) const;
    void AddKouGroup(Card_t kind,int *idx);
    int  FindCards(int cardIdx[],Card_t card) const;
    void SetGroupStatus(int gIdx,CardStatus_t status);
    void ClearKouCardInfo();
    /***************************************************
            antificial intelligence (for single-game only)
    ***************************************************/
    bool _Has3Same(const SimpleList &cards) const ;
    bool _Has3Sequence(const SimpleList &cards) const ;
    bool _IsCharDismatched(const SimpleList &cards) const;
    bool _6Couples(const SimpleList &cards) const;
    void _Remove3Same(SimpleList &cards)const ;
    void _Remove3Sequence(SimpleList &cards)const ;
    bool PatternMatch2(const SimpleList &cards) const;

    bool PatternMatch(const SimpleList &cards) const;
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

#endif
