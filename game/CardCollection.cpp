
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
	char   kinds[84];
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

void CardInHand::perform(ActionId_t act) {
    if(act==aAN_GANG) {
        int cardIdx[4] = {0};
        Card_t kind = _FindGangCard(cardIdx);

        CardNode_t gangCard;
        gangCard.kind    = kind;
        gangCard.status  = sAN_GANG;
        gangCard.canPlay = false;

        delete_card(cardIdx[0],4);
        insert_card(gangCard,4);
        
        active_place += 4;

        CardNode_t cardsAfterGangCard[18];
        int cardsNum = 0;
        for(int i=cardIdx[3]+1;i<size();i++) {
            cardsAfterGangCard[cardsNum++] = *at(i);
        }

        for(int i=0;i<cardsNum;i++) {
            delete_card(cardIdx[3]+1+i,1);
            insert_card(cardsAfterGangCard[i],1);
        }
    }
}

int CardInHand::_FindInsertPoint(CardNode_t data) const {
    if(data.status!=sFREE) {
        for(int i=active_place;i>0;i--) {
            if(get_status(i-1)!=sMING_KOU) {
                return i;
            } else if(data.kind>=get_kind(i-1)) {
                return i;
            }
        }

        return 0;
    } else {
        for(int i=active_place;i<size();i++) {
            if(get_kind(i)>=data.kind) {
                return i;
            }
        }

        return size();
    }
}

Card_t CardInHand::_FindGangCard(int cardIdx[]) const{
    for(int i=active_place; i<size(); i++) {
        cardIdx[0] = i;
        int matchCardNum = 1;
            
        for(int j=i+1; j<size(); j++) {
            if(get_kind(i)==get_kind(j)) {
                cardIdx[matchCardNum++] = j;

                if(matchCardNum==4) {
                    return get_kind(cardIdx[0]);
                }
            } else {
                break;
            }
        }
    }
    
    return CARD_UNKNOWN;
}

int CardInHand::_FindCards(int idx[],Card_t kind) const {
    int num = 0;
    
    for(int i=active_place; i<size(); i++) {   
        if(get_kind(i)==kind) {
            idx[num++] = i;
        }
    }

    return num;
}

/***************************************************
        kou cards info
***************************************************/
void CardInHand::ClearKouCardInfo() {
    memset(&_bufKouCards,0,sizeof(KouCards_t));
}

int  CardInHand::KouGroupNum() const {
    return _bufKouCards.num;
}

int  CardInHand::KouCardIndex(int gIdx,int cIdx) const {
    return _bufKouCards.group[gIdx].idx[cIdx];
}

Card_t CardInHand::KouGroupKind(int gIdx) const {
    return get_kind(KouCardIndex(gIdx,0));
}

CardStatus_t CardInHand::KouGroupStatus(int gIdx) const {
    return get_status(KouCardIndex(gIdx,0));
}

void CardInHand::SetGroupStatus(int gIdx,CardStatus_t status) {
    set_status(KouCardIndex(gIdx,0),status);
    set_status(KouCardIndex(gIdx,1),status);
    set_status(KouCardIndex(gIdx,2),status);
}

void CardInHand::AddKouGroup(Card_t kind,int *idx) {
    _bufKouCards.group[_bufKouCards.num].idx[0] = idx[0];
    _bufKouCards.group[_bufKouCards.num].idx[1] = idx[1];
    _bufKouCards.group[_bufKouCards.num].idx[2] = idx[2];

    SetGroupStatus(_bufKouCards.num,sKOU_ENABLE);

    _bufKouCards.num++;
}

bool CardInHand::IsKouInclude(Card_t kind) const {
    for(int i=0;i<_bufKouCards.num;i++) {
        if(KouGroupKind(i)==kind) {
            return true;
        }
    }

    return false;
}

void CardInHand::SwitchGroupStatus(int gIdx) {
    if(get_status(KouCardIndex(gIdx,0))==sMING_KOU) {
        SetGroupStatus(gIdx,sKOU_ENABLE);
    } else {
        SetGroupStatus(gIdx,sMING_KOU);
    }
}

