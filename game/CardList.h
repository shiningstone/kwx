
#ifndef _CARD_LIST_
#define _CARD_LIST_

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
    CardList(Card_t *cards,int len);
	~CardList();

	void show();
	Card_t get(unsigned int idx) const;
    
private:
    Logger *_logger;
};

#endif

