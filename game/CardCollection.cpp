
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
    
    for(INT8U i=0;i<size();i++) {
        if(get_kind(i)==kind) {
            num++;
        }    
    }

    return num;
}

int CardList::find_cards(Card_t kind,int *idx,int start) const {
    int num = 0;
    
    for(INT8U i=start; i<size(); i++) {   
        if(get_kind(i)==kind) {
            if(idx!=NULL) {
                idx[num] = i;
            }

            num++;
        }
    }

    return num;
}

int CardList::get_idx_in_group(int idxInHand) const {
    Card_t kind = get_kind(idxInHand);

    for(INT8U i=0;i<4;i++) {
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

void CardList::clear() {
    while(size()>0) {
        pop_back();
    }
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

CardInHand::~CardInHand() {
    /* memory release */
    
}

void CardInHand::init(Card_t *cards,int len) {
    clear();

	for(INT8U i=0;i<len;i++) {
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

int CardInHand::real_last() const {
    if(_IncludingOthersCard) {
        return last()-1;
    } else {
        return last();
    }
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

    CardNode_t *card = new CardNode_t;
    memcpy(card,&data,sizeof(CardNode_t));

    if(insertPlace==size()) {
        for(INT8U i=0;i<times;i++) {
            push_back(card);
        }
    } else {
        for(INT8U i=0;i<times;i++) {
            insert(begin()+insertPlace+i,card);
        }
    }

    DBG_SHOW();
}

void CardInHand::lock_all_cards(bool lock) {
	for(INT8U i=FreeStart;i<size();i++) {
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

Card_t CardInHand::find_an_gang_cards(int cardIdx[]) const{/*BUG : always first group*/
    if(IsMing) {
        for(INT8U i=0; i<size(); i++) {
            int matchNum = find_cards(get_kind(i),cardIdx,i);
        
            if(matchNum==4 && get_status(i)==sMING_KOU) {
                return get_kind(i);
            }
        }
    } else {
        for(INT8U i=FreeStart; i<size(); i++) {
            int matchNum = find_cards(get_kind(i),cardIdx,i);
        
            if(matchNum==4) {
                return get_kind(i);
            }
        }
    }
    
    return CARD_UNKNOWN;
}

Card_t CardInHand::find_ming_gang_cards(int idx[],Card_t kind) const{
    if(find_cards(kind,idx)==4) {
        return kind;
    } else {
        return CARD_UNKNOWN;
    }
}

int CardInHand::find_free_cards(int idx[],Card_t kind) const {
    return find_cards(kind,idx,FreeStart);
}

void CardInHand::_AnGang(Card_t card) {
    int cardIdx[4] = {0};

    Card_t kind = find_an_gang_cards(cardIdx);
    
    CardNode_t gangCard;
    gangCard.kind    = kind;
    gangCard.status  = sAN_GANG;
    gangCard.canPlay = false;

    for(int i=3;i>=0;i--) {
        if(get_status(cardIdx[i])!=sFREE) {
            FreeStart--;
        }
        
        delete_card(cardIdx[i]);
    }
    insert_card(gangCard,4);
    
    FreeStart += 4;
    
    /* BUG : 在这里排序导致没有插牌效果 */
    CardNode_t cardsAfterGangCard[18];
    int cardsNum = 0;
    for(INT8U i=cardIdx[3]+1;i<size();i++) {
        cardsAfterGangCard[cardsNum++] = *at(i);
    }
    
    for(INT8U i=0;i<cardsNum;i++) {
        delete_card(cardIdx[3]+1+i,1);
        insert_card(cardsAfterGangCard[i],1);
    }
}

void CardInHand::_MingGang(Card_t kind) {
    CardNode_t node;
    node.kind    = kind;
    node.status  = sMING_GANG;
    node.canPlay = false;
    
    for(int i=last();i>=0;i--) {
        if(get_kind(i)==kind) {
            if(get_status(i)==sPENG || get_status(i)==sMING_KOU) {
                FreeStart--;
            }
            
            delete_card(i,1);
        }
    }
    
    insert_card(node,4);
    FreeStart += 4;
}

void CardInHand::_ShouGang() {
    int gangIdx[4];
    Card_t kind = find_an_gang_cards(gangIdx);

    CardNode_t node;
    node.kind    = kind;
    node.status  = sAN_GANG;
    node.canPlay = false;
    
#if 0
    pop_back();
    delete_card(gangIdx[0],3);
#else
    delete_card(gangIdx[0],4);
#endif

    insert_card(node,4);
    FreeStart += 4;
    
    /* WHY??? redundant??? */
    node.kind    = get_kind(last());
    node.status  = sFREE;
    node.canPlay = true;
    
    pop_back();
    insert_card(node,1);
}

void CardInHand::_Peng(Card_t card) {
    CardNode_t node;
    node.kind    = card;
    node.canPlay = false;
    node.status  = sPENG;
    
    for(INT8U i=FreeStart;i<size();i++) {
        if(get_kind(i)==card){
            delete_card(i,2);
            break;
        }
    }
    
    pop_back();
    insert_card(node,3);
    
    FreeStart += 3;

    _IncludingOthersCard = false;
}

void CardInHand::_Ming() {
    for(INT8U i=FreeStart;i<size();i++) {
        if(can_handout(i)) {
            at(i)->canPlay=true;
        } else {
            at(i)->canPlay=false;
        }
    }
}

void CardInHand::_Hu(bool isZimo) {
    CardNode_t node;
    node.kind    = get_kind(last());
    node.status  = sFREE;
    node.canPlay = false;
    
    if(isZimo) {
        pop_back();
    }

    insert_card(node,1);
}

void CardInHand::_Kou() {
    CardNode_t node;
    node.canPlay = false;
    node.status  = sMING_KOU;
    
    for(int i=last();i>=FreeStart;i--) {
        if(get_status(i)==sMING_KOU) {
            node.kind=get_kind(i);
            
            delete_card(i,1);
            insert_card(node,1);
            FreeStart += 1;
            
            i++;
        }
    }
}

void CardInHand::_CancelKou() {
    CardNode_t node;
    node.status  = sFREE;
    node.canPlay = true;

    Card_t prevKou = CARD_UNKNOWN;
    
    for(int i=FreeStart-1;i>=0;i--) {
        if(get_status(i)==sMING_KOU && get_kind(i)!=prevKou) {
            node.kind = get_kind(i);
            
            delete_card(i-2,3);
            FreeStart -= 3;
            insert_card(node,1);
            insert_card(node,1);
            insert_card(node,1);

            prevKou = node.kind;
        }
    }
}

void CardInHand::_AcceptNewCard() {
    CardNode_t node;
    node.kind    = get_kind(last());
    node.status  = sFREE;

    if(IsMing) {
        node.canPlay = false;
    } else {
        node.canPlay = true;
    }
    
    pop_back();
    insert_card(node,1);
}

CartApperance_t CardInHand::get_appearance(int idx,PlayerDir_t dir,bool isMiddleMing) const{
    CardStatus_t status = get_status(idx);

    if(status==sFREE) {
        if(IsMing) {
            return LAYDOWN_SHOW ;
        } else if (isMiddleMing) {
            return LAYDOWN_HIDE ;
        }
    } else if(status==sPENG || status==sMING_GANG) {
        return LAYDOWN_SHOW ;
    } else if(status==sAN_GANG) {
        int groupIdx = get_idx_in_group(idx);
        
        if((dir==LEFT&&groupIdx==3) || (dir==RIGHT&&groupIdx==2)) {
            if(!IsMing && isMiddleMing) {/* here must be a bug */
                if(IsMing) {
                    return LAYDOWN_SHOW;
                } else if(!IsMing&&isMiddleMing) {
                    return LAYDOWN_HIDE;
                }
            }
        }
    }

    return NORMAL_APPERANCE;
}

void CardInHand::perform(ActionId_t act,bool isZimo) {
    if(act==aAN_GANG) {
        _AnGang();
    } else if(act==aMING_GANG) {
        _MingGang(get_kind(last()));
    } else if(act==aSHOU_GANG) {
        _ShouGang();
    } else if(act==aPENG) {
        _Peng(get_kind(last()));
    } else if(act==aMING) {
        _Ming();
    } else if(act==aHU) {
        _Hu(isZimo);
    } else if(act==aKOU) {
        _Kou();
    } else if(act==aKOU_CANCEL) {
        _CancelKou();
    } else {
        _AcceptNewCard();
    }

    DBG_SHOW();
}

void CardInHand::others_perform(bool isNewDistributed,ActionId_t act,Card_t kind) {
    CardNode_t node;
    node.kind    = kind;
    node.canPlay = false;

	if(act==aPENG) {
        node.status=sPENG;
        
        delete_card(FreeStart,3);
        insert_card(node,3);
        FreeStart += 3;
	} else if(act==aMING_GANG) {
        node.status=sMING_GANG;

        delete_card(FreeStart,4);
        insert_card(node,4);
		FreeStart += 4;
	} else if(act==aAN_GANG || act==aSHOU_GANG) {
        node.kind=CARD_UNKNOWN;
        node.status=sAN_GANG;

        delete_card(FreeStart,4);
        insert_card(node,4);
		FreeStart += 4;
	} else if(act==aQi) {
		if(isNewDistributed) {
			node.status  = sFREE;
            node.canPlay = false;
            
			pop_back();
            insert_card(node,1);
		}
	}
}

int CardInHand::_FindInsertPoint(CardNode_t data) const {
    if(data.status!=sFREE) {
        if(FreeStart==0) {
            return 0;
        }
        
        for(int i=FreeStart;i>0;i--) {
            if(get_status(i-1)!=sMING_KOU) {
                return i;
            } else if(data.kind>get_kind(i-1)) {
                return i;
            }
        }

        return 0;
    } else {
        for(INT8U i=FreeStart;i<size();i++) {
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

    for(INT8U i=FreeStart;i<size();i++) {
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
    for(INT8U i=0;i<_bufKouCards.num;i++) {
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

void CardInHand::refresh_kou_cards() {
    for( int group=0; group<kou_group_num(); group++ ) {
        if(kou_group_status(group)!=sMING_KOU) {
            if( can_kou(KouGroupKind(group)) ) {
                SetGroupStatus(group,sKOU_ENABLE);
            } else {
                SetGroupStatus(group,sFREE);
            }
        }
    }
}

void CardInHand::scan_kou_cards(Card_t handingout) {
    ClearKouCardInfo();
        
    for(INT8U i=FreeStart; i<size(); i++){
        auto kind = get_kind(i);
        
        if( !IsKouInclude(kind) ) {
            int cardIdx[4] = {-1,-1,-1,-1};
            
            if(find_free_cards(cardIdx, kind)==3  && can_kou(kind,handingout)) {
                AddKouGroup(kind,cardIdx);
            }
        }
    }
}

void CardInHand::choose_all_kou_cards(Card_t handingout) {
    scan_kou_cards(handingout);

	for(INT8U i=0;i<kou_group_num();i++) {
		if(can_kou(KouGroupKind(i),handingout)) {
		    SetGroupStatus(i,sMING_KOU);
		}
	}
}

void CardInHand::clear_kou_choices() {
    for(INT8U i=0;i<_bufKouCards.num;i++) {
        SetGroupStatus(i,sFREE);
    }
}

int CardInHand::get_kou_kinds(Card_t kouKind[]) const {
    int idx = 0;
    
    for(INT8U i=0;i<_bufKouCards.num;i++) {
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
        for(INT8U i=0;i<_ming.choiceNum;i++) {
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
    
    for(INT8U i=FreeStart;i<size();i+=usedLen) {
        int sameCount = 1;
        
        for(INT8U j=i+1;j<size();j++) {
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
    Card_t kind = find_an_gang_cards(cardIdx);
    
    if(kind!=CARD_UNKNOWN) {
        return true;
    } else {
        return false;
    }
}

bool CardInHand::can_hu(Card_t newCard) const {
    SmartList cards(*this,ONLY_FREE);
    cards.len--;                    /*the last should not be included*/
    cards.insert(newCard);          /*the last inserted in order*/

    return cards.can_hu();
}

bool CardInHand::can_hu(int position, int newKind) const {
    SmartList cards(*this,ONLY_FREE);
    cards.displace(position-FreeStart,(Card_t)newKind);

    return cards.can_hu();
}

bool CardInHand::can_kou(Card_t kouKind,Card_t handingout) const {
    SmartList newCards = _Exclude(kouKind);
    
	if(handingout==CARD_UNKNOWN) {
		for(INT8U i=0;i<newCards.len;i++) {
			for(INT8U k=0;k<CARD_KIND_MAX;k++) {
                SmartList cards(newCards);
                cards.displace(i,(Card_t)k);
				if(cards.can_hu()) {
					return true;
                }
			}
        }
	} else {
		for(INT8U i=0;i<newCards.len;i++) {
			if(newCards.kind[i]==handingout) {
                for(INT8U k=0;k<CARD_KIND_MAX;k++) {
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
ActionMask_t CardInHand::judge_action(Card_t newCard,bool isNewDistributed, bool isLastOne) {
    push_back(newCard);

    if(!isNewDistributed) {
        _IncludingOthersCard = true;
    } else {
        _IncludingOthersCard = false;
    }

    ActionMask_t act = 0;
        
    if( has_shou_gang() && isNewDistributed && !isLastOne ) {
        act |= aSHOU_GANG;
	}

    int num = 0;
	for(INT8U i=0;i<last();i++) {
		if(get_kind(i)==newCard) {
            CardStatus_t status = get_status(i);
            
			if(status==sPENG) {
				if(isNewDistributed&&!isLastOne) {
					act |= aMING_GANG;
                    break;
				}
			} else if(status==sMING_KOU) {
                if(!isLastOne) {
                    act |= aMING_GANG;

                    if(isNewDistributed) {
                        act |= aAN_GANG;
                    }
                }
                
                break;
            } else if(status==sFREE && canPlay(i)) {
				num++;
			}
		}
	}

    if(num==3) {
        if(!isLastOne) {
            act |= aMING_GANG | aPENG;
        }

        if(isNewDistributed) {
            act &= ~aPENG;
            act |= aAN_GANG;
        }
    } else if(num==2 && !isNewDistributed) {
    	act |= aPENG;
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
        
	for(INT8U i=0;i<size();i++) {
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
		for(INT8U k=i+1;(k<i+4) & (k<size());k++) {
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
RobotTarget_t CardInHand::assess_aim() {
    int color = 0;
    if(PreferQingYiSe(color)) {
        if(color==0) {
            aim = SAME_TIAO;
        } else if (color==1) {
            aim = SAME_TONG;
        }
    }

    int coupleNum = GetCoupleNum();
    if(FreeStart==0 && coupleNum>=5) {
        aim = SEVEN_COUPLES;
    } else if (coupleNum>=3) {
        aim = FOUR_PENG;
    }

    aim = PI_HU;

    return aim;
}

bool CardInHand::PreferQingYiSe(int &targetColor) const{
    targetColor  = get_kind(0)/9;

    for(INT8U i=1;i<FreeStart;i++) {
        if(targetColor!=get_kind(i)/9) {
            return false;
        }
    }
    
    int colorNum = FreeStart;

    for(INT8U i=FreeStart;i<size();i++) {
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
    
    INT8U i = 0;
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
    if(aim==SAME_TIAO) {
        if(kind/9!=0&&!(act&a_HU)&&!(act&a_AN_GANG)&&!(act&a_SHOU_GANG)&&!(act&a_MING_GANG))
            return true;
    } else if(aim==SAME_TONG) {
        if(kind/9!=1&&!(act&a_HU)&&!(act&a_AN_GANG)&&!(act&a_SHOU_GANG)&&!(act&a_MING_GANG))
            return true;
    } else if(aim==SEVEN_COUPLES)
        if(!(act&a_HU)&&!(act&a_AN_GANG)&&!(act&a_SHOU_GANG)&&!(act&a_MING_GANG))
            return true;

    return false;
}

/**************************************************
        ming info
**************************************************/
long CardInHand::CalcTimes(Card_t kind) {
    update_statistics(kind);
    return sum_up_score(statHuFanMask);
}

int CardInHand::CollectTingItem(TingItem_t *tingCard,Card_t kind,const CardList *river) {
    tingCard->kind = kind;
    
    tingCard->fan  = CalcTimes(kind);
    for(INT8U i=0;i<FreeStart;i++) {
        if( get_kind(i)==kind && get_status(i)==sPENG) {
            tingCard->fan *= 2;      /*BUG why ? MingSiGui???*/
            break;
        }
    }
    
    if(river!=NULL) {
        tingCard->remain = 4 - get_num(kind) - river->get_num(kind);
    } else {
        tingCard->remain = 0;
    }

    return tingCard->remain;
}

bool CardInHand::CollectTingInfo(int position,TingInfo_t &ting,const CardList *river) {
    ting.cardNum = 0;
    ting.huNum   = 0;
    ting.cards   = new TingItem_t[9];

    for(INT8U k=0;k<CARD_KIND_MAX;k++) {
        if( can_hu(position,k) ) {
            ting.huNum = CollectTingItem(ting.cards+ting.cardNum,(Card_t)k,river);
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
        
        for(INT8U i=FreeStart;i<size();i++) {
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

void CardInHand::set_ming_info(const MingInfo_t &ming) {
    _ting = NULL;

    _ming.choiceNum = ming.choiceNum;
    _ming.handouts  = new MingChoice_t[18];

    for(INT8U i=0;i<ming.choiceNum;i++) {
        MingChoice_t  *src = ming.handouts + i;
        MingChoice_t  *choice = _ming.handouts + i;

        choice->kind = src->kind;
        choice->ting.cardNum = src->ting.cardNum;
        choice->ting.cards = new TingItem_t[9];

        for(INT8U j=0;j<choice->ting.cardNum;j++) {
            *(choice->ting.cards+j) = *(src->ting.cards+j);
        }
    }
}

/* NOTE : memory release should be pay more attention, since _ting may refer to _ming in SINGLE_GAME */
void CardInHand::set_ting_info(const TingInfo_t &ting) {
    _ting = new TingInfo_t;
    _ting->cardNum = ting.cardNum;
    _ting->cards = new TingItem_t[9];
    
    for(INT8U i=0;i<ting.cardNum;i++) {
        *(_ting->cards+i) = *(ting.cards+i);
    }
}

bool CardInHand::can_handout(int idx) const {
    if( _mingChoicesMask & (1<<idx) ) {
        return true;
    } else {
        return false;
    }
}

void CardInHand::get_hu_cards(CARD_KIND cards[],int *len) {
    collect_ming_info();
    
    if(_ting!=NULL) {
	    *len = _ting->cardNum;

        for(INT8U i=0;i<_ting->cardNum;i++) {
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

        for(INT8U i=0;i<_ming.choiceNum;i++) {
            MingChoice_t *handout = _ming.handouts + i;
            
			res->hu_cards_num[i] = handout->ting.cardNum;
			for(INT8U j=0;j<res->hu_cards_num[i];j++)
				res->hu_cards[i][j] = (CARD_KIND)(handout->ting.cards+j)->kind;
        }

        return true;
	} else {
        return false;
    }
}

void CardInHand::update_ting_num(const CardList *river) {
    if(river!=NULL && _ting!=NULL) {
        _ting->huNum = 0;
        
        for(int i=0;i<_ting->cardNum;i++) {
            _ting->huNum += CollectTingItem(_ting->cards+i,(_ting->cards+i)->kind,river);
        }
    }
}

int CardInHand::get_ting_num(Card_t choice) {
    for(int i=0;i<_ming.choiceNum;i++) {
        if(choice==(_ming.handouts+i)->kind) {
            return (_ming.handouts+i)->ting.huNum;
        }
    }
    
    return INVALID;
}

TingInfo_t *CardInHand::get_ting_info(int idx) {
    if(_ting!=NULL) {
        return _ting;
    } else {
        Card_t kind = get_kind(idx);

        for(int i=0;i<_ming.choiceNum;i++) {
            if(kind==(_ming.handouts+i)->kind) {
                return &((_ming.handouts+i)->ting);
            }
        }

        return NULL;
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

int CardInHand::rechoose_after_gang(int chosen,int gangIdx[3],bool isNewDistributed,int gang4) {
    int newChosen = 0;

    if(isNewDistributed) {
        if(chosen>gangIdx[2]) {
            newChosen = chosen + 1;
        } else {
            newChosen = chosen;
        }
    } else {
        if(chosen<gangIdx[0]) {
            newChosen = chosen + 4;
        } else if(chosen==gangIdx[0]||chosen==gangIdx[1]||chosen==gangIdx[2]) {
            newChosen = INVALID;
        } else if(gang4!=INVALID && chosen==gang4) {
            newChosen = INVALID;
        } else {
            newChosen = chosen + 1;
        }
    }

    return newChosen;
}

int CardInHand::rechoose_after_peng(int chosen,int pengIdx[2]) {
    int newChosen = 0;

    if(chosen > pengIdx[1]) {
        newChosen = chosen + 1;
    } else if(chosen== pengIdx[1] || chosen==pengIdx[0]) {
        newChosen = INVALID;
    } else if(chosen<pengIdx[0]) {
        newChosen = chosen + 3;
    }

    return newChosen;
}

typedef struct {
    INT32U fan;
    INT32U score;
}FanScore_t;

FanScore_t TblFanScore[] = {
    /*  fan               coefficient*/
	{ RH_QIANGGANG        , 2},
	{ RH_GANGHUA          , 2},
	{ RH_GANGPAO          , 2},
	{ RH_HAIDILAO         , 2},
	{ RH_MING             , 2},
	{ RH_ZIMO             , 2},
	{ RH_QINYISE          , 4},
	{ RH_SIPENG           , 2},
	{ RH_DASANYUAN        , 8},
	{ RH_XIAOSANYUAN      , 4},
	{ RH_ANSIGUI          , 4},
	{ RH_MINGSIGUI        , 2},
	{ RH_QIDUI            , 4},
	{ RH_SANYUANQIDUI     , 32},
	{ RH_HAOHUAQIDUI      , 8},
	{ RH_CHAOHAOHUAQIDUI  , 32},
	{ RH_CHAOCHAOHAOHUAQIDUI, 128},
	{ RH_KAWUXIN          , 4},
};

long CardInHand::sum_up_score(unsigned int fan) {
	long score = 1;
    
    int i = 0;
    while(i<sizeof(TblFanScore)/sizeof(FanScore_t)) {
        if(_is_active(fan,TblFanScore[i].fan)) {
            score *= TblFanScore[i].score;
        }

        i++;
    }

    return score;
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
    
    for(INT8U i=start;i<cards.size();i++) {
        kind[len++] = cards.at(i)->kind;
    }
}

SmartList::SmartList(const SmartList &orig) {
	len = 0;

    for(INT8U i=0;i<orig.len;i++) {
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
    for(INT8U i=1;i<len;i++) {
        int seqNum = 0;
        seqIdx[seqNum++] = 0;

        if((kind[i]==kind[0]+1) && (kind[i]/9==kind[0]/9)) {
            seqIdx[seqNum++] = i;

            for(INT8U j=i+1;j<len;j++) {
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
	    for(INT8U i=0;i<len;i+=2) {
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
    
	for(INT8U i=0;i<len;i++) {
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

    for(INT8U i=0;i<len;i++) {
        for(INT8U j=i+1;j<len;j++) {
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
    
    for(INT8U i=0;i<len;i++) {
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
    for(int i=len-1;i>=0;i--) {
        if(newCard<kind[i]) {
            kind[i+1] = kind[i];
            if(i==0) {
                kind[0] = newCard;
            }
        } else {
            kind[i+1] = newCard;
            break;
        }
    }

    len++;
}

/*BUG HERE!!! 不能判断包含四个一样的牌型*/
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
    
    for(INT8U i=0;i<len;i++) {
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

