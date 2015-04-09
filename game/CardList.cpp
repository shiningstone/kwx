
#include "CardList.h"

CardList::CardList() {
	_logger = LOGGER_REGISTER("CardList");
}

CardList::~CardList() {
    LOGGER_DEREGISTER(_logger);
}

Card_t CardList::get_kind(unsigned int idx) const {
	if(idx>=size()) {
        LOGGER_WRITE("%s index %d is too big\n",__FUNCTION__,idx);
		return CARD_UNKNOWN;
	} else {
		return (at(idx))->kind;
	}
}

CardStatus_t CardList::get_status(unsigned int idx) const {
	if(idx>=size()) {
        LOGGER_WRITE("%s index %d is too big\n",__FUNCTION__,idx);
		return CARD_UNKNOWN;
	} else {
		return (at(idx))->status;
	}
}

void CardList::set_status(unsigned int idx,CardStatus_t status) {
	if(idx>=size()) {
        LOGGER_WRITE("%s index %d is too big\n",__FUNCTION__,idx);
	} else {
		(at(idx))->status = status;
	}
}

void CardList::push_back(Card_t kind) {
    CardNode_t *card = new CardNode_t;
    card->kind    = kind;
    card->status  = sFREE;
    card->canPlay = true;
    
    vector::push_back(card);
}

void CardList::pop_back() {
    CardNode_t *last = back();
    delete last;

	vector::pop_back();
}

void CardList::show() {
	vector<CardNode_t *>::iterator it;
	char   kinds[MAX_HANDIN_NUM];
	int    idx = 0;

	for(it=begin();it!=end();it++) {
		kinds[idx++] = (*it)->kind;
	}

	LOGGER_WRITE_ARRAY(kinds,size());	
}

/***********************************************

***********************************************/
void CardInHand::init(Card_t *cards,int len) {
	for(int i=0;i<len;i++) {
		push_back(cards[i]);
	}
}

