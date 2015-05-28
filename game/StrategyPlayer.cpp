
#include "CardCollection.h"
#include "Player.h"
#include "RoundManager.h"

#include "StrategyPlayer.h"

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
long StrategyPlayer::calc_score(Card_t kind) {
    bool isNewDistributed = _rm->_isNewDistributed;
    bool isLastOne        = (_rm->_distributedNum==TOTAL_CARD_NUM);
    ActionId_t action     = _rm->_lastActionWithGold;
    int continueGang      = _rm->_continue_gang_times;
    bool isGangHua        = _rm->_isGangHua;

    _employer->_cards->update_statistics(kind);
    _employer->_fan = _employer->_cards->statHuFanMask;
    
    if(isLastOne) {
		_employer->_fan |= RH_HAIDILAO;
    }

	if(isNewDistributed) {
		_employer->_fan |= RH_ZIMO;
	}

	if((continueGang!=0)
        &&(action==aMING_GANG||action==aAN_GANG||action==aQIANG_GANG)) {
		if(isNewDistributed&&action==aQIANG_GANG)
			_employer->_fan |= RH_QIANGGANG;
		else if(isNewDistributed&&isGangHua)
			_employer->_fan |= RH_GANGHUA;
		else if(!isNewDistributed)
			_employer->_fan |= RH_GANGPAO;
	}

	if(_employer->_AIM!=RA_NOTASK)
		check_task(_employer->_fan);
    
	return _employer->_cards->sum_up_score(_employer->_fan);
}

void StrategyPlayer::check_task(unsigned int flag) {
	if( !(flag&_employer->_AIM) ) {
		_employer->_aimDone=0;
    }
}

/******************************************************************
    选牌
******************************************************************/
bool StrategyPlayer::OthersCanHu(Card_t kind) const {/*the efficiency could be optimized*/
    for(int i=0;i<2;i++) {
        if(_ctx.OthersTing[i]==NULL) {
            continue;
        }
        
        for(int j=0;j<_ctx.OthersTing[i]->cardNum;j++) {
            if(kind==(_ctx.OthersTing[i]->cards+j)->kind) {
                return true;
            }
        }
    }

    return false;
}

int StrategyPlayer::AvailNum(Card_t kind) const {
    int num = _ctx.river->find_cards(kind);

    if(_ctx.remain<10 && num==0) {
        return 4;
        return 3;/*WHY???*/
    } else {
        return (4-num);
    }
}

bool StrategyPlayer::IsInStrictSequences(Card_t kind,int seqIdx) const {
    int num0 = _ctx.cards[kind].num;
    int num1 = 0;
    int num2 = 0;

    switch(seqIdx) {
        case 0:
            num1 = _ctx.cards[kind+1].num;
            num2 = _ctx.cards[kind+2].num;
            break;
        case 1:
            num1 = _ctx.cards[kind-1].num;
            num2 = _ctx.cards[kind+1].num;
            break;
        case 2:
            num1 = _ctx.cards[kind-2].num;
            num2 = _ctx.cards[kind-1].num;
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
    int num = _ctx.cards[kind].num;
    
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
        if(!OthersCanHu((Card_t)i) && _ctx.cards[i].num==1) {
            return _ctx.cards[i].position[_ctx.cards[i].num-1];
        }
    }

    return INVALID;
}

/* 只有一个，而且不连续 */
int StrategyPlayer::_FindSingleAndNonSequence(Card_t HeadKind,Card_t TailKind) {
    if(!OthersCanHu(HeadKind) && _ctx.cards[HeadKind].num==1 && _ctx.cards[HeadKind].num==0) {
        return _ctx.cards[HeadKind].position[_ctx.cards[HeadKind].num-1];
    } else if(!OthersCanHu(TailKind) && _ctx.cards[TailKind].num==1 && _ctx.cards[TailKind].num==0) {
        return _ctx.cards[TailKind].position[_ctx.cards[TailKind].num-1];
    } else {
        for(int i=HeadKind+1;i<TailKind;i++) {
            if(!OthersCanHu((Card_t)i)) {
                if(_ctx.cards[i].num==1 && _ctx.cards[i-1].num==0 && _ctx.cards[i+1].num==0) {
                    return _ctx.cards[i].position[_ctx.cards[i].num-1];
                }
            }
        }
    }

    return INVALID;
}

int StrategyPlayer::_FindSingleAndUnstable(Card_t HeadKind,Card_t TailKind) {
    if(!OthersCanHu(HeadKind) || !OthersCanHu(TailKind)) {
        if(_ctx.cards[HeadKind].num==1 && !IsStable(HeadKind)) {
            return _ctx.cards[HeadKind].position[_ctx.cards[HeadKind].num-1];
        }
        if(_ctx.cards[TailKind].num==1 && !IsStable(TailKind)) {
            return _ctx.cards[TailKind].position[_ctx.cards[TailKind].num-1];
        }
    }

    for(int i=HeadKind;i<TailKind;i++) {
        if(!OthersCanHu((Card_t)i)) {
            if(_ctx.cards[i].num==1 && !IsStable((Card_t)i)) {
                return _ctx.cards[i].position[_ctx.cards[i].num-1];
            }
        }
    }

    return INVALID;
}

int StrategyPlayer::PickupForSameColor(int reserveColor) {
    const Card_t RiverLast    = _ctx.river->get_kind(_ctx.river->last());
    const Card_t River2ndLast = _ctx.river->get_kind(_ctx.river->last()-1);
    const KindPosition &Card1 = _ctx.cards[RiverLast];
    const KindPosition &Card2 = _ctx.cards[River2ndLast];

    const int Hu1 = _ctx.OthersTing[0]->cardNum;
    const int Hu2 = _ctx.OthersTing[1]->cardNum;
    
    Card_t HeadKind = (Card_t)((1-reserveColor)*9);
    Card_t TailKind = (Card_t)(HeadKind+8);

    if(Hu1==0&&Hu2==0&&_ctx.remain>38) {
        for(int i=HeadKind;i<=TailKind;i++) {
            if(_ctx.cards[i].num>0 && !IsStable((Card_t)i)) {
                return _ctx.cards[i].position[_ctx.cards[i].num-1];
            }
        }
    }

    /*跟别人出*/
    if(!OthersCanHu(RiverLast) || !OthersCanHu(River2ndLast)) {
        if(River2ndLast/9==(1-reserveColor)) {
            if(Card1.num>0 && !IsStable(RiverLast)) {
                return _ctx.cards[RiverLast].position[_ctx.cards[RiverLast].num-1];
            } else if(Card2.num>0 && !IsStable(River2ndLast)) {
                return _ctx.cards[River2ndLast].position[_ctx.cards[River2ndLast].num-1];
            }
        }
    }

    RETURN_IF_VALID(_FindSingleChar());
    RETURN_IF_VALID(_FindSingleAndNonSequence(HeadKind,TailKind));
    RETURN_IF_VALID(_FindSingleAndUnstable(HeadKind,TailKind));

    for(int i=HeadKind;i<=TailKind;i++) {
        if(_ctx.cards[i].num>0 && !IsStable((Card_t)i) && !OthersCanHu((Card_t)i)) {
            return _ctx.cards[i].position[_ctx.cards[i].num-1];
        }
    }
    
    for(int i=ZHONG;i<ZHONG+3;i++) {
        if(_ctx.cards[i].num>0 && !IsStable((Card_t)i) && !OthersCanHu((Card_t)i)) {
            return _ctx.cards[i].position[_ctx.cards[i].num-1];
        }
    }

    for(int i=HeadKind;i<=TailKind;i++) {
        if(_ctx.cards[i].num>0 && !OthersCanHu((Card_t)i)) {
            return _ctx.cards[i].position[_ctx.cards[i].num-1];
        }
    }

    for(int i=ZHONG;i<ZHONG+3;i++) {
        if(_ctx.cards[i].num>0 && !OthersCanHu((Card_t)i)) {
            return _ctx.cards[i].position[_ctx.cards[i].num-1];
        }
    }

    /*同一种花色中选*/
    HeadKind = (Card_t)(reserveColor*9);
    TailKind = (Card_t)(HeadKind+8);

    RETURN_IF_VALID(_FindSingleAndNonSequence(HeadKind,TailKind));

    if(!OthersCanHu(HeadKind) && _ctx.cards[HeadKind].num==1) {
        return _ctx.cards[HeadKind].position[_ctx.cards[HeadKind].num-1];
    } else if(!OthersCanHu(TailKind) && _ctx.cards[TailKind].num==1) {
        return _ctx.cards[TailKind].position[_ctx.cards[TailKind].num-1];
    }
    
    for(int i=HeadKind;i<=TailKind;i++) {
        if(_ctx.cards[i].num>0 && !IsStable((Card_t)i) && !OthersCanHu((Card_t)i)) {
            return _ctx.cards[i].position[_ctx.cards[i].num-1];
        }
    }

    for(int i=HeadKind+1;i<TailKind;i++) {
        if(!OthersCanHu((Card_t)i)) {
            if(_ctx.cards[i].num==1 && _ctx.cards[i-1].num==0 && _ctx.cards[i+1].num==0) {
                return _ctx.cards[i].position[_ctx.cards[i].num-1];
            }
        }
    }

    for(int i=HeadKind;i<=TailKind;i++) {
        if(!OthersCanHu((Card_t)i) && _ctx.cards[i].num>0 && _ctx.cards[i].num<3) {
            return _ctx.cards[i].position[_ctx.cards[i].num-1];
        }
    }

    return INVALID;
}
    
int StrategyPlayer::PickupForSevenCouples() {
    const Card_t RiverLast    = _ctx.river->get_kind(_ctx.river->last());
    const Card_t River2ndLast = _ctx.river->get_kind(_ctx.river->last()-1);
    const KindPosition &Card1 = _ctx.cards[RiverLast];
    const KindPosition &Card2 = _ctx.cards[River2ndLast];

    const int Hu1 = _ctx.OthersTing[0]->cardNum;
    const int Hu2 = _ctx.OthersTing[1]->cardNum;
    
    if(_ctx.cards[RiverLast].num!=2 && _ctx.cards[RiverLast].num!=4
        && !OthersCanHu(RiverLast)) {
        
        return _ctx.cards[RiverLast].position[_ctx.cards[RiverLast].num-1];
        
    } else if(_ctx.cards[River2ndLast].num!=2 && _ctx.cards[River2ndLast].num!=4
        && !IsStable(River2ndLast)) {/*BUG HERE??? 没有判断是否会点炮*/
        
        return _ctx.cards[River2ndLast].position[_ctx.cards[River2ndLast].num-1];
        
    }

    RETURN_IF_VALID(_FindSingleChar());

    for(int i=0;i<TOTAL_CARD_KIND;i++) {
        if(!OthersCanHu((Card_t)i) && _ctx.cards[i].num==3) {
            return _ctx.cards[i].position[_ctx.cards[i].num-1];
        }
    }

    for(int avail=0;avail<=4;avail--) {
        for(int i=0;i<TOTAL_CARD_KIND;i++) {
            if(!OthersCanHu((Card_t)i)  && _ctx.cards[i].num==1 && AvailNum((Card_t)i)==avail) {
                return _ctx.cards[i].position[_ctx.cards[i].num-1];
            }
        }
    }

    return INVALID;
}

int StrategyPlayer::PickupForFourPeng() {
    const Card_t RiverLast    = _ctx.river->get_kind(_ctx.river->last());
    const Card_t River2ndLast = _ctx.river->get_kind(_ctx.river->last()-1);
    const KindPosition &Card1 = _ctx.cards[RiverLast];
    const KindPosition &Card2 = _ctx.cards[River2ndLast];

    const int Hu1 = _ctx.OthersTing[0]->cardNum;
    const int Hu2 = _ctx.OthersTing[1]->cardNum;
    
    if(_ctx.cards[RiverLast].num==1 && !IsStable(RiverLast) && !OthersCanHu(RiverLast)) {
        return _ctx.cards[RiverLast].position[_ctx.cards[RiverLast].num-1];
    } else if(_ctx.cards[River2ndLast].num==1 && !IsStable(RiverLast) && !OthersCanHu(RiverLast)) {
        return _ctx.cards[River2ndLast].position[_ctx.cards[River2ndLast].num-1];
    }

    RETURN_IF_VALID(_FindSingleChar());

    for(int cardInHand=1;cardInHand<3;cardInHand++) {
        for(int avail=0;avail<=4;avail++) {
            for(int i=0;i<TOTAL_CARD_KIND;i++) {/*LOGIC CHANGED!!!*/
                if(!OthersCanHu((Card_t)i)) {
                    if(_ctx.cards[i].num==cardInHand && AvailNum((Card_t)i)==avail) {
                        return _ctx.cards[i].position[_ctx.cards[i].num-1];
                    }
                }
            }
        }
    }

    return INVALID;
}

int StrategyPlayer::PickupForPiHu() {
    const Card_t RiverLast    = _ctx.river->get_kind(_ctx.river->last());
    const Card_t River2ndLast = _ctx.river->get_kind(_ctx.river->last()-1);
    const KindPosition &Card1 = _ctx.cards[RiverLast];
    const KindPosition &Card2 = _ctx.cards[River2ndLast];

    const int Hu1 = _ctx.OthersTing[0]->cardNum;
    const int Hu2 = _ctx.OthersTing[1]->cardNum;
    
    if(_ctx.cards[RiverLast].num==1 && !IsStable(RiverLast) && !OthersCanHu(RiverLast)) {
        return _ctx.cards[RiverLast].position[_ctx.cards[RiverLast].num-1];
    } else if(_ctx.cards[River2ndLast].num==1 && !IsStable(RiverLast) && !OthersCanHu(RiverLast)) {
        return _ctx.cards[River2ndLast].position[_ctx.cards[River2ndLast].num-1];
    }

    RETURN_IF_VALID(_FindSingleChar());

    for(int cardInHand=1;cardInHand<3;cardInHand++) {
        for(int avail=0;avail<4;avail++) {
                for(int i=0;i<TOTAL_CARD_KIND;i++) {
                    if(_ctx.cards[i].num==cardInHand && !IsStable((Card_t)i) && !OthersCanHu((Card_t)i) && AvailNum((Card_t)i)==avail) {
                        return _ctx.cards[i].position[_ctx.cards[i].num-1];
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

#include "RoundManager.h"

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

void StrategyPlayer::_SetContext()
{
    memset(&_ctx,0,sizeof(Context_t));

    _ctx.river  = _rm->_gRiver;
    _ctx.remain = TOTAL_CARD_NUM - _rm->_distributedNum;
    _ctx.aim    = _employer->_cards->assess_aim();

    int curPlayer = _rm->_curPlayer;
    _ctx.OthersTing[0] = _rm->_players[(curPlayer+1)%3]->_cards->_ting;
    _ctx.OthersTing[1] = _rm->_players[(curPlayer+2)%3]->_cards->_ting;

    _CollectPosition(_ctx.cards);
    
    if(_employer->_cards->_ting) {
        _ctx.huNum = _employer->_cards->_ting->cardNum;
    } else {
        _ctx.huNum = 0;
    }
}

int StrategyPlayer::PickupForMing(ActionId_t &ming,bool &canKou) {
    Card_t target = CARD_UNKNOWN;
    int    minum  = 0;

    if(_employer->_cards->collect_ming_info(_ctx.river)) {
        for(int i=0;i<_employer->_cards->_ming.choiceNum;i++) {
            MingChoice_t  *choice = _employer->_cards->_ming.handouts+i;

            if(!OthersCanHu(choice->kind)) {
                int num = _employer->_cards->get_ting_num(target);

                if(minum > num) {
                    target = choice->kind;
                    minum = num;
                }
            }
        }
    }

    for(int i=0;i<_employer->_cards->size();i++) {
        if(_employer->_cards->get_kind(i)==target) {
            _employer->_cards->set_ming(i);
            _ctx.huNum = _employer->_cards->_ting->cardNum;
            if(_ctx.huNum>=6) {
                canKou = true;
            }            

            return i;
        }
    }
    
    return INVALID;
}

int StrategyPlayer::choose_card(ActionId_t &ming,bool &canKou) {
    if(_employer->_cards->IsMing) {
        return _employer->_cards->last();
    } else {
        _SetContext();
        
        if(ming==aMING) {
            int index = PickupForMing(ming,canKou);
            if(index!=INVALID) {
                return index;
            } else {
                ming=aQi;
            }
        }

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
}


