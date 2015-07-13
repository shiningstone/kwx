
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


//#define CARD_DEBUG
#ifdef CARD_DEBUG
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
		return CARD_UNKNOWN;
	} else {
		return (at(idx))->kind;
	}
}

CardStatus_t CardList::get_status(unsigned int idx) const {
	if(idx>=size()) {
		return sUNDEFINDED;
	} else {
		return (at(idx))->status;
	}
}

bool CardList::canPlay(unsigned int idx) const {
	if(idx>=size()) {
		return false;
	} else {
		return (at(idx))->canPlay;
	}
}

void CardList::set_status(unsigned int idx,CardStatus_t status) {
	if(idx<size()) {
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
    if(size()>0) {
        CardNode_t *last = back();
        delete last;
        
        vector::pop_back();
        DBG_SHOW();
    }
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
    
    LOGGER_WRITE("CardList:");
	LOGGER_WRITE_ARRAY(kinds,size());	
}

/***********************************************

***********************************************/
CardInHand::CardInHand() {
    _alter = new Alternatives(this);

    IsMing    = false;
    memset(&_ming,0,sizeof(MingInfo_t));
    _ting = NULL;

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

    delete _alter;    
}

void CardInHand::init(Card_t *cards,int len) {
    clear();

	for(INT8U i=0;i<len;i++) {
		push_back(cards[i]);
	}

    IsMing    = false;
	FreeStart = 0;

    _ClearStats();
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

void CardInHand::insert_card(CardNode_t data,int times,bool isZimo) {
    int insertPlace = _FindInsertPoint(data,isZimo);

    CardNode_t *card[4] = {0};
    for(int i=0;i<times;i++) {
        card[i] = new CardNode_t;
        memcpy(card[i],&data,sizeof(CardNode_t));
    }

    if(insertPlace==size()) {
        for(INT8U i=0;i<times;i++) {
            push_back(card[i]);
        }
    } else {
        for(INT8U i=0;i<times;i++) {
            insert(begin()+insertPlace+i,card[i]);
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
        for(INT8U i=0; i<size()-3; i++) {
            int matchNum = find_cards(get_kind(i),cardIdx,i);
        
            if(matchNum==4 && get_status(i)==sMING_KOU) {
                return get_kind(i);
            }
        }
    } else {
        for(INT8U i=FreeStart; i<size()-3; i++) {
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

void CardInHand::FindAnGangCards(int *idx) const {
    int num = 0;
    
    for(INT8U i=0; i<size(); i++) {   
        if(get_status(i)==sAN_GANG || get_status(i)==sSHOU_GANG) {
            idx[num++] = i;
        }
    }
}

void CardInHand::refresh(CardNode_t cards[],int len) {
    int num = size()-FreeStart;

    for(int i=0;i<num;i++) {
        pop_back();
    }
    
    for(int i=0;i<len;i++) {
        if(cards[i].status==sPENG || cards[i].status==sMING_GANG) {
            continue;
        } else if(cards[i].status==sAN_GANG || cards[i].status==sSHOU_GANG){
            int idx[4] = {0};

            FindAnGangCards(idx);
            for(int k=0;k<4;k++) {
                (at(idx[k]))->kind = cards[i].kind;
            }
        } else {
            insert_card(cards[i]);
        }
    }
}

void CardInHand::_AnGang(Card_t card) {
    int cardIdx[4] = {0};
    Card_t kind = CARD_UNKNOWN;

    if(card==CARD_UNKNOWN) {
        kind = find_an_gang_cards(cardIdx);
    } else {
        kind = card;
        find_cards(kind,cardIdx);
    }
    
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
    
    //for(INT8U i=0;i<cardsNum;i++) {
    //    delete_card(cardIdx[3]+1+i,1);
    //    insert_card(cardsAfterGangCard[i],1);
    //}   
	if(cardsNum>0)
	{
		delete_card(cardIdx[3]+cardsNum,1);
		insert_card(cardsAfterGangCard[cardsNum-1],1);
	}
}

void CardInHand::_MingGang(Card_t kind,bool isZimo) {
    CardNode_t node;
    node.kind    = kind;
    node.status  = sMING_GANG;
    node.canPlay = false;
    
	if(isZimo)
	{
		delete_card(last(),1);
		insert_card(node,1,isZimo);
		for(int i=0;i<FreeStart;i++)
			if(get_kind(i)==kind)
			{
				set_status(i,sMING_GANG);
				set_status(i+1,sMING_GANG);
				set_status(i+2,sMING_GANG);
				break;
			}
		FreeStart++;
	}
	else
	{
		for(int i=last();i>=0;i--) {
			if(get_kind(i)==kind) {
				if(get_status(i)==sPENG || (get_status(i)==sMING_KOU && i!=last())) {/*the status of last card will be modified to sMING_KOU after scan/clear*/
					FreeStart--;
				}
				delete_card(i,1);
			}
		}
		insert_card(node,4,isZimo);
		FreeStart += 4;
	}
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

void CardInHand::perform(ActionId_t act,Card_t kind,bool isZimo) {
    if(act==aAN_GANG) {
        _AnGang(kind);
    } else if(act==aMING_GANG) {
        _MingGang(kind,isZimo);
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
    } else if(act==aKOU_CANCEL || act==aMING_CANCEL) {
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

int CardInHand::_FindInsertPoint(CardNode_t data,bool isZimo) const {
    if(data.status!=sFREE) {
        if(FreeStart==0) {
            return 0;
        }

		for(int i=FreeStart;i>0;i--) {
			if(data.status==sMING_GANG&&isZimo)
			{
				if(get_kind(i-1)==data.kind)
					return i;
			}
			else
			{
				if(get_status(i-1)!=sMING_KOU) {
					return i;
				} else if(data.kind>get_kind(i-1)) {
					return i;
				}
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
    INT32U checked    = 0;
    int    GroupSameCount = 0;
    int    remainKindNum = 0;
    int    remainCardNum = 0;
    
    for(INT8U i=0;i<size()-1;i++) {
        int sameCount = 1;

        if(!_is_active(checked,1<<i)) {

            _set(checked,1<<i);

            for(INT8U j=i+1;j<size();j++) {
                if(get_kind(j)==get_kind(i)) {
                    _set(checked,1<<j);
                    sameCount++;
                }
            }

            if(sameCount>=3) {
                GroupSameCount++;
            } else {
                remainKindNum++;
                remainCardNum += sameCount;
            }
        }
    }
    
    if(GroupSameCount==4 && remainKindNum==1 && remainCardNum==2) {
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

    _alter->scan_gang(isNewDistributed);
    if(!isLastOne) {
        act |= _alter->get_actions();
    }

    if(!isNewDistributed && !IsMing) {
        if( find_cards(newCard,NULL,FreeStart)==3 ) {
            act |= aPENG;
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

void CardInHand::_ClearStats() {
    statFreeCards      = 0;
    statCouples        = 0;
    statGroupSameNum   = 0;
    statzhongFaBai[0]  = 0;
    statzhongFaBai[1]  = 0;
    statzhongFaBai[2]  = 0;
    statHuFanMask      = 0;
}

void CardInHand::update_statistics(Card_t huKind) {
    _ClearStats();

    _SetHu(RH_QINYISE);
        
    int color = huKind/9;
    int sameAsHuKind = 0;
        
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
            sameAsHuKind++;
			if(get_status(i)==sPENG) {
                _SetHu(RH_MINGSIGUI);
            } else {
                if(sameAsHuKind==4) {
                    _SetHu(RH_ANSIGUI);
                }
            }
		}
        
		int freeSameCard = 1;
        int sameCard = 1;
		for(INT8U k=i+1;k<size();k++) {
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
 	}
    
    if(_is_active(statHuFanMask,RH_QINYISE) && statGroupSameNum>0) {
        _SetHu(RH_ANSIGUI);
    }

	_JudgePengPengHu();

    _JudgeDaXiaoSanYuan();

    if(statCouples==7 && size()==14) {
        _clr(statHuFanMask,RH_ANSIGUI);
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
    while(i<size()-1) {
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
    ting.kindNum = 0;
    ting.cardNum   = 0;
    ting.cards   = new TingItem_t[9];

    for(INT8U k=0;k<CARD_KIND_MAX;k++) {
        if( can_hu(position,k) ) {
            ting.cardNum += CollectTingItem(ting.cards+ting.kindNum,(Card_t)k,river);
            ting.kindNum++;
        }
    }

    return (ting.kindNum>0);
}

bool CardInHand::scan_ming(const CardList *river) {
    if(IsMing) {
        return true;
    } else {
        //show();
    
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

void CardInHand::load_ming_info(const MingInfo_t &ming) {
    if(ming.choiceNum>0) {
        _mingChoicesMask = 0;
        
        _ting = NULL;

        _ming.choiceNum = ming.choiceNum;
        _ming.handouts  = new MingChoice_t[18];
        
        for(INT8U i=0;i<ming.choiceNum;i++) {
            MingChoice_t  *src = ming.handouts + i;
            MingChoice_t  *choice = _ming.handouts + i;
        
            choice->kind = src->kind;
            choice->ting.kindNum = src->ting.kindNum;
            choice->ting.cards = new TingItem_t[9];
        
            for(INT8U j=0;j<choice->ting.kindNum;j++) {
                *(choice->ting.cards+j) = *(src->ting.cards+j);
            }

            int idx[4] = {0};
            int num = find_cards(src->kind,idx);
            for(INT8U j=0;j<num;j++) {
                _mingChoicesMask |= 1<<idx[j];
            }
        }
    }
}

/* NOTE : memory release should be pay more attention, since _ting may refer to _ming in SINGLE_GAME */
void CardInHand::set_ting_info(const TingInfo_t &ting) {
    _ting = new TingInfo_t;
    _ting->kindNum = ting.kindNum;
    _ting->cards = new TingItem_t[9];
    
    for(INT8U i=0;i<ting.kindNum;i++) {
        *(_ting->cards+i) = *(ting.cards+i);
    }
}

void CardInHand::set_ting_info(int tingNum,Card_t tingCards[]) {
    _ting = new TingInfo_t;
    _ting->kindNum = tingNum;
    _ting->cards = new TingItem_t[9];
    
    for(INT8U i=0;i<tingNum;i++) {
        (_ting->cards+i)->kind = tingCards[i];
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
    scan_ming();
    
    if(_ting!=NULL) {
	    *len = _ting->kindNum;

        for(INT8U i=0;i<_ting->kindNum;i++) {
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
            
			res->hu_cards_num[i] = handout->ting.kindNum;
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
        _ting->cardNum = 0;
        
        for(int i=0;i<_ting->kindNum;i++) {
            _ting->cardNum += CollectTingItem(_ting->cards+i,(_ting->cards+i)->kind,river);
        }
    }
}

int CardInHand::get_ting_num(Card_t choice) {
    for(int i=0;i<_ming.choiceNum;i++) {
        if(choice==(_ming.handouts+i)->kind) {
            return (_ming.handouts+i)->ting.cardNum;
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
        if( (i!=deletes[deleteIdx]) || (deleteIdx==deleteNum) ) {
            kind[idx] = kind[i];
            idx++;
        } else {
            deleteIdx++;
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

/***************************************************
        Alternatives : 
            used to support choose card in some speicail situation
***************************************************/
Alternatives::Alternatives(CardInHand * cards) {
    _cards = cards;
}

void Alternatives::Init(ActionId_t action) {
    _action = action;
    
    for(int i=0;i<4;i++) {
        if(_action==aKOU) {
            _group[i].ACTIVE_STATUS = sMING_KOU;
            _group[i].FREE_STATUS   = sKOU_ENABLE;
            _group[i].cardNum = 3;
        } else {
            _group[i].ACTIVE_STATUS = sUNDEFINDED;
            _group[i].FREE_STATUS   = sUNDEFINDED;
            _group[i].cardNum = 0;
        }

        memset(_group[i].idx,INVALID,4);
    }

    _groupNum = 0;
}

void Alternatives::load_kou_info(const Card_t kinds[],int num) {
    clear();

    Init(aKOU);
    
    if(num>0) {
        for(int i=0;i<num;i++) {
            int cardIdx[3] = {0};
            _cards->find_cards(kinds[i],cardIdx);
            AddGroup(3,cardIdx,sMING_KOU,sKOU_ENABLE);
        }
    }
}

void Alternatives::scan_kou(Card_t handingout) {
    Init(aKOU);
    
    for(INT8U i=_cards->FreeStart; i<_cards->size(); i++){
        auto kind = _cards->get_kind(i);
        
        if( !IsInclude(kind) ) {
            int cardIdx[4] = {-1,-1,-1,-1};
            
            if(_cards->find_free_cards(cardIdx, kind)==3  && _cards->can_kou(kind,handingout)) {
                AddGroup(3,cardIdx,sMING_KOU,sKOU_ENABLE);
            }
        }
    }
}

void Alternatives::load_gang_info(ActionMask_t actions,const Card_t kinds[],int num) {
    clear();

    Init(aGANG);
    
    if(num>0) {
        for(int i=0;i<num;i++) {
            int cardIdx[4] = {0};
            int cardNum = _cards->find_cards(kinds[i],cardIdx);
        
            if(actions & aAN_GANG) {
                AddGroup(4,cardIdx,sAN_GANG,sGANG_ENABLE);
            } else if(actions & aMING_GANG) {
                AddGroup(cardNum,cardIdx,sMING_GANG,sGANG_ENABLE);
            } else {
                AddGroup(4,cardIdx,sSHOU_GANG,sGANG_ENABLE);
            }
        }
    }
}

/*假如底金：100
明杠：1.A手上有三张A，B打出A        B给A  100*2            A收200
      2.自己碰过A，再次摸起A时杠    B、C各给A  100*1       A收200
暗杠：A手上共有四张A                B、C各给A  100*2       A收400
      1.手上有三张A再抓起A时杠        anGang
      2.手上有连续的四张A             shouGang
*/
void Alternatives::scan_gang(bool isNewDistributed) {
    Init(aGANG);
    
	auto lastKind = _cards->get_kind(_cards->last());

    int cardIdx[4] = {-1,-1,-1,-1};
    
    if(_cards->find_cards(lastKind,cardIdx)==4) {
        CardStatus_t orig = _cards->get_status(cardIdx[0]);

        if(isNewDistributed) {
            if(orig==sPENG) {
                AddGroup(4,cardIdx,sMING_GANG,sGANG_ENABLE);
            } else if(_cards->IsMing && orig==sMING_KOU){
                AddGroup(4,cardIdx,sAN_GANG,sGANG_ENABLE);
            } else {
                AddGroup(4,cardIdx,sAN_GANG,sGANG_ENABLE);
            }
        } else {
            if(orig==sFREE || orig==sMING_KOU) {
                AddGroup(4,cardIdx,sMING_GANG,sGANG_ENABLE);
            }
        }
    } else if(isNewDistributed) {
        for(int i=_cards->FreeStart;i<_cards->last();i++) {
            if(_cards->find_cards(_cards->get_kind(i),cardIdx,i)==4) {
                AddGroup(4,cardIdx,sSHOU_GANG,sGANG_ENABLE);
            }
        }
    }
}

bool Alternatives::is_activated(int gIdx) const {
    return (get_status(gIdx)==_group[gIdx].ACTIVE_STATUS);
}

int Alternatives::activated_cards_num() const {/*only used in KOU*/
    int num = 0;

    for(INT8U i=0;i<_groupNum;i++) {
        if(is_activated(i)) {
            num += _group[i].cardNum;
        }
    }

    return num;
}

int  Alternatives::group_num() const {
    return _groupNum;
}

int  Alternatives::cards_num(int gIdx) const {
    return _group[gIdx].cardNum;
}

int  Alternatives::get_card_idx(int gIdx,int cIdx) const {
    return _group[gIdx].idx[cIdx];
}

CardStatus_t Alternatives::get_status(int gIdx) const {
    return _cards->get_status(get_card_idx(gIdx,0));
}

Card_t Alternatives::GetKind(int gIdx) const {
    return _cards->get_kind(get_card_idx(gIdx,0));
}

void Alternatives::SetStatus(int gIdx,CardStatus_t status) const {
    for(int i=0;i<_group[gIdx].cardNum;i++) {
        _cards->set_status(get_card_idx(gIdx,i),status);
    }
}

void Alternatives::AddGroup(int num,int *idx,CardStatus_t activeStatus,CardStatus_t freeStatus) {
    _group[_groupNum].ACTIVE_STATUS = activeStatus;
    _group[_groupNum].FREE_STATUS   = freeStatus;
    _group[_groupNum].cardNum = num;

    for(int i=0;i<num;i++) {
        _group[_groupNum].idx[i] = idx[i];
    }

    _group[_groupNum].BACK_STATUS   = _cards->get_status(_group[_groupNum].idx[0]);
    SetStatus(_groupNum,_group[_groupNum].FREE_STATUS);

    _groupNum++;
}


bool Alternatives::IsInclude(Card_t kind) const {
    for(INT8U i=0;i<_groupNum;i++) {
        if(GetKind(i)==kind) {
            return true;
        }
    }

    return false;
}

void Alternatives::switch_status(int gIdx) {
    if(_action==aGANG) {
        for(int i=0;i<_groupNum;i++) {
            if(is_activated(i)) {
                SetStatus(i,_group[i].FREE_STATUS);
            }
        }
        SetStatus(gIdx,_group[gIdx].ACTIVE_STATUS);
    } else {
        if(get_status((gIdx))==_group[gIdx].ACTIVE_STATUS) {
            SetStatus(gIdx,_group[gIdx].FREE_STATUS);
        } else {
            SetStatus(gIdx,_group[gIdx].ACTIVE_STATUS);
        }
    }
}

void Alternatives::refresh() {/* ONLY USED IN aKOU */
    if(_action==aKOU) {
        for( int i=0; i<group_num(); i++ ) {
            if(get_status(i)!=_group[i].ACTIVE_STATUS) {
                if( _cards->can_kou(GetKind(i)) ) {
                    SetStatus(i,_group[i].FREE_STATUS);
                } else {
                    SetStatus(i,sFREE);
                }
            }
        }
    }
}

void Alternatives::active_all(Card_t handingout) {
    scan_kou(handingout);

    for(INT8U i=0;i<group_num();i++) {
        if(_cards->can_kou(GetKind(i),handingout)) {
            SetStatus(i,_group[i].ACTIVE_STATUS);
        }
    }
}

void Alternatives::clear() {
    for(INT8U i=0;i<_groupNum;i++) {
        SetStatus(i,_group[i].BACK_STATUS);
    }

    _groupNum = 0;
}

int Alternatives::get_activated_kinds(Card_t kinds[]) const {
    int num = 0;
    
    for(INT8U i=0;i<_groupNum;i++) {
        if(get_status(i)==_group[i].ACTIVE_STATUS) {
            kinds[num++] = GetKind(i);    
        }
    }

    return num;
}

/* this function is used to find gang cards and gang type */
Card_t Alternatives::get_activated_cards(int idx[],ActionId_t *action) const {
    int activated = 0;

    for(int i=0;i<_groupNum;i++) {
        if(is_activated(i)) {
            activated = i;
            break;
        }
    }
    
    for(int i=0;i<_group[activated].cardNum;i++) {
        idx[i] = _group[activated].idx[i];    
    }

    if(action!=NULL) {
        if(_group[activated].ACTIVE_STATUS==sAN_GANG) {
            *action = aAN_GANG;
        } else if(_group[activated].ACTIVE_STATUS==sMING_GANG) {
            *action = aMING_GANG;
        } else if(_group[activated].ACTIVE_STATUS==sSHOU_GANG){
            *action = aSHOU_GANG;
        }
    }

    return GetKind(activated);
}

ActionMask_t Alternatives::get_actions() const {
    ActionMask_t actions = 0;
    
    for(int i=0;i<group_num();i++) {
        actions |= _group[i].ACTIVE_STATUS;
    }

    return actions;
}
