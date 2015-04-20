
#include "./../RaceType.h"

#include "./../utils/UtilBasic.h"

#include "CardCollection.h"

#define MAX_HANDIN_NUM 18

/*************************************************
    命名说明:
        GroupGang        4个一样花色
        GroupSame        3个一样花色
        GroupSequence    3个连续花色
        Couple           一对
        Char             字    
        fan              番型
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
    
    statFreeCards      = 0;
    statCouples        = 0;
    statGroupSameNum   = 0;
    statzhongFaBai[0]  = 0;
    statzhongFaBai[1]  = 0;
    statzhongFaBai[2]  = 0;
    statSameAsLastCard = 0;
    statHuFanMask      = 0;
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
        
        FreeStart += 4;
        
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
        for(int i=FreeStart;i>0;i--) {
            if(get_status(i-1)!=sMING_KOU) {
                return i;
            } else if(data.kind>=get_kind(i-1)) {
                return i;
            }
        }

        return 0;
    } else {
        for(int i=FreeStart;i<size();i++) {
            if(get_kind(i)>=data.kind) {
                return i;
            }
        }

        return size();
    }
}

Card_t CardInHand::_FindGangCard(int cardIdx[]) const{
    for(int i=FreeStart; i<size(); i++) {
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
    
    for(int i=FreeStart; i<size(); i++) {   
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

void CardInHand::clear_kou_choices() {
    for(int i=0;i<_bufKouCards.num;i++) {
        SetGroupStatus(i,sFREE);
    }
}

int CardInHand::get_kou_kinds(Card_t kouKind[]) const {
    int idx = 0;
    
    for(int i=0;i<_bufKouCards.num;i++) {
        if(kou_group_status(i)==sMING_KOU) {
            kouKind[idx++] = KouGroupKind(i);    
        }
    }

    return idx;
}

SmartList CardInHand::_Exclude(Card_t kouKind) const {
    int kouIdx[4];
    FindCards(kouIdx,kouKind);

    SmartList  freeCards(*this,true);

    freeCards.remove(3,kouIdx);
    return freeCards;
}

void CardInHand::cancel_ming() {
	for(int i=FreeStart;i<size();i++) {
        at(i)->canPlay = true;
	}
}

/***************************************************
        番型计算
***************************************************/
void CardInHand::_JudgeDaXiaoSanYuan() {
    if(statzhongFaBai[0]>=3&&statzhongFaBai[1]>=3&&statzhongFaBai[2]>=3) {//澶т笁鍏?
        _set(statHuFanMask,RH_DASANYUAN);
    } else if( (statzhongFaBai[0]>=3&&statzhongFaBai[1]>=3&&statzhongFaBai[2]==2) 
            || (statzhongFaBai[0]==2&&statzhongFaBai[1]>=3&&statzhongFaBai[2]>=3) 
            || (statzhongFaBai[0]>=3&&statzhongFaBai[1]==2&&statzhongFaBai[2]>=3) ) {//灏忎笁鍏?
        _set(statHuFanMask,RH_XIAOSANYUAN);
    }
}

void CardInHand::_JudgeKaWuXing(Card_t kind) {
    SmartList freeCards(*this,true);
    if( freeCards.is_ka_wu_xing(kind) ) {
        _set(statHuFanMask,RH_KAWUXIN);
    }
}

void CardInHand::_JudgeQiDui() {
    if(statzhongFaBai[0]==2&&statzhongFaBai[1]==2&&statzhongFaBai[2]==2) {
        _set(statHuFanMask,RH_SANYUANQIDUI);
    } 
    
    if(statGroupSameNum==1) {
        _set(statHuFanMask,RH_HAOHUAQIDUI);
    } else if(statGroupSameNum==2) {
        _set(statHuFanMask,RH_CHAOHAOHUAQIDUI);
    } else if(statGroupSameNum==3) {
        _set(statHuFanMask,RH_CHAOCHAOHAOHUAQIDUI);
    } else {
        _set(statHuFanMask,RH_QIDUI);
    }
}

void CardInHand::_JudgePengPengHu() {
    int usedLen  = 0;
    int GroupSameCount = 0;
    
    for(int i=FreeStart;i<size();i+=usedLen) {
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
        } else {
            usedLen   += 1;
        }
    }
    
    if(GroupSameCount==4) {
        _set(statHuFanMask,RH_SIPENG);
    }
}

void CardInHand::update_statistics(Card_t huKind) {
    _set(statHuFanMask,RH_QINYISE);
        
    int color = huKind/9;;
        
	for(int i=0;i<size();i++) {
		if(get_status(i)==sFREE ) {
			statFreeCards++;
		}

        Card_t curCard = get_kind(i);
        if(curCard==ZHONG || curCard==FA || curCard==BAI) {
            statzhongFaBai[curCard-ZHONG] += 1;
        }

		if(	curCard/9!=color ) {
            _clr(statHuFanMask,RH_QINYISE);
		}
        
		if( curCard==huKind ) {
			if(get_status(i)==sPENG) {
                _set(statHuFanMask,RH_MINGSIGUI);
            } else {
                statSameAsLastCard++;
                if(statSameAsLastCard==4) {
                    _set(statHuFanMask,RH_ANSIGUI);
                }
            }
		}
        
		int freeSameCard = 1;
        int sameCard = 1;
		for(int k=i+1;k<i+4;k++) {
			if(curCard==get_kind(k) && get_status(k)==sFREE) {
                if( get_status(i)==sFREE ) {
                    freeSameCard++;
                }
                
				sameCard++;
            }
        }

        if(freeSameCard==2||freeSameCard==4)
			statCouples++;

        if(sameCard==4)
			statGroupSameNum++;
	}
    
	if(statFreeCards==2) {
		_set(statHuFanMask,RH_SHOUYIZHUA);
 	} else {
		_JudgePengPengHu();
	}

    _JudgeDaXiaoSanYuan();

    if(statCouples==7 && size()==14) {
        _JudgeQiDui();
    } else {
        _JudgeKaWuXing(huKind);
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

SmartList::SmartList(const CardInHand &cards,bool onlyFree) {
	len = 0;

    int start = onlyFree?(cards.FreeStart):0;
    
    for(int i=start;i<cards.size();i++) {
        kind[len++] = cards.at(i)->kind;
    }
}

SmartList::SmartList(const SmartList &orig) {
	len = 0;

    for(int i=0;i<orig.len;i++) {
        kind[len++] = orig.kind[i];
    }
}

bool SmartList::_IsFirstInGroupSame() const  {
    if(kind[0]==kind[1]&&kind[0]==kind[2]) {
        return true;
    } else {
        return false;
    }
}

bool SmartList::_IsFirstInGroupSequence(int seqIdx[3]) const  {
    for(int i=1;i<len;i++) {
        int seqNum = 0;
        seqIdx[seqNum++] = 0;

        if((kind[i]==kind[0]+1) && (kind[i]/9==kind[0]/9)) {
            seqIdx[seqNum++] = i;

            for(int j=i+1;j<len;j++) {
                if((kind[j]==kind[0]+2) && (kind[j]/9==kind[0]/9)) {
                    seqIdx[seqNum++] = j;
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

void SmartList::displace(int changeIdx, Card_t card) {
    kind[changeIdx] = card;

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

void SmartList::remove(int deleteNum,int deletes[]) {
	int deleteIdx = 0;
    int idx = 0;
    
    for(int i=0;i<len;i++) {
        if(i==deletes[deleteIdx]) {
            deleteIdx++;
        } else {
            kind[idx] = kind[i];
            idx++;
        }
    }

	len -= deleteNum;
}

void SmartList::insert(Card_t newCard) {
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

bool SmartList::_PatternMatch() const {
    if(_IsCharDismatched()) {
        return false;
    } else if(_GetContinuousCoupleNum()==6) {
        return true;
    } else {
        SmartList remainCards(*this);
        
        while(remainCards.len>0) {
            if(remainCards._IsFirstInGroupSame()) {
                SmartList subList(remainCards);

                int deletes[] = {0,1,2};
                subList.remove(3,deletes);
                
                if( subList._PatternMatch() ) {
                    return true;
                }
            }

            int seqIdx[3];
            if(remainCards._IsFirstInGroupSequence(seqIdx)) {
                remainCards.remove(3,seqIdx);
                
                return remainCards._PatternMatch();
            } else {
                return false;
            }
        }
        
        return true;
    }
}

bool SmartList::is_ka_wu_xing(Card_t wuXing) const {
    int Pos4   = INVALID;
    int Pos6   = INVALID;
    
    for(int i=0;i<len;i++) {
        if(kind[i]==wuXing-1)
            Pos4 = i;
        else if(kind[i]==wuXing+1)
            Pos6 = i;
    }
        
    if(Pos4!=INVALID && Pos6!=INVALID) {/*BUG???*/
        int deletes[] = {Pos4,Pos6};

        SmartList remain(*this);
        remain.remove(2,deletes);
        if(remain.can_hu()) {
            return true;
        }
    }

    return false;
}

/*BUG???: cards_stable 如果有一组三个或者四个，被删掉就胡不了*/
bool SmartList::can_hu() const {
	int i=0;
    
	while(i<len-1) {
		if(	kind[i]==kind[i+1] ) {
            int deletes[] = {i,i+1};

            SmartList remainCards(*this);
			remainCards.remove(2,deletes);
            if(remainCards._PatternMatch()) {
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

