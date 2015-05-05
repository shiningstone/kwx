
#include <string.h>

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

int CardList::last() const {
    return size()-1;
}

int CardList::get_num(Card_t kind) const {
    int num = 0;
    
    for(int i=0;i<size();i++) {
        if(get_kind(i)==kind) {
            num++;
        }    
    }

    return num;
}

int CardList::find_cards(int idx[],Card_t kind,int start) const {
    int num = 0;
    
    for(int i=start; i<size(); i++) {   
        if(get_kind(i)==kind) {
            idx[num++] = i;
        }
    }

    return num;
}

int CardList::get_idx_in_group(int idxInHand) const {
    Card_t kind = get_kind(idxInHand);

    for(int i=0;i<4;i++) {
        if(kind!=get_kind(idxInHand+i+1)) {
            return 4-i; 
        }
    }

    return 1;
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
CardInHand::CardInHand() {
    IsMing    = false;
	FreeStart = 0;
    
    statFreeCards      = 0;
    statCouples        = 0;
    statGroupSameNum   = 0;
    statzhongFaBai[0]  = 0;
    statzhongFaBai[1]  = 0;
    statzhongFaBai[2]  = 0;
    statHuFanMask      = 0;
}

void CardInHand::init(Card_t *cards,int len) {
    clear();

	for(int i=0;i<len;i++) {
		push_back(cards[i]);
	}

    IsMing    = false;
	FreeStart = 0;

    statFreeCards      = 0;
    statCouples        = 0;
    statGroupSameNum   = 0;
    statzhongFaBai[0]  = 0;
    statzhongFaBai[1]  = 0;
    statzhongFaBai[2]  = 0;
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

void CardInHand::lock_all_cards(bool lock) {
	for (int i=FreeStart;i<size();i++) {
        at(i)->canPlay = !lock;
	}
}

bool CardInHand::is_wait_handout() const {
    if((size()-FreeStart)%3==2) {
        return true;
    } else {
        return false;
    }
}

Card_t CardInHand::_FindGangCard(int cardIdx[]) const{/*BUG : always first group*/
    for(int i=FreeStart; i<size(); i++) {
        int matchNum = find_free_cards(cardIdx,get_kind(i));

        if(matchNum==4) {
            return get_kind(i);
        }
    }
    
    return CARD_UNKNOWN;
}

int CardInHand::find_free_cards(int idx[],Card_t kind) const {
    int num = 0;
    
    for(int i=FreeStart; i<size(); i++) {   
        if(get_kind(i)==kind) {
            idx[num++] = i;
        }
    }

    return num;
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

/***************************************************
        kou cards info
***************************************************/
int CardInHand::kou_cards_num() const {/*this function could be optimized by recording kou operation*/
    int num = 0;

    for(int i=FreeStart;i<size();i++) {
        if(get_status(i)==sMING_KOU) {
            num++;
        }
    }

    return num;
}

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
    find_free_cards(kouIdx,kouKind);

    SmartList  freeCards(*this,true);

    freeCards.remove(3,kouIdx);
    return freeCards;
}

void CardInHand::set_ming(int handout) {
    IsMing = true;
	lock_all_cards(true);

    Card_t kind = get_kind(handout);

    if(kind==CARD_UNKNOWN) {
        _ting = &((_ming.handouts+_ming.choiceNum-1)->ting);
    } else {
        for(int i=0;i<_ming.choiceNum;i++) {
            if((_ming.handouts+i)->kind==kind) {
                _ting = &((_ming.handouts+i)->ting);
                break;
            }
        }
    }
}

void CardInHand::cancel_ming() {
    IsMing = false;
    lock_all_cards(false);
}

/***************************************************
        番型计算
***************************************************/
void CardInHand::_JudgeDaXiaoSanYuan() {
    if(statzhongFaBai[0]>=3&&statzhongFaBai[1]>=3&&statzhongFaBai[2]>=3) {
        _SetHu(RH_DASANYUAN);
    } else if( (statzhongFaBai[0]>=3&&statzhongFaBai[1]>=3&&statzhongFaBai[2]==2) 
            || (statzhongFaBai[0]==2&&statzhongFaBai[1]>=3&&statzhongFaBai[2]>=3) 
            || (statzhongFaBai[0]>=3&&statzhongFaBai[1]==2&&statzhongFaBai[2]>=3) ) {
        _SetHu(RH_XIAOSANYUAN);
    }
}

void CardInHand::_JudgeKaWuXing(Card_t kind) {
    SmartList freeCards(*this,true);
    if( freeCards.is_ka_wu_xing(kind) ) {
        _SetHu(RH_KAWUXIN);
    }
}

void CardInHand::_JudgeQiDui() {
    if(statzhongFaBai[0]==2&&statzhongFaBai[1]==2&&statzhongFaBai[2]==2) {
        _SetHu(RH_SANYUANQIDUI);
    } 
    
    if(statGroupSameNum==1) {
        _SetHu(RH_HAOHUAQIDUI);
    } else if(statGroupSameNum==2) {
        _SetHu(RH_CHAOHAOHUAQIDUI);
    } else if(statGroupSameNum==3) {
        _SetHu(RH_CHAOCHAOHAOHUAQIDUI);
    } else {
        _SetHu(RH_QIDUI);
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
        _SetHu(RH_SIPENG);
    }
}

bool CardInHand::has_shou_gang() const {/*BUG??? : should compare with current gang card's kind*/
    int cardIdx[4] = {0};
    Card_t kind = _FindGangCard(cardIdx);
    
    if(kind!=CARD_UNKNOWN) {
        return true;
    } else {
        return false;
    }
}

bool CardInHand::can_hu(Card_t newCard) const {
    SmartList cards(*this);
    cards.len--;                    /*the last should not be included*/
    cards.insert(newCard);          /*the last inserted in order*/

    return cards.can_hu();
}

bool CardInHand::can_hu(int position, int newKind) const {
    SmartList cards(*this,true);
    cards.displace(position-FreeStart,(Card_t)newKind);

    return cards.can_hu();
}

bool CardInHand::can_kou(Card_t kouKind,PlayerDir_t dir,Card_t otherHandedOut) const {
    SmartList newCards = _Exclude(kouKind);
    
	if(dir==MIDDLE) {
		for(int i=0;i<newCards.len;i++) {
			for(int k=0;k<CARD_KIND_MAX;k++) {
                SmartList cards(newCards);
                cards.displace(i,(Card_t)k);
				if(cards.can_hu()) {
					return true;
                }
			}
        }
	} else {
		for(int i=0;i<newCards.len;i++) {
			if(newCards.kind[i]==otherHandedOut) {
                for(int k=0;k<CARD_KIND_MAX;k++) {
                    SmartList cards(newCards);
                    cards.displace(i,(Card_t)k);
                    if(cards.can_hu()) {
                        return true;
                    }
                }

                return false;
			}
		}
	}
    
	return false;
}

/***************************************************
        strategy
***************************************************/
ActionMask_t CardInHand::judge_action(bool isNewDistributed, bool isLastOne) const {
    Card_t       newCard = get_kind(last());
    ActionMask_t act = 0;
        
	if(!IsMing) {
        if( has_shou_gang() && isNewDistributed && !isLastOne ) {
            act |= aSHOU_GANG;
		}

        int freeNum = 0;
		for(int i=0;i<last();i++) {
			if(get_kind(i)==newCard) {
				if(get_status(i)==sPENG) {
					if(isNewDistributed&&!isLastOne) {
						act |= aMING_GANG;
                        break;
					}
				} else if(get_status(i)==sFREE) {
					freeNum++;
				}
			}
		}

        if(freeNum==3) {
            if(isNewDistributed) {
                if(!isLastOne) {
                    act |= (aMING_GANG | aAN_GANG);
                }
            } else {
                if(!isLastOne) {
                    act |= (aMING_GANG | aPENG);
                } else {
                    act |= aPENG;
                }
            }
        } else if(freeNum==2&&!isNewDistributed) {
        	act |= aPENG;
        }
	} else {
		for(int i=0;i<FreeStart;i++) {
			if(get_status(i)==sMING_KOU && get_kind(i)==newCard) {
				if(!isLastOne) {
					if(isNewDistributed)
						act |= (aMING_GANG | aAN_GANG);
					else
						act |= aMING_GANG;
				}
                
				break;
			}
        }
	}

	return act;
}

/***************************************************
        antificial intelligence (for single-game only)
***************************************************/
void CardInHand::_SetHu(INT32U hu) {
    _set(statHuFanMask,hu);
}

void CardInHand::update_statistics(Card_t huKind) {
    _SetHu(RH_QINYISE);
        
    int color = huKind/9;
    int sameAsHuKind = 1;
        
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
                _SetHu(RH_MINGSIGUI);
            } else {
                sameAsHuKind++;
                if(sameAsHuKind==4) {
                    _SetHu(RH_ANSIGUI);
                }
            }
		}
        
		int freeSameCard = 1;
        int sameCard = 1;
		for(int k=i+1;(k<i+4) & (k<size());k++) {
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

    if(IsMing) {
		_SetHu(RH_MING);
    }
    
	if(statFreeCards==2) {
		_SetHu(RH_SHOUYIZHUA);
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
        牌型判断
***************************************************/
ROBOT_TARGET CardInHand::assess_aim() {
    int color = 0;
    if(PreferQingYiSe(color)) {
        if(color==0) {
            aim = SAME_TIAO_TARGET;
        } else if (color==1) {
            aim = SAME_TONG_TARGET;
        }
    }

    int coupleNum = GetCoupleNum();
    if(FreeStart==0 && coupleNum>=5) {
        aim = SEVEN_COUPLES_TARGET;
    } else if (coupleNum>=3) {
        aim = FOUR_PENG_TARGET;
    }

    aim = PI_HU_TARGET;

    return aim;
}

bool CardInHand::PreferQingYiSe(int &targetColor) const{
    targetColor  = get_kind(0)/9;

    for(int i=1;i<FreeStart;i++) {
        if(targetColor!=get_kind(i)/9) {
            return false;
        }
    }
    
    int colorNum = FreeStart;

    for(int i=FreeStart;i<size();i++) {
        if(targetColor!=get_kind(i)/9) {
            break;
        } else {
            colorNum++;
        }
    }    

    if(colorNum>=9) {
        return true;
    } else {
        return false;
    }
}

int CardInHand::GetCoupleNum() const {
    int coupleNum = 0;
    
    int i = 0;
    while(i<size()) {
        if(get_kind(i)==get_kind(i+1)) {
            coupleNum++;
            i += 2;
        } else {
            i += 1;
        }
    }

    return coupleNum;
}

bool CardInHand::is_aim_limit(unsigned int act, Card_t kind) const {
    if(aim==SAME_TIAO_TARGET) {
        if(kind/9!=0&&!(act&a_HU)&&!(act&a_AN_GANG)&&!(act&a_SHOU_GANG)&&!(act&a_MING_GANG))
            return true;
    } else if(aim==SAME_TONG_TARGET) {
        if(kind/9!=1&&!(act&a_HU)&&!(act&a_AN_GANG)&&!(act&a_SHOU_GANG)&&!(act&a_MING_GANG))
            return true;
    } else if(aim==SEVEN_COUPLES_TARGET)
        if(!(act&a_HU)&&!(act&a_AN_GANG)&&!(act&a_SHOU_GANG)&&!(act&a_MING_GANG))
            return true;

    return false;
}

/**************************************************
        ming info
**************************************************/
#include "Ai.h"
long CardInHand::CalcTimes(Card_t kind) {
    update_statistics(kind);
    return Ai::getInstance()->sum_up_score(statHuFanMask);
}

bool CardInHand::CollectTingInfo(int position,TingInfo_t &ting,const CardList *river) {
    ting.cardNum = 0;
    ting.cards   = new TingItem_t[9];

    for(int k=0;k<CARD_KIND_MAX;k++) {
        if( can_hu(position,k) ) {
            TingItem_t *tingCard = ting.cards + ting.cardNum;
            
            tingCard->kind = (Card_t)k;
            
            tingCard->fan  = CalcTimes((Card_t)k);
            for(int i=0;i<FreeStart;i++) {
                if( get_kind(i)==k && get_status(i)==sPENG) {
                    tingCard->fan *= 2;      /*BUG why ? MingSiGui???*/
                    break;
                }
            }

            if(river!=NULL) {
                tingCard->remain = 4 - get_num((Card_t)k) - river->get_num((Card_t)k);
            }
            
            ting.cardNum++;
        }
    }

    return (ting.cardNum>0);
}

bool CardInHand::collect_ming_info(const CardList *river) {
    if(IsMing) {
        return true;
    } else {
        _mingChoicesMask = 0;
        _ming.choiceNum = 0;
        _ming.handouts  = new MingChoice_t[18];
        
        _ting = NULL;
        
        Card_t prevCardCanHu = CARD_UNKNOWN;
        
        for(int i=FreeStart;i<size();i++) {
            if(get_status(i)==sMING_KOU) {
                continue;
            }
        
            Card_t kind = get_kind(i);
            if(kind==prevCardCanHu && prevCardCanHu!=CARD_UNKNOWN) {
                _mingChoicesMask |= 1<<i;          /*??? handoutMask |= ( ( handoutMask&( 1<<(i-1) ) )<<1 );*/
                continue;
            }
        
            MingChoice_t *choice = _ming.handouts + _ming.choiceNum;
            if( CollectTingInfo(i,choice->ting,river) ) {
                choice->kind = kind;
                
                _ming.choiceNum++;
                _mingChoicesMask |= 1<<i;
            }
        }
        
        return (_ming.choiceNum>0);
    }
}

bool CardInHand::can_handout(int idx) const {
    if( _mingChoicesMask & (1<<idx) ) {
        return true;
    } else {
        return false;
    }
}

void CardInHand::get_hu_cards(CARD_KIND cards[],int *len) const {
    if(_ting!=NULL) {
	    *len = _ting->cardNum;

        for(int i=0;i<_ting->cardNum;i++) {
		    cards[i] = (CARD_KIND)(_ting->cards+i)->kind;
        }
    } else {
        *len = 0;
    }
}

bool CardInHand::get_ming_info(MRES *res) const {
	if(_ming.choiceNum>0) {
		res->hu_places_num = _ming.choiceNum;
		res->hu_places     = _mingChoicesMask;

        for(int i=0;i<_ming.choiceNum;i++) {
            MingChoice_t *handout = _ming.handouts + i;
            
			res->hu_cards_num[i] = handout->ting.cardNum;
			for(int j=0;j<res->hu_cards_num[i];j++)
				res->hu_cards[i][j] = (CARD_KIND)(handout->ting.cards+j)->kind;
        }

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

