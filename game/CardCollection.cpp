
#include "CardCollection.h"

CardList::CardList() {
	_logger = LOGGER_REGISTER("CardList");
}

CardList::~CardList() {
    LOGGER_DEREGISTER(_logger);
}

void CardList::init(Card_t *cards,int len) {
	for(int i=0;i<len;i++) {
		CardNode_t *card = new CardNode_t;
		card->kind = cards[i];
		card->status = sFREE;
		card->canPlay = true;

		vector::push_back(card);
	}
}

Card_t CardList::get_kind(unsigned int idx) const {
	if(idx>=size()) {
		return CARD_UNKNOWN;
	} else {
		return (at(idx))->kind;
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
	CardList::iterator it;
	char   kinds[18];
	int    idx = 0;

	for(it=begin();it!=end();it++) {
		kinds[idx++] = (*it)->kind;
	}

	LOGGER_WRITE_ARRAY(kinds,size());	
}

int CardList::generate_raw(Card_t * array) {
	CardList::iterator it;
    int idx = 0;

	for(it=begin();it!=end();it++) {
		array[idx++] = (*it)->kind;
	}

    return idx;
}
