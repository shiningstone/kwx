
#include "CardCollection.h"

#define MAX_HANDIN_NUM 18

/*************************************************
    命名说明:
        GroupGang        4个一样花色
        GroupSame        3个一样花色
        GroupSequence    3个连续花色
        Couple           一对
        Char             字    
*************************************************/


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
	for (int i=0;i<size();i++) {
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

SmartList CardInHand::_Remove(Card_t kouKind) const {
    SmartList remainCards;
	int        match  = 0;
    
    remainCards.len = 0;

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
	for(int i=active_place;i<size();i++) {
        at(i)->canPlay = true;
	}
}

/***************************************************
        SmartList logic
***************************************************/

void CardInHand::_Remove(SmartList &cards,int idx1,int idx2) const {
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
        SmartList remain;
        remain.len = size()-active_place;
        for(int i=0;i<remain.len;i++) {
            remain.kind[i] = get_kind(active_place+i);
        }
        
        _Remove(remain,Pos4,Pos6);
        if(remain.CardsStable()) {
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
    
	if(free_num==2) {
		HuShouZhuaYi = true;
 	} else {
		int totalLen = size()-active_place;
        int usedLen  = 0;
        int GroupSameCount = 0;
        
		for(int i=active_place;i<size();i+=usedLen) {
			int sameCount = 1;
            
			for(int j=i+1;j<size();j++) {
                if(get_kind(j)==get_kind(i)) {
                    sameCount++;
                }
            }
            
            if(sameCount==4) {
                break;
            } else if(sameCount==3) {
                usedLen   += 3;
                GroupSameCount++;
            } else if(sameCount==2) {
                usedLen   += 2;
            }
		}
        
		if(GroupSameCount==4) {
			HuPengPengHu = true;
        }
	}
}

/***************************************************
    to be removed
***************************************************/
bool CardInHand::pattern_match(const SmartList &cards) const{
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

/***************************************************
    SmartList
***************************************************/
SmartList::SmartList() {
    len = 0;
}

SmartList::SmartList(const CardInHand &cards) {
    len = cards.size();

    for(int i=0;i<len;i++) {
        kind[i] = cards.at(i)->kind;
    }
}

SmartList::SmartList(const SmartList &orig) {
    len = orig.len;

    for(int i=0;i<len;i++) {
        kind[i] = orig.kind[i];
    }
}

bool SmartList::_IsFirstInGroupSame() const  {
    if(kind[0]==kind[1]&&kind[0]==kind[2]) {
        return true;
    } else {
        return false;
    }
}

bool SmartList::_IsFirstInGroupSequence() const  {
    for(int i=1;i<len;i++) {
        if((kind[i]==kind[0]+1) && (kind[i]/9==kind[0]/9)) {
            for(int j=i+1;j<len;j++) {
                if((kind[j]==kind[0]+2) && (kind[j]/9==kind[0]/9)) {
                    return true;
                }
            }
        }
    }

    return false;
}

int SmartList::_GetContinuousCoupleNum() const {
    int coupleNum = 0;
    
	if(len%2==0) {
	    for(int i=0;i<len;i+=2) {
	        if( kind[i]==kind[i+1] ) {
	            coupleNum++;
	        }
	    }
	}

    return coupleNum;
}

bool SmartList::_IsCharDismatched() const {
	int zhongNum = 0;
	int faNum=0;
	int baiNum=0;
    
	for(int i=0;i<len;i++) {
		if(kind[i]==ZHONG)
			zhongNum++;
		if(kind[i]==FA)
			faNum++;
		if(kind[i]==BAI)
			baiNum++;
	}

    if(zhongNum==1 ||faNum==1 || baiNum == 1 ) {
		return true;
    } else if(zhongNum==2 && faNum==2 && baiNum==2 && len!=12) {
        return true;
    } else {
        return false;
    }
}

void SmartList::_Remove3Same() {
    len -= 3;
    memcpy(kind,&(kind[3]),len);
}

void SmartList::_Remove3Sequence() {
    for(int i=1;i<len;i++) {
        if((kind[i]==kind[0]+1) && (kind[i]/9==kind[0]/9)) {
            for(int j=i+1;j<len;j++) {
                if((kind[j]==kind[0]+2) && (kind[j]/9==kind[0]/9)) {
                    int idx = 0;

                    for(int k=0;k<len;k++) {
                        if(k==0 || k==i || k==j) {
                            continue;
                        } else {
                            kind[idx++] = kind[k];
                        }
                    }
                    len -= 3;
					return ;
                }
            }
        }
    }
}

void SmartList::_Remove(int idx1,int idx2) {
    int idx = 0;
    
    for(int i=0;i<len;i++) {
        if(i==idx1 || i==idx2) {
            continue;
        } else {
            kind[idx] = kind[i];
            idx++;
        }
    }

    len -= 2;
}

void SmartList::_Displace(int changeIdx, Card_t card) {
    kind[changeIdx] = card;
    _Order();
}

void SmartList::_Order() {
	for(int i=0;i<len;i++) {
		for(int j=i+1;j<len;j++) {
			if(kind[j]<kind[i]) {
                Card_t temp   = kind[i];
				kind[i] = kind[j];
				kind[j] = temp;
			}
		}
	}
}

void SmartList::_Insert(Card_t newCard) {
    for(int i=len;i>=0;i--) {
        if(newCard<kind[i]) {
            kind[i] = kind[i-1];
        } else {
            kind[i] = (Card_t)newCard;
            break;
        }
    }

    len++;
}

bool SmartList::PatternMatch() const {
    if(_IsCharDismatched()) {
        return false;
    } else if(_GetContinuousCoupleNum()==6) {
        return true;
    } else {
        SmartList remainCards(*this);
        
        while(remainCards.len>0) {
            if(remainCards._IsFirstInGroupSame()) {
                SmartList subList(remainCards);
                
                subList._Remove3Same();
                if( subList.PatternMatch() ) {
                    return true;
                }
            }
        
            if(remainCards._IsFirstInGroupSequence()) {
                remainCards._Remove3Sequence();
                return remainCards.PatternMatch();
            } else {
                return false;
            }
        }
        
        return true;
    }
}

/*BUG???: cards_stable 如果有一组三个或者四个，被删掉就胡不了*/
bool SmartList::CardsStable() const {
	int i=0;
    
	while(i<len-1) {
		if(	kind[i]==kind[i+1] ) {
            SmartList remainCards(*this);

			remainCards._Remove(i,i+1);
            if(remainCards.PatternMatch()) {
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

