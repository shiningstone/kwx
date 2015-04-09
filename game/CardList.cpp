
#include "CardList.h"

CardList::CardList(Card_t *cards,int len) {
	for(int i=0;i<len;i++) {
		CardNode_t *card = new CardNode_t;
		card->kind = cards[i];
		card->status = sFREE;
		card->canPlay = true;

		push_back(card);
	}

	_logger = LOGGER_REGISTER("CardList");
}

CardList::~CardList() {
    LOGGER_DEREGISTER(_logger);
}

Card_t CardList::get(unsigned int idx) const {
	if(idx>=size()) {
		return CARD_UNKNOWN;
	} else {
		return (at(idx))->kind;
	}
}

void CardList::show() {
	vector<CardNode_t *>::iterator it;
	char   kinds[18];
	int    idx = 0;

	for(it=begin();it!=end();it++) {
		kinds[idx++] = (*it)->kind;
	}

	LOGGER_WRITE_ARRAY(kinds,size());	
}