
#include "CardCollection.h"

#define MAX_HANDIN_NUM 18

//#define DEBUG
#ifdef DEBUG
#define DBG_SHOW() show()
#else
#define DBG_SHOW()
#endif

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

void CardList::lock_all_cards() {
	for (int i=0;i<=size()-1;i++) {
        at(i)->canPlay = false;
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
    DBG_SHOW();
}

void CardList::pop_back() {
    CardNode_t *last = back();
    delete last;

	vector::pop_back();
    DBG_SHOW();
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

    DBG_SHOW();
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

    DBG_SHOW();
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
        
        /* 在这里排序导致没有插牌效果 */
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

    DBG_SHOW();
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

int CardInHand::FindCards(int idx[],Card_t kind) const {
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

int  CardInHand::kou_group_num() const {
    return _bufKouCards.num;
}

int  CardInHand::kou_card_index(int gIdx,int cIdx) const {
    return _bufKouCards.group[gIdx].idx[cIdx];
}

Card_t CardInHand::KouGroupKind(int gIdx) const {
    return get_kind(kou_card_index(gIdx,0));
}

CardStatus_t CardInHand::kou_group_status(int gIdx) const {
    return get_status(kou_card_index(gIdx,0));
}

void CardInHand::SetGroupStatus(int gIdx,CardStatus_t status) {
    set_status(kou_card_index(gIdx,0),status);
    set_status(kou_card_index(gIdx,1),status);
    set_status(kou_card_index(gIdx,2),status);
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

void CardInHand::switch_group_status(int gIdx) {
    if(get_status(kou_card_index(gIdx,0))==sMING_KOU) {
        SetGroupStatus(gIdx,sKOU_ENABLE);
    } else {
        SetGroupStatus(gIdx,sMING_KOU);
    }
}

SimpleList CardInHand::_Remove(Card_t kouKind) const {
    SimpleList remainCards;
	int        match  = 0;
    
	for(int i=active_place;i<size();i++) {
		if(get_status(i)!=sMING_KOU) {
            if(get_kind(i)==kouKind && match<3) {
    			match++;
     		} else {
    			remainCards.kind[remainCards.len++] = get_kind(i);
            }
        }
	}

    return remainCards;
}

void CardInHand::cancel_ming() {
	for(int i=active_place;i<size()-1;i++) {
        at(i)->canPlay = true;
	}
}

/***************************************************
        SimpleList logic
***************************************************/
bool CardInHand::_Has3Same(const SimpleList &cards) const  {
    if(cards.kind[0]==cards.kind[1]&&cards.kind[0]==cards.kind[2]) {
        return true;
    } else {
        return false;
    }
}

bool CardInHand::_Has3Sequence(const SimpleList &cards) const  {
    for(int i=1;i<cards.len;i++) {
        if((cards.kind[i]==cards.kind[0]+1) && (cards.kind[i]/9==cards.kind[0]/9)) {
            for(int j=i+1;j<cards.len;j++) {
                if((cards.kind[j]==cards.kind[0]+2) && (cards.kind[j]/9==cards.kind[0]/9)) {
                    return true;
                }
            }
        }
    }

    return false;
}

int CardInHand::_GetSequenceCoupleNum(const SimpleList &cards) const {
    int coupleNum = 0;
    
    for(int i=0;i<cards.len;i+=2) {
        if( cards.kind[i]==cards.kind[i+1] ) {
            coupleNum++;
        }
    }

    return coupleNum;
}

bool CardInHand::_IsCharDismatched(const SimpleList &cards) const {
	int zhongNum = 0;
	int faNum=0;
	int baiNum=0;
    
	for(int i=0;i<cards.len;i++) {
		if(cards.kind[i]==ZHONG)
			zhongNum++;
		if(cards.kind[i]==FA)
			faNum++;
		if(cards.kind[i]==BAI)
			baiNum++;
	}

    if(zhongNum==1 ||faNum==1 || baiNum == 1 ) {
		return true;
    } else if(zhongNum==2 && faNum==2 && baiNum==2 && cards.len!=12) {
        return true;
    } else {
        return false;
    }
}

void CardInHand::_Remove3Same(SimpleList &cards) const {
    memcpy(cards.kind,&(cards.kind[3]),cards.len-3);
    cards.len -= 3;
}

void CardInHand::_Remove3Sequence(SimpleList &cards) const {
    for(int i=1;i<cards.len;i++) {
        if((cards.kind[i]==cards.kind[0]+1) && (cards.kind[i]/9==cards.kind[0]/9)) {
            for(int j=i+1;j<cards.len;j++) {
                if((cards.kind[j]==cards.kind[0]+2) && (cards.kind[j]/9==cards.kind[0]/9)) {
                    SimpleList newList;
                    int idx = 0;

                    for(int k=0;k<cards.len;k++) {
                        if(k==0 || k==i || k==j) {
                            continue;
                        } else {
                            newList.kind[idx++] = cards.kind[k];
                        }
                    }
                    newList.len = cards.len - 3;

                    memcpy(&cards,&newList,sizeof(SimpleList));
					return ;
                }
            }
        }
    }
}

void CardInHand::_Remove(SimpleList &cards,int idx1,int idx2) const {
    int idx = 0;
    for(int i=0;i<cards.len;i++) {
        if(i==idx1 || i==idx2) {
            continue;
        } else {
            cards.kind[idx] = cards.kind[i];
            idx++;
        }
    }

    cards.len -= 2;
}

/* _Displace+_Order has efficiency problem */
SimpleList CardInHand::_Displace(const SimpleList &input, int changeIdx, Card_t kind) const {
    SimpleList newList = input;
    newList.kind[changeIdx] = kind;

    _Order(newList);
    
    return newList;
}

void CardInHand::_Order(SimpleList &input) const {
	for(int i=0;i<input.len;i++) {
		for(int j=i+1;j<input.len;j++) {
			if(input.kind[j]<input.kind[i]) {
                Card_t temp   = input.kind[i];
				input.kind[i] = input.kind[j];
				input.kind[j] = temp;
			}
		}
	}
}

bool CardInHand::PatternMatch(const SimpleList &cards) const {
    if(_IsCharDismatched(cards)) {
        return false;
    } else if(_GetSequenceCoupleNum(cards)==6) {
        return true;
    } else {
        SimpleList remainCards;
        memcpy(&remainCards,&cards,sizeof(SimpleList));
        
        while(remainCards.len>0) {
            if(_Has3Same(remainCards)) {
                SimpleList subList;
                memcpy(&subList,&remainCards,sizeof(SimpleList));
                
                _Remove3Same(subList);
        
                if( PatternMatch(subList) ) {
                    return true;
                }
            }
        
            if(_Has3Sequence(remainCards)) {
                _Remove3Sequence(remainCards);
                return PatternMatch(remainCards);
            } else {
                return false;
            }
        }
        
        return true;
    }
}

bool CardInHand::CanHu(const SimpleList &cards) const {
	int i=0;
    
	while(i<cards.len-1) {
		if(	cards.kind[i]==cards.kind[i+1] ) {
            SimpleList remainCards;

            memcpy(&remainCards,&cards,sizeof(SimpleList));
			_Remove(remainCards,i,i+1);

            if(PatternMatch(remainCards)) {
				return true;
            } else {
                i += 2;
            }
		}
		else {
            i++;
        }
	}
    
	return false;
}

bool CardInHand::IsKaWuXing(Card_t kind) const {
    int Pos4   = INVALID;
    int Pos6   = INVALID;
    
    for(int i=active_place;i<size();i++) {
        if(get_kind(i)==kind-1)
            Pos4 = i-active_place;
        else if(get_kind(i)==kind+1)
            Pos6 = i-active_place;
    }
        
    if(Pos4!=INVALID && Pos6!=INVALID) {/*BUG???*/
        SimpleList remain;
        remain.len = size()-active_place;
        for(int i=0;i<remain.len;i++) {
            remain.kind[i] = get_kind(active_place+i);
        }
        
        _Remove(remain,Pos4,Pos6);
        if(CanHu(remain)) {
            return true;
        }
    }

    return false;
}

void CardInHand::get_statistics(Card_t huKind) const {
	unsigned char color = huKind/9;;

    unsigned char free_num = 0;
	unsigned char couple_num = 0;
    unsigned char four_num = 0;
	unsigned char last_card_same_num = 0;

    unsigned char zhongFaBaiNum[3] = {0};
    
	bool HuQingYiSe   = true;
	bool HuMingSiGui  = false;
    bool HuAnSiGui    = false;
	bool HuPengPengHu = false;
    bool HuShouZhuaYi = false;

	unsigned int  hu_flag = 0;

	for(int i=0;i<size();i++) {
		if(get_status(i)==sFREE ) {
			free_num++;
		}

        Card_t curCard = get_kind(i);

        if(curCard==ZHONG || curCard==FA || curCard==BAI) {
            zhongFaBaiNum[curCard-ZHONG] += 1;
        }

		if(	curCard/9!=color ) {
			HuQingYiSe = false;
		}
        
		if( curCard==huKind ) {
			if(get_status(i)==sPENG) {
				HuMingSiGui = true;
            } else {
                last_card_same_num++;
                if(last_card_same_num==4) {
                    HuAnSiGui = true;
                }
            }
		}
        
		int freeSameCard = 1;
		for(int k=i+1;k<i+4;k++) {
			if(curCard==get_kind(k) && get_status(i)==sFREE && get_status(k)==sFREE) {
				freeSameCard++;
            }
        }
        if(freeSameCard==2||freeSameCard==4)
			couple_num++;


        int sameCard = 1;
        for(int k=i+1;k<i+4;k++) {
			if(curCard==get_kind(k) && get_status(k)==sFREE) {
				sameCard++;
            }
        }

        if(sameCard==4)
			four_num++;
	}
    
	if(free_num==2)
	{
		HuShouZhuaYi = true;
	}
	else
	{
		int len = size()-active_place;
        
		int l_couple=0;
		for(int k=active_place;k<size();k++)
		{
			int l_num=1;
			for(int t=k+1;t<size();t++)
				if(get_kind(k)==get_kind(t))
					l_num++;
                
			if(l_num==3)
				len -= 3;

            l_num=0;

            for(int t=active_place;t<size();t++)
				if(get_kind(k)==get_kind(t))
					l_num++;

            if(l_num==2)
				l_couple++;
		}
        
		if(len==2&&l_couple==2)
			HuPengPengHu = true;
	}
}

/***************************************************
    to be removed
***************************************************/
bool CardInHand::pattern_match(const SimpleList &cards) const{
	int zhong_num=0;
	int fa_num=0;
	int bai_num=0;
    
	for(int i=0;i<cards.len;i++) {
		if(cards.kind[i]==ZHONG)
			zhong_num++;
		if(cards.kind[i]==FA)
			fa_num++;
		if(cards.kind[i]==BAI)
			bai_num++;
	}

    if(zhong_num==1 ||fa_num==1 || bai_num == 1 ) {
		return false;
    } else if(fa_num==2 && bai_num==2 && zhong_num==2 && cards.len!=12) {
        return false;
    }
    
	int i=0;

	while(i<cards.len)
	{
		if(cards.len-i>=3)
		{
			if( cards.kind[i+2]==cards.kind[i] && cards.kind[i+1]==cards.kind[i] ||
			(cards.kind[i+2]==cards.kind[i]+2 && cards.kind[i+1]==cards.kind[i]+1)&&(cards.kind[i+2]/9==cards.kind[i]/9 && cards.kind[i+2]/9==cards.kind[i+1]/9) )
				i += 3;
			else if(cards.len-i>=6)
			{
				if( (cards.kind[i+1]/9==cards.kind[i]/9 && cards.kind[i+2]/9==cards.kind[i]/9 &&
					cards.kind[i+3]/9==cards.kind[i]/9 && cards.kind[i+4]/9==cards.kind[i]/9 && cards.kind[i+5]/9==cards.kind[i]/9)
				&&( ( cards.kind[i+1]==cards.kind[i]+1 && cards.kind[i+2]==cards.kind[i]+1 &&
				cards.kind[i+3]==cards.kind[i]+2 && cards.kind[i+4]==cards.kind[i]+2 && cards.kind[i+5]==cards.kind[i]+3 ) ||
					( cards.kind[i+1]==cards.kind[i]+1 && cards.kind[i+2]==cards.kind[i]+1 &&
				cards.kind[i+3]==cards.kind[i]+1 && cards.kind[i+4]==cards.kind[i]+1 && cards.kind[i+5]==cards.kind[i]+2) ||
					( cards.kind[i+1]==cards.kind[i] && cards.kind[i+2]==cards.kind[i]+1 &&
				cards.kind[i+3]==cards.kind[i]+1 && cards.kind[i+4]==cards.kind[i]+2 && cards.kind[i+5]==cards.kind[i]+2) ) )
					i += 6;
				else if(cards.len-i>=9)
				{
					if( (cards.kind[i]/9==cards.kind[i+1]/9&&cards.kind[i]/9==cards.kind[i+2]/9&&cards.kind[i]/9==cards.kind[i+3]/9&&
					cards.kind[i]/9==cards.kind[i+4]/9&&cards.kind[i]/9==cards.kind[i+5]/9&&cards.kind[i]/9==cards.kind[i+7]/9 && cards.kind[i]/9==cards.kind[i+8]/9)
						&&( cards.kind[i+1]==cards.kind[i]+1 && cards.kind[i+2]==cards.kind[i]+1 && cards.kind[i+3]==cards.kind[i]+1 &&
					cards.kind[i+4]==cards.kind[i]+2 && cards.kind[i+5]==cards.kind[i]+2 && cards.kind[i+6]==cards.kind[i]+2  &&
					cards.kind[i+7]==cards.kind[i]+3 && cards.kind[i+8]==cards.kind[i]+3 || cards.kind[i+1]==cards.kind[i] &&
					cards.kind[i+2]==cards.kind[i]+1 && cards.kind[i+3]==cards.kind[i]+1 && cards.kind[i+4]==cards.kind[i]+1 &&
					cards.kind[i+5]==cards.kind[i]+2 && cards.kind[i+6]==cards.kind[i]+2 && cards.kind[i+7]==cards.kind[i]+2 && cards.kind[i+8]==cards.kind[i]+3) )
						i += 9;
					else if(cards.len-i==12)
					{
						if(  (cards.kind[i]/9==cards.kind[i+1]/9&&cards.kind[i]/9==cards.kind[i+2]/9&&cards.kind[i]/9==cards.kind[i+3]/9&&
						cards.kind[i]/9==cards.kind[i+4]/9&&cards.kind[i]/9==cards.kind[i+5]/9&&cards.kind[i]/9==cards.kind[i+7]/9 && 
						cards.kind[i]/9==cards.kind[i+8]/9&&cards.kind[i]/9==cards.kind[i+9]/9&&cards.kind[i]/9==cards.kind[i+10]/9&&cards.kind[i]/9==cards.kind[i+11]/9)
							&&(cards.kind[i+1]==cards.kind[i] && cards.kind[i+2]==cards.kind[i]+1 && cards.kind[i+3]==cards.kind[i]+1 &&
						cards.kind[i+4]==cards.kind[i]+1 && cards.kind[i+5]==cards.kind[i]+1 && cards.kind[i+6]==cards.kind[i]+2  &&
						cards.kind[i+7]==cards.kind[i]+2 && cards.kind[i+8]==cards.kind[i]+2 && cards.kind[i+9]==cards.kind[i]+2  &&
						cards.kind[i+10]==cards.kind[i]+3 && cards.kind[i+11]==cards.kind[i]+3) || (cards.kind[i+1]==cards.kind[i] &&
						cards.kind[i+2]==cards.kind[i+3] && cards.kind[i+4]==cards.kind[i+5] && cards.kind[i+6]==cards.kind[i+7] &&
						cards.kind[i+8]==cards.kind[i+9] && cards.kind[i+10]==cards.kind[i+11]) )
							i +=12;
						else
							break;
					}
					else
						break;
				}
				else
					break;
			}
			else
				break;
		}
		else
			break;
	}

	if(i==cards.len) {
		return true;
    } else {
        return false;
    }
}

/***************************************************
        effect
***************************************************/
void CardInHand::add_effect_card() {
    CardNode_t *effectCard = new CardNode_t(*back());
    push_back(effectCard);
}

void CardInHand::del_effect_card() {
    pop_back();
}

