
#include "CardCollection.h"
#include "Player.h"
#include "StrategyPlayer.h"
#include "RoundManager.h"
#include "NetRoundManager.h"

#define RETURN_IF_VALID(x) do { \
    int chosen = x;             \
                                \
    if(chosen!=INVALID)         \
        return chosen;          \
}while(0)

StrategyPlayer::StrategyPlayer(Player *aPlayer) {
    _employer = aPlayer;
	_rm = RoundManager::getInstance();
}

void StrategyPlayer::assign(RoundManager *rm) {
    _rm = rm;
}

/******************************************************************
    算分
******************************************************************/
void StrategyPlayer::SetScoreContext() {
    _scoreCtx.isNewDistributed = _rm->_isNewDistributed;
    _scoreCtx.isLastOne        = (_rm->_distributedNum==TOTAL_CARD_NUM);
    _scoreCtx.action           = _rm->_lastActionWithGold;
    _scoreCtx.continueGang     = _rm->_continue_gang_times;
    _scoreCtx.isGangHua        = _rm->_isGangHua;
}

long StrategyPlayer::CalcScore(Card_t kind,const ScoreContext_t &ctx) {
    _employer->_fan = 0;

    _employer->_cards->update_statistics(kind);
    _employer->_fan = _employer->_cards->statHuFanMask;
    
    if(ctx.isLastOne) {
		_employer->_fan |= RH_HAIDILAO;
    }

	if(ctx.isNewDistributed) {
		_employer->_fan |= RH_ZIMO;
	}

	if((ctx.continueGang!=0)
        &&(ctx.action==aMING_GANG||ctx.action==aAN_GANG||ctx.action==aQIANG_GANG)) {
		if(ctx.isNewDistributed&&ctx.action==aQIANG_GANG)
			_employer->_fan |= RH_QIANGGANG;
		else if(ctx.isNewDistributed&&ctx.isGangHua)
			_employer->_fan |= RH_GANGHUA;
		else if(!ctx.isNewDistributed)
			_employer->_fan |= RH_GANGPAO;
	}

	if(_employer->_AIM!=RA_NOTASK)
		check_task(_employer->_fan);
    
	return _employer->_cards->sum_up_score(_employer->_fan);
}

long StrategyPlayer::calc_score(Card_t kind) {
    SetScoreContext();
    return CalcScore(kind,_scoreCtx);
}

void StrategyPlayer::check_task(unsigned int flag) {
	if( !(flag&_employer->_AIM) ) {
		_employer->_aimDone=0;
    }
}

bool StrategyPlayer::scan_ming(const CardList *river) {
    if(_rm->_MODE==LOCAL_GAME) {
        return _employer->_cards->scan_ming(river);
    } else {
        return (_employer->_cards->_ming.choiceNum>0);
    }
}

void StrategyPlayer::scan_gang(bool isNewDistributed) {
    if(_rm->_MODE==LOCAL_GAME) {
        _employer->_cards->_alter->scan_gang(isNewDistributed);
    } else {
        Reminds_t &remind = static_cast<NetRoundManager *>(_rm)->_serverReminds;
        _employer->_cards->_alter->load_gang_info(remind.actions,remind.gangCard,remind.gangKindNum);
    }
}

void StrategyPlayer::scan_kou(Card_t handingout) {
    if(_rm->_MODE==LOCAL_GAME) {
        _employer->_cards->_alter->scan_kou(handingout);
    } else {
        Reminds_t &remind = static_cast<NetRoundManager *>(_rm)->_serverReminds;
        _employer->_cards->_alter->load_kou_info(remind.kouCard,remind.kouKindNum);
    }
}
/******************************************************************
    选择要出的牌                       
******************************************************************/
void StrategyPlayer::SetChooseContext() {
    memset(&_chooseCtx,0,sizeof(ChooseContext_t));

    _chooseCtx.river  = _rm->_gRiver;
    _chooseCtx.remain = TOTAL_CARD_NUM - _rm->_distributedNum;
    _chooseCtx.aim    = _employer->_cards->assess_aim();

    int curPlayer = _rm->_curPlayer;
    _chooseCtx.OthersTing[0] = _rm->_players[(curPlayer+1)%3]->_cards->_ting;
    _chooseCtx.OthersTing[1] = _rm->_players[(curPlayer+2)%3]->_cards->_ting;

    _CollectPosition(_chooseCtx.cards);
    
    if(_employer->_cards->_ting) {
        _chooseCtx.huNum = _employer->_cards->_ting->cardNum;
    } else {
        _chooseCtx.huNum = 0;
    }
}

int StrategyPlayer::ChooseForMing(ActionId_t &ming,bool &canKou) {
    Card_t handingout = CARD_UNKNOWN;
    int    minum  = 0;

    if(_employer->_strategy->scan_ming(_chooseCtx.river)) {
        for(int i=0;i<_employer->_cards->_ming.choiceNum;i++) {
            MingChoice_t  *choice = _employer->_cards->_ming.handouts+i;

            if(!OthersCanHu(choice->kind)) {
                int num = _employer->_cards->get_ting_num(choice->kind);

                if(minum > num) {
                    handingout = choice->kind;
                    minum = num;
                }
            }
        }
    }

    for(int i=0;i<_employer->_cards->size();i++) {
        if(_employer->_cards->get_kind(i)==handingout) {
            _employer->_cards->set_ming(i);
            _chooseCtx.huNum = _employer->_cards->_ting->cardNum;
            if(_chooseCtx.huNum>=6) {
                canKou = true;
            }            

            return i;
        }
    }
    
    return INVALID;
}

int StrategyPlayer::ChooseCard() {
    /*******************************************/
    HAH *s_res = new HAH;
    
    CARD_KIND list1[9];
    CARD_KIND list2[9];
    int len1;
    int len2;
    
    memset(s_res,0,sizeof(HAH));
    memset(s_res->card_in_river,ck_NOT_DEFINED,sizeof(CARD_KIND)*TOTAL_CARD_NUM);
    
    s_res->reserved_card_num = TOTAL_CARD_NUM - _rm->_distributedNum;
    
    CARD s_card;
    
    for(int i=0;i<_rm->_gRiver->size();i++) {
        s_res->card_in_river[s_res->river_len++] = (CARD_KIND)_rm->_gRiver->get_kind(i);
    }
    
    int curPlayer = _rm->_curPlayer;
    _rm->_players[(curPlayer+1)%3]->_cards->get_hu_cards(list1,&len1);
    _rm->_players[(curPlayer+2)%3]->_cards->get_hu_cards(list2,&len2);
    
    for(int i=_employer->_cards->FreeStart;i<_employer->_cards->size();i++) {
        int time = s_res->list[_employer->_cards->get_kind(i)].same_times++;
        s_res->list[_employer->_cards->get_kind(i)].place[time]=i;
    }
    
    s_res->target = (ROBOT_TARGET)_employer->_cards->assess_aim();
    
    int index = Robot_pickup_single(s_res,list1,list2,len1,len2);
    if( index==INVALID ) {
        index = _employer->_cards->last();
    }
    
    return index;
}

int StrategyPlayer::choose_card(ActionId_t &ming,bool &canKou) {
    if(_employer->_cards->IsMing) {
        return _employer->_cards->last();
    } else {
        SetChooseContext();

        int index = INVALID;
        if(ming==aMING) {
            index = ChooseForMing(ming,canKou);
        }

        if(index!=INVALID) {
            return index;
        } else {
            ming=aQi;
            return ChooseCard();
        }
    }
}

bool StrategyPlayer::OthersCanHu(Card_t kind) const {/*the efficiency could be optimized*/
    for(int i=0;i<2;i++) {
        if(_chooseCtx.OthersTing[i]==NULL) {
            continue;
        }
        
        for(int j=0;j<_chooseCtx.OthersTing[i]->kindNum;j++) {
            if(kind==(_chooseCtx.OthersTing[i]->cards+j)->kind) {
                return true;
            }
        }
    }

    return false;
}

int StrategyPlayer::AvailNum(Card_t kind) const {
    int num = _chooseCtx.river->find_cards(kind);

    if(_chooseCtx.remain<10 && num==0) {
        return 4;
        return 3;/*WHY???*/
    } else {
        return (4-num);
    }
}

bool StrategyPlayer::IsInStrictSequences(Card_t kind,int seqIdx) const {
    int num0 = _chooseCtx.cards[kind].num;
    int num1 = 0;
    int num2 = 0;

    switch(seqIdx) {
        case 0:
            num1 = _chooseCtx.cards[kind+1].num;
            num2 = _chooseCtx.cards[kind+2].num;
            break;
        case 1:
            num1 = _chooseCtx.cards[kind-1].num;
            num2 = _chooseCtx.cards[kind+1].num;
            break;
        case 2:
            num1 = _chooseCtx.cards[kind-2].num;
            num2 = _chooseCtx.cards[kind-1].num;
            break;
    }

    if(num0==num1 && num0==num2) {
        return true;
    } else if(num0==num1 || num0==num2) {/*BUG HERE??? Robot_check_card_stable 4th if*/
        return true;
    } else {
        return false;
    }
}

bool StrategyPlayer::IsInSequences(Card_t kind) const {
    if(kind%9==0) {
        return IsInStrictSequences(kind,0);
    } else if(kind%9==8) {
        return IsInStrictSequences(kind,2);
    } else {
        for(int seqIdx=0;seqIdx<3;seqIdx++) {
            if(IsInStrictSequences(kind,seqIdx)) {
                return true;
            } else {
                return false;
            }
        }
    }
}

bool StrategyPlayer::IsStable(Card_t kind) const {
    int num = _chooseCtx.cards[kind].num;
    
    if( num>=3 ) {
        return true;
    } else if (num>=1 && IsInSequences(kind)) {
        return true;
    }

    return false;
}

int StrategyPlayer::Robot_check_pickup_card(CARD_KIND kind,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
    int j,k;
    for(j=0;j<len1;j++)
        if(list1[j]==kind)
            break;
    for(k=0;k<len2;k++)
        if(list2[k]==kind)
            break;
    if(j==len1&&k==len2)
        return 0;
    
    return -1;
}

int StrategyPlayer::river_reserved_card(HAH *hash_table,int card)
{
    int num=0;
    for(int i=0;i<hash_table->river_len;i++)
        if(hash_table->card_in_river[i]==card)
        {
            num++;          
        }
        
    if(hash_table->reserved_card_num<10&&num==0)
        num=3;
    
    return (4-num);
}

int StrategyPlayer::Robot_check_card_stable(HAH *card_array,CARD_KIND card)
{
    if(card_array->list[card].same_times==1||card_array->list[card].same_times==2)
    {
        if(card%9==0)
        {
            if(card_array->list[card+1].same_times==card_array->list[card].same_times
            &&card_array->list[card+2].same_times==card_array->list[card].same_times)
                return 0;
        }
        else if(card%9==8)
        {
            if(card_array->list[card-1].same_times==card_array->list[card].same_times&&
            card_array->list[card-2].same_times==card_array->list[card].same_times)
                return 0;
        }
        else if(card_array->list[card+1].same_times==card_array->list[card].same_times
            &&card_array->list[card+2].same_times==card_array->list[card].same_times||
            card_array->list[card-1].same_times==card_array->list[card].same_times&&
            card_array->list[card-2].same_times==card_array->list[card].same_times||
            card_array->list[card-1].same_times==card_array->list[card].same_times&&
            card_array->list[card+1].same_times==card_array->list[card].same_times)
                return 0;
        else
        {
            if(card_array->list[card+1].same_times==card_array->list[card].same_times
            &&card_array->list[card+2].same_times!=card_array->list[card].same_times
            ||card_array->list[card].same_times==card_array->list[card+2].same_times
            &&card_array->list[card+1].same_times!=card_array->list[card].same_times
            ||card_array->list[card+1].same_times==card_array->list[card].same_times
            &&card_array->list[card-1].same_times!=card_array->list[card].same_times)
                return 0;
        }
    }
    else if(card_array->list[card].same_times==3||card_array->list[card].same_times==4)
        return 0;
    return -1;
}

int StrategyPlayer::_FindSingleChar() {
    for(int i=ZHONG;i<=BAI;i++) {
        if(!OthersCanHu((Card_t)i) && _chooseCtx.cards[i].num==1) {
            return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
        }
    }

    return INVALID;
}

/* 只有一个，而且不连续 */
int StrategyPlayer::_FindSingleAndNonSequence(Card_t HeadKind,Card_t TailKind) {
    if(!OthersCanHu(HeadKind) && _chooseCtx.cards[HeadKind].num==1 && _chooseCtx.cards[HeadKind].num==0) {
        return _chooseCtx.cards[HeadKind].position[_chooseCtx.cards[HeadKind].num-1];
    } else if(!OthersCanHu(TailKind) && _chooseCtx.cards[TailKind].num==1 && _chooseCtx.cards[TailKind].num==0) {
        return _chooseCtx.cards[TailKind].position[_chooseCtx.cards[TailKind].num-1];
    } else {
        for(int i=HeadKind+1;i<TailKind;i++) {
            if(!OthersCanHu((Card_t)i)) {
                if(_chooseCtx.cards[i].num==1 && _chooseCtx.cards[i-1].num==0 && _chooseCtx.cards[i+1].num==0) {
                    return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
                }
            }
        }
    }

    return INVALID;
}

int StrategyPlayer::_FindSingleAndUnstable(Card_t HeadKind,Card_t TailKind) {
    if(!OthersCanHu(HeadKind) || !OthersCanHu(TailKind)) {
        if(_chooseCtx.cards[HeadKind].num==1 && !IsStable(HeadKind)) {
            return _chooseCtx.cards[HeadKind].position[_chooseCtx.cards[HeadKind].num-1];
        }
        if(_chooseCtx.cards[TailKind].num==1 && !IsStable(TailKind)) {
            return _chooseCtx.cards[TailKind].position[_chooseCtx.cards[TailKind].num-1];
        }
    }

    for(int i=HeadKind;i<TailKind;i++) {
        if(!OthersCanHu((Card_t)i)) {
            if(_chooseCtx.cards[i].num==1 && !IsStable((Card_t)i)) {
                return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
            }
        }
    }

    return INVALID;
}

int StrategyPlayer::PickupForSameColor(int reserveColor) {
    const Card_t RiverLast    = _chooseCtx.river->get_kind(_chooseCtx.river->last());
    const Card_t River2ndLast = _chooseCtx.river->get_kind(_chooseCtx.river->last()-1);
    const KindPosition &Card1 = _chooseCtx.cards[RiverLast];
    const KindPosition &Card2 = _chooseCtx.cards[River2ndLast];

    const int Hu1 = _chooseCtx.OthersTing[0]->kindNum;
    const int Hu2 = _chooseCtx.OthersTing[1]->kindNum;
    
    Card_t HeadKind = (Card_t)((1-reserveColor)*9);
    Card_t TailKind = (Card_t)(HeadKind+8);

    if(Hu1==0&&Hu2==0&&_chooseCtx.remain>38) {
        for(int i=HeadKind;i<=TailKind;i++) {
            if(_chooseCtx.cards[i].num>0 && !IsStable((Card_t)i)) {
                return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
            }
        }
    }

    /*跟别人出*/
    if(!OthersCanHu(RiverLast) || !OthersCanHu(River2ndLast)) {
        if(River2ndLast/9==(1-reserveColor)) {
            if(Card1.num>0 && !IsStable(RiverLast)) {
                return _chooseCtx.cards[RiverLast].position[_chooseCtx.cards[RiverLast].num-1];
            } else if(Card2.num>0 && !IsStable(River2ndLast)) {
                return _chooseCtx.cards[River2ndLast].position[_chooseCtx.cards[River2ndLast].num-1];
            }
        }
    }

    RETURN_IF_VALID(_FindSingleChar());
    RETURN_IF_VALID(_FindSingleAndNonSequence(HeadKind,TailKind));
    RETURN_IF_VALID(_FindSingleAndUnstable(HeadKind,TailKind));

    for(int i=HeadKind;i<=TailKind;i++) {
        if(_chooseCtx.cards[i].num>0 && !IsStable((Card_t)i) && !OthersCanHu((Card_t)i)) {
            return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
        }
    }
    
    for(int i=ZHONG;i<ZHONG+3;i++) {
        if(_chooseCtx.cards[i].num>0 && !IsStable((Card_t)i) && !OthersCanHu((Card_t)i)) {
            return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
        }
    }

    for(int i=HeadKind;i<=TailKind;i++) {
        if(_chooseCtx.cards[i].num>0 && !OthersCanHu((Card_t)i)) {
            return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
        }
    }

    for(int i=ZHONG;i<ZHONG+3;i++) {
        if(_chooseCtx.cards[i].num>0 && !OthersCanHu((Card_t)i)) {
            return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
        }
    }

    /*同一种花色中选*/
    HeadKind = (Card_t)(reserveColor*9);
    TailKind = (Card_t)(HeadKind+8);

    RETURN_IF_VALID(_FindSingleAndNonSequence(HeadKind,TailKind));

    if(!OthersCanHu(HeadKind) && _chooseCtx.cards[HeadKind].num==1) {
        return _chooseCtx.cards[HeadKind].position[_chooseCtx.cards[HeadKind].num-1];
    } else if(!OthersCanHu(TailKind) && _chooseCtx.cards[TailKind].num==1) {
        return _chooseCtx.cards[TailKind].position[_chooseCtx.cards[TailKind].num-1];
    }
    
    for(int i=HeadKind;i<=TailKind;i++) {
        if(_chooseCtx.cards[i].num>0 && !IsStable((Card_t)i) && !OthersCanHu((Card_t)i)) {
            return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
        }
    }

    for(int i=HeadKind+1;i<TailKind;i++) {
        if(!OthersCanHu((Card_t)i)) {
            if(_chooseCtx.cards[i].num==1 && _chooseCtx.cards[i-1].num==0 && _chooseCtx.cards[i+1].num==0) {
                return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
            }
        }
    }

    for(int i=HeadKind;i<=TailKind;i++) {
        if(!OthersCanHu((Card_t)i) && _chooseCtx.cards[i].num>0 && _chooseCtx.cards[i].num<3) {
            return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
        }
    }

    return INVALID;
}
    
int StrategyPlayer::PickupForSevenCouples() {
    const Card_t RiverLast    = _chooseCtx.river->get_kind(_chooseCtx.river->last());
    const Card_t River2ndLast = _chooseCtx.river->get_kind(_chooseCtx.river->last()-1);
    const KindPosition &Card1 = _chooseCtx.cards[RiverLast];
    const KindPosition &Card2 = _chooseCtx.cards[River2ndLast];

    const int Hu1 = _chooseCtx.OthersTing[0]->kindNum;
    const int Hu2 = _chooseCtx.OthersTing[1]->kindNum;
    
    if(_chooseCtx.cards[RiverLast].num!=2 && _chooseCtx.cards[RiverLast].num!=4
        && !OthersCanHu(RiverLast)) {
        
        return _chooseCtx.cards[RiverLast].position[_chooseCtx.cards[RiverLast].num-1];
        
    } else if(_chooseCtx.cards[River2ndLast].num!=2 && _chooseCtx.cards[River2ndLast].num!=4
        && !IsStable(River2ndLast)) {/*BUG HERE??? 没有判断是否会点炮*/
        
        return _chooseCtx.cards[River2ndLast].position[_chooseCtx.cards[River2ndLast].num-1];
        
    }

    RETURN_IF_VALID(_FindSingleChar());

    for(int i=0;i<TOTAL_CARD_KIND;i++) {
        if(!OthersCanHu((Card_t)i) && _chooseCtx.cards[i].num==3) {
            return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
        }
    }

    for(int avail=0;avail<=4;avail--) {
        for(int i=0;i<TOTAL_CARD_KIND;i++) {
            if(!OthersCanHu((Card_t)i)  && _chooseCtx.cards[i].num==1 && AvailNum((Card_t)i)==avail) {
                return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
            }
        }
    }

    return INVALID;
}

int StrategyPlayer::PickupForFourPeng() {
    const Card_t RiverLast    = _chooseCtx.river->get_kind(_chooseCtx.river->last());
    const Card_t River2ndLast = _chooseCtx.river->get_kind(_chooseCtx.river->last()-1);
    const KindPosition &Card1 = _chooseCtx.cards[RiverLast];
    const KindPosition &Card2 = _chooseCtx.cards[River2ndLast];

    const int Hu1 = _chooseCtx.OthersTing[0]->kindNum;
    const int Hu2 = _chooseCtx.OthersTing[1]->kindNum;
    
    if(_chooseCtx.cards[RiverLast].num==1 && !IsStable(RiverLast) && !OthersCanHu(RiverLast)) {
        return _chooseCtx.cards[RiverLast].position[_chooseCtx.cards[RiverLast].num-1];
    } else if(_chooseCtx.cards[River2ndLast].num==1 && !IsStable(RiverLast) && !OthersCanHu(RiverLast)) {
        return _chooseCtx.cards[River2ndLast].position[_chooseCtx.cards[River2ndLast].num-1];
    }

    RETURN_IF_VALID(_FindSingleChar());

    for(int cardInHand=1;cardInHand<3;cardInHand++) {
        for(int avail=0;avail<=4;avail++) {
            for(int i=0;i<TOTAL_CARD_KIND;i++) {/*LOGIC CHANGED!!!*/
                if(!OthersCanHu((Card_t)i)) {
                    if(_chooseCtx.cards[i].num==cardInHand && AvailNum((Card_t)i)==avail) {
                        return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
                    }
                }
            }
        }
    }

    return INVALID;
}

int StrategyPlayer::PickupForPiHu() {
    const Card_t RiverLast    = _chooseCtx.river->get_kind(_chooseCtx.river->last());
    const Card_t River2ndLast = _chooseCtx.river->get_kind(_chooseCtx.river->last()-1);
    const KindPosition &Card1 = _chooseCtx.cards[RiverLast];
    const KindPosition &Card2 = _chooseCtx.cards[River2ndLast];

    const int Hu1 = _chooseCtx.OthersTing[0]->kindNum;
    const int Hu2 = _chooseCtx.OthersTing[1]->kindNum;
    
    if(_chooseCtx.cards[RiverLast].num==1 && !IsStable(RiverLast) && !OthersCanHu(RiverLast)) {
        return _chooseCtx.cards[RiverLast].position[_chooseCtx.cards[RiverLast].num-1];
    } else if(_chooseCtx.cards[River2ndLast].num==1 && !IsStable(RiverLast) && !OthersCanHu(RiverLast)) {
        return _chooseCtx.cards[River2ndLast].position[_chooseCtx.cards[River2ndLast].num-1];
    }

    RETURN_IF_VALID(_FindSingleChar());

    for(int cardInHand=1;cardInHand<3;cardInHand++) {
        for(int avail=0;avail<4;avail++) {
                for(int i=0;i<TOTAL_CARD_KIND;i++) {
                    if(_chooseCtx.cards[i].num==cardInHand && !IsStable((Card_t)i) && !OthersCanHu((Card_t)i) && AvailNum((Card_t)i)==avail) {
                        return _chooseCtx.cards[i].position[_chooseCtx.cards[i].num-1];
                    }
            }
        }
    }

    return INVALID;
}

int StrategyPlayer::Robot_picup_single_for_samecolor(int color,HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
    int chose_place=-1;
    int offset=color*9;
    int index1=card_array->river_len-1;
    int index2=index1-1;
    CARD_KIND kind1=card_array->card_in_river[index1];
    CARD_KIND kind2=card_array->card_in_river[index2];
    HA card1=card_array->list[kind1];
    HA card2=card_array->list[kind2];

    if(color!=2)
    {
        if(len1==0&&len2==0&&card_array->reserved_card_num>38)
        {
            for(int i=offset;i<offset+9;i++)
                if(Robot_check_card_stable(card_array,(CARD_KIND)i)!=0)
                {
                    if(card_array->list[i].same_times!=0)
                    {
                        chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                        break;
                    }
                }
        }
        if(len1!=0||len2!=0)
        {
            if(Robot_check_pickup_card((CARD_KIND)(kind1),list1,list2,len1,len2)==0||Robot_check_pickup_card((CARD_KIND)(kind2),list1,list2,len1,len2)==0)
            {
                if(kind2/9==color&&card1.same_times!=0&&Robot_check_card_stable(card_array,kind1)!=0)
                {
                    chose_place=card_array->list[kind1].place[card1.same_times-1];
                }
                else if(kind2/9==color&&card2.same_times!=0&&Robot_check_card_stable(card_array,kind2)!=0)
                {
                    chose_place=card_array->list[kind2].place[card2.same_times-1];
                }
            }
        }
        if(chose_place==-1)
        {
            for(int i=18;i<21;i++)
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&card_array->list[i].same_times==1)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
        }
        if(chose_place==-1)
        {
            if(Robot_check_pickup_card((CARD_KIND)(offset),list1,list2,len1,len2)==0||Robot_check_pickup_card((CARD_KIND)(offset+8),list1,list2,len1,len2)==0)
            {
                if(card_array->list[offset].same_times==1&&card_array->list[offset+1].same_times==0)
                    chose_place=card_array->list[offset].place[card_array->list[offset].same_times-1];
                else if(card_array->list[offset+8].same_times==1&&card_array->list[offset+7].same_times==0)
                    chose_place=card_array->list[offset+8].place[card_array->list[offset+8].same_times-1];
            }
            else
            {   
                for(int i=offset+1;i<8+offset;i++)
                {
                    if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0)
                        if(card_array->list[i].same_times==1&&card_array->list[i-1].same_times==0
                            &&card_array->list[i+1].same_times==0)
                        {
                            chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                            break;
                        }           
                }
            }
            if(chose_place==-1)
            {
                if(Robot_check_pickup_card((CARD_KIND)(offset),list1,list2,len1,len2)==0||Robot_check_pickup_card((CARD_KIND)(offset+8),list1,list2,len1,len2)==0)
                {
                    if(card_array->list[offset].same_times==1&&Robot_check_card_stable(card_array,(CARD_KIND)offset)!=0)
                        chose_place=card_array->list[offset].place[card_array->list[offset].same_times-1];
                    else if(card_array->list[offset+8].same_times==1&&Robot_check_card_stable(card_array,(CARD_KIND)(offset+8))!=0)
                        chose_place=card_array->list[offset+8].place[card_array->list[offset+8].same_times-1];
                }
            }
            if(chose_place==-1)
            {
                for(int i=offset+1;i<8+offset;i++)
                {
                    if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0)
                        if(card_array->list[i].same_times==1&&Robot_check_card_stable(card_array,(CARD_KIND)i)!=0)
                        {
                                chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                                break;
                        }
                }
            }
            if(chose_place==-1)
            {
                for(int i=offset;i<offset+9;i++)
                    if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                        &&card_array->list[i].same_times!=0&&Robot_check_card_stable(card_array,(CARD_KIND)i)!=0)
                        {
                            chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                            break;
                        }
            }
            if(chose_place==-1)
            {
                for(int i=18;i<21;i++)
                    if(card_array->list[i].same_times!=0&&Robot_check_card_stable(card_array,(CARD_KIND)i)!=0&&
                        Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0)
                    {
                        chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                        break;
                    }
            }
            if(chose_place==-1)
            {
                for(int i=offset;i<offset+9;i++)
                    if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                        &&card_array->list[i].same_times!=0)
                        {
                            chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                            break;
                        }
            }
            if(chose_place==-1)
            {
                for(int i=18;i<21;i++)
                    if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                        &&card_array->list[i].same_times!=0)
                    {
                        chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                        break;
                    }
            }
            if(chose_place==-1)
            {
                offset=(1-color)*9;
                if(Robot_check_pickup_card((CARD_KIND)(offset),list1,list2,len1,len2)==0
                    &&card_array->list[offset].same_times==1&&card_array->list[1+offset].same_times==0)
                        chose_place=card_array->list[offset].place[card_array->list[offset].same_times-1];
                else if(Robot_check_pickup_card((CARD_KIND)(offset+8),list1,list2,len1,len2)==0
                    &&card_array->list[offset+8].same_times==1&&card_array->list[offset+7].same_times==0)
                        chose_place=card_array->list[offset+8].place[card_array->list[offset+8].same_times-1];
                else
                {   
                    for(int i=offset+1;i<8+offset;i++)
                    {
                        if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0)
                            if(card_array->list[i].same_times==1&&card_array->list[i-1].same_times==0
                                &&card_array->list[i+1].same_times==0)
                            {
                                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];;
                                    break;
                            }           
                    }
                }
                if(chose_place==-1)
                {
                        if(Robot_check_pickup_card((CARD_KIND)(offset),list1,list2,len1,len2)==0&&
                            card_array->list[offset].same_times==1)
                                chose_place=card_array->list[offset].place[card_array->list[offset].same_times-1];
                        else if(Robot_check_pickup_card((CARD_KIND)(offset+8),list1,list2,len1,len2)==0&&
                            card_array->list[offset+8].same_times==1)
                                chose_place=card_array->list[offset+8].place[card_array->list[offset+8].same_times-1];
                }
                if(chose_place==-1)
                {
                    for(int j=offset;j<offset+9;j++)
                        if(Robot_check_pickup_card(CARD_KIND(j),list1,list2,len1,len2)==0&&
                            card_array->list[j].same_times!=0&&Robot_check_card_stable(card_array,CARD_KIND(j))!=0)
                        {   
                            chose_place=card_array->list[j].place[card_array->list[j].same_times-1];
                            break;
                        }
                }
                if(chose_place==-1)
                {
                    for(int j=offset+1;j<offset+8;j++)
                        if(Robot_check_pickup_card(CARD_KIND(j),list1,list2,len1,len2)==0&&
                            (card_array->list[j].same_times==1&&card_array->list[j-1].same_times==0)||
                            (card_array->list[j].same_times==1&&card_array->list[j+1].same_times==0))
                        {   
                            chose_place=card_array->list[j].place[card_array->list[j].same_times-1];
                            break;
                        }
                }
                if(chose_place==-1)
                {
                    for(int j=offset;j<offset+9;j++)
                        if(Robot_check_pickup_card(CARD_KIND(j),list1,list2,len1,len2)==0&&
                            card_array->list[j].same_times!=3&&card_array->list[j].same_times!=4)
                        {   
                            chose_place=card_array->list[j].place[card_array->list[j].same_times-1];
                            break;
                        }
                }
            }
        }
    }
    return chose_place;
}

int StrategyPlayer::Robot_pickup_for_sevencouples(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{   
    int chose_place=-1;

    int index1=card_array->river_len-1;
    int index2=index1-1;
    CARD_KIND kind1=card_array->card_in_river[index1];
    CARD_KIND kind2=card_array->card_in_river[index2];
    HA card1=card_array->list[kind1];
    HA card2=card_array->list[kind2];

    if(len1!=0||len2!=0)
    {
        if(Robot_check_pickup_card((CARD_KIND)(kind1),list1,list2,len1,len2)==0&&
            card1.same_times!=2&&card1.same_times!=4)
        {
            chose_place=card_array->list[kind1].place[card1.same_times-1];
        }
        else if(Robot_check_card_stable(card_array,kind2)!=0&&
            card2.same_times!=2&&card2.same_times!=4)
        {
            chose_place=card_array->list[kind2].place[card2.same_times-1];
        }
    }
    if(chose_place==-1)
    {
        for(int i=18;i<21;i++)
            if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&card_array->list[i].same_times==1)
            {
                chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                break;
            }
    }
    if(chose_place==-1)
    {
        for(int i=0;i<TOTAL_CARD_KIND;i++)
            if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                &&card_array->list[i].same_times==3)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
    }
    if(chose_place==-1)
    {
        for(int i=0;i<TOTAL_CARD_KIND;i++)
            if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&
                card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==1)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
    }
    if(chose_place==-1)
    {
        for(int i=0;i<TOTAL_CARD_KIND;i++)
            if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                &&card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==2)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
    }
    if(chose_place==-1)
    {
        for(int i=0;i<TOTAL_CARD_KIND;i++)
            if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&
                card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==3)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
    }
    if(chose_place==-1)
    {
        for(int i=0;i<TOTAL_CARD_KIND;i++)
            if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                &&card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==4)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
    }
    return chose_place;
}

int StrategyPlayer::Robot_pickup_for_fourpeng(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
    int chose_place=-1;
    
    int index1=card_array->river_len-1;
    int index2=index1-1;
    CARD_KIND kind1=card_array->card_in_river[index1];
    CARD_KIND kind2=card_array->card_in_river[index2];
    HA card1=card_array->list[kind1];
    HA card2=card_array->list[kind2];

    if(len1!=0||len2!=0)
    {
        if(Robot_check_pickup_card((CARD_KIND)(kind1),list1,list2,len1,len2)==0
            &&card1.same_times==1&&Robot_check_card_stable(card_array,kind1)!=0)
        {
            chose_place=card_array->list[kind1].place[card1.same_times-1];
        }
        else if(card2.same_times==1&&Robot_check_card_stable(card_array,kind2)!=0
            &&Robot_check_pickup_card((CARD_KIND)(kind2),list1,list2,len1,len2)==0)
        {
            chose_place=card_array->list[kind2].place[card2.same_times-1];
        }
    }
    if(chose_place==-1)
    {
        for(int i=18;i<21;i++)
            if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&card_array->list[i].same_times==1)
            {
                chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                break;
            }
    }
    if(chose_place==-1)
    {
        for(int i=0;i<TOTAL_CARD_KIND;i++)
            {
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                    &&card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==1)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
            }
        if(chose_place==-1)
        {
            for(int i=0;i<TOTAL_CARD_KIND;i++)
            {
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                    &&card_array->list[i].same_times==2&&river_reserved_card(card_array,i)==2)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
            }
        }
        if(chose_place==-1)
        {
            for(int i=0;i<TOTAL_CARD_KIND;i++)
            {
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                    &&card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==2)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
            }
        }
        if(chose_place==-1)
        {
            for(int i=0;i<TOTAL_CARD_KIND;i++)
            {
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                    &&card_array->list[i].same_times==2&&river_reserved_card(card_array,i)==3)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
            }   
        }
        if(chose_place==-1)
        {
            for(int i=0;i<TOTAL_CARD_KIND;i++)
            {
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&
                    card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==3)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
            }
        }
        if(chose_place==-1)
        {
            for(int i=0;i<TOTAL_CARD_KIND;i++)
            {
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&
                    card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==4)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
            }
        }
        if(chose_place==-1)
        {
            for(int i=0;i<TOTAL_CARD_KIND;i++)
            {
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                    &&card_array->list[i].same_times==2&&river_reserved_card(card_array,i)==4)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
            }
        }
    }
    return chose_place;
}

int StrategyPlayer::Robot_pickup_for_lowwin(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
    int chose_place=-1;
    int index1=card_array->river_len-1;
    int index2=index1-1;
    CARD_KIND kind1=card_array->card_in_river[index1];
    CARD_KIND kind2=card_array->card_in_river[index2];
    HA card1=card_array->list[kind1];
    HA card2=card_array->list[kind2];

    if(len1!=0||len2!=0)
    {
        if(Robot_check_pickup_card((CARD_KIND)(kind1),list1,list2,len1,len2)==0
            &&card1.same_times==1&&Robot_check_card_stable(card_array,kind1)!=0)
        {
            chose_place=card_array->list[kind1].place[card1.same_times-1];
        }
        else if(card2.same_times==1&&Robot_check_card_stable(card_array,kind2)!=0
            &&Robot_check_pickup_card((CARD_KIND)(kind2),list1,list2,len1,len2)==0)
        {
            chose_place=card_array->list[kind2].place[card2.same_times-1];
        }
    }
    if(chose_place==-1)
    {
        for(int i=18;i<21;i++)
            if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&card_array->list[i].same_times==1)
            {
                chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                break;
            }
    }
    if(chose_place==-1)
    {       
        for(int i=0;i<TOTAL_CARD_KIND;i++)
        {
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&
                    card_array->list[i].same_times==1&&river_reserved_card(card_array,i)!=4&&
                    Robot_check_card_stable(card_array,CARD_KIND(i))!=0)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
        }
    }
    if(chose_place==-1)
    {
        for(int i=0;i<TOTAL_CARD_KIND;i++)
        {
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                    &&card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==4&&
                    Robot_check_card_stable(card_array,card_kind(i))!=0)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
        }
    }
    if(chose_place==-1)
    {
        for(int i=0;i<TOTAL_CARD_KIND;i++)
        {
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                    &&card_array->list[i].same_times==2&&river_reserved_card(card_array,i)!=4&&
                    Robot_check_card_stable(card_array,CARD_KIND(i))!=0)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
        }
    }
    if(chose_place==-1)
    {
        for(int i=0;i<TOTAL_CARD_KIND;i++)
        {
                if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
                    &&card_array->list[i].same_times==2&&river_reserved_card(card_array,i)==4&&
                    Robot_check_card_stable(card_array,CARD_KIND(i))!=0)
                {
                    chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
                    break;
                }
        }
    }
    return chose_place;
}
int StrategyPlayer::Robot_pickup_single(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
    int chose_place=-1;
    
    switch(card_array->target)
    {
        case SAME_TIAO:
            chose_place=Robot_picup_single_for_samecolor(1,card_array,list1,list2,len1,len2);
            break;
        case SAME_TONG:
            chose_place=Robot_picup_single_for_samecolor(0,card_array,list1,list2,len1,len2);
            break;
        case SEVEN_COUPLES:
            chose_place=Robot_pickup_for_sevencouples(card_array,list1,list2,len1,len2);
            break;
        case FOUR_PENG:
            chose_place=Robot_pickup_for_fourpeng(card_array,list1,list2,len1,len2);
            break;
        default:
            chose_place=Robot_pickup_for_lowwin(card_array,list1,list2,len1,len2);
            break;      
    }
    return chose_place;
}

void StrategyPlayer::_CollectPosition(KindPosition *cards) {
    Card_t prev = CARD_UNKNOWN;

    for(int i=_employer->_cards->FreeStart;i<_employer->_cards->size();i++) {
        Card_t kind = _employer->_cards->get_kind(i);
        
        if(kind!=prev) {
            cards[kind].num = 0;
            
            cards[kind].position[cards[kind].num] = i;
            cards[kind].num = 1;

            prev = kind;
        } else {
            cards[kind].position[cards[kind].num] = i;
            cards[kind].num++;
        }
    }
}
