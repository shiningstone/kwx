
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
private:
    Card_t CardInHand::_FindGangCard(int cardIdx[]);

    int _FindInsertPoint(CardNode_t data);
};

#endif

