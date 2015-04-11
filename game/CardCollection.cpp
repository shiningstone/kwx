
#include "CardCollection.h"

#define MAX_HANDIN_NUM 18

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
		return sUNDEFINDED;
	} else {
		return (at(idx))->status;
	}
}

bool CardList::canPlay(unsigned int idx) const {
	if(idx>=size()) {
        LOGGER_WRITE("%s index %d is too big\n",__FUNCTION__,idx);
		return false;
	} else {
		return (at(idx))->canPlay;
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
    
    push_back(card);
}

void CardList::push_back(CardNode_t *node) {    
    vector::push_back(node);
}

void CardList::pop_back() {
    CardNode_t *last = back();
    delete last;

	vector::pop_back();
}

void CardList::show() {
	CardList::iterator it;
	char   kinds[MAX_HANDIN_NUM];
	int    idx = 0;

	for(it=begin();it!=end();it++) {
		kinds[idx++] = (*it)->kind;
	}
    
    LOGGER_WRITE("CardList:\n");
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
/***********************************************

***********************************************/
void CardInHand::init(Card_t *cards,int len) {
    clear();

	for(int i=0;i<len;i++) {
		push_back(cards[i]);
	}

    FreeStart = 0;
    Last      = len-1;
    Residue   = (len-FreeStart)%3;
}

void CardInHand::delete_card(int from,int len) {
    CardNode_t *p[18];
    
	vector<CardNode_t *>::iterator it = begin()+from;
    int idx = 0;
    while(idx<len) {
        p[idx++] = (*it);
        it++;
    }
    
    erase(begin()+from,begin()+from+len);

    idx = 0;
    while(idx<len) {
        delete p[idx];
        idx++;
    }
}

int CardInHand::_FindInsertPoint(CardNode_t data) {
    if(data.status!=sFREE) {
        for(int i=FreeStart;i>0;i--) {
            if(get_status(i-1)!=sMING_KOU) {
                return i;
            } else if(data.kind>=get_kind(i-1)) {
                return i;
            }
        }

        return 0;
    } else {
        for(int i=FreeStart;i<=size();i++) {
            if(get_kind(i)>=data.kind) {
                return i;
            }
        }

        return size();
    }
}

void CardInHand::insert_card(CardNode_t data,int times) {
    int insertPlace = _FindInsertPoint(data);

    if(insertPlace==size()) {
        for(int i=0;i<times;i++) {
            CardNode_t *card = new CardNode_t;
            memcpy(card,&data,sizeof(CardNode_t));
            push_back(card);
        }
    } else {
        for(int i=0;i<times;i++) {
            CardNode_t *card = new CardNode_t;
            memcpy(card,&data,sizeof(CardNode_t));
            insert(begin()+insertPlace+i,card);
        }
    }
}


