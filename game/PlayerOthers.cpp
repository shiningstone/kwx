
#include "./../utils/LogManager.h"

#include "CardCollection.h"
#include "PlayerOthers.h"

USING_NS_CC;

PlayerOthers::PlayerOthers(PlayerDir_t dir)
    :Player(dir){
    _logger = LOGGER_REGISTER("PlayerOthers");
}

PlayerOthers::PlayerOthers(int id,PlayerDir_t dir)
    :Player(dir){//this is for default settings ( robot ) 
    _logger = LOGGER_REGISTER("NetPlayer_%d");
}

PlayerOthers::~PlayerOthers() {
    LOGGER_DEREGISTER(_logger);
}

bool PlayerOthers::OthersCanHu(Card_t kind) const {/*the efficiency could be optimized*/
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

int PlayerOthers::AvailNum(Card_t kind) const {
    int num = _ctx.river->find_cards(kind);

    if(_ctx.remain<10 && num==0) {
        return 4;
        return 3;/*WHY???*/
    } else {
        return (4-num);
    }
}

bool PlayerOthers::IsInStrictSequences(Card_t kind,int seqIdx) const {
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

bool PlayerOthers::IsInSequences(Card_t kind) const {
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

bool PlayerOthers::IsStable(Card_t kind) const {
    int num = _ctx.cards[kind].num;
    
    if( num>=3 ) {
        return true;
    } else if (num>=1 && IsInSequences(kind)) {
        return true;
    }

    return false;
}

int PlayerOthers::Robot_check_pickup_card(CARD_KIND kind,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
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

int PlayerOthers::river_reserved_card(HAH *hash_table,int card)
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

int PlayerOthers::Robot_check_card_stable(HAH *card_array,CARD_KIND card)
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

int PlayerOthers::_FindSingleChar() {
    for(int i=ZHONG;i<=BAI;i++) {
        if(!OthersCanHu((Card_t)i) && _ctx.cards[i].num==1) {
            return _ctx.cards[i].position[_ctx.cards[i].num-1];
        }
    }

    return INVALID;
}

/* 只有一个，而且不连续 */
int PlayerOthers::_FindSingleAndNonSequence(Card_t HeadKind,Card_t TailKind) {
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

int PlayerOthers::_FindSingleAndUnstable(Card_t HeadKind,Card_t TailKind) {
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

#define RETURN_IF_VALID(x) do { \
    int chosen = x;             \
                                \
    if(chosen!=INVALID)         \
        return chosen;          \
}while(0)

int PlayerOthers::PickupForSameColor(int reserveColor) {
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
    
int PlayerOthers::PickupForSevenCouples() {
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

int PlayerOthers::PickupForFourPeng() {
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

int PlayerOthers::PickupForPiHu() {
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

int PlayerOthers::PickupForMing() {
    Card_t target = CARD_UNKNOWN;
    int    minum  = 0;
    
    if(_cards->collect_ming_info(_ctx.river)) {
        for(int i=0;i<_cards->_ming.choiceNum;i++) {
            MingChoice_t  *choice = _cards->_ming.handouts+i;

            if(!OthersCanHu(choice->kind)) {
                int num = _cards->get_ting_num(target);

                if(minum > num) {
                    target = choice->kind;
                    minum = num;
                }
            }
        }
    }

    for(int i=0;i<_cards->size();i++) {
        if(_cards->get_kind(i)==target) {
            return i;
        }
    }
    
    return INVALID;
}

int PlayerOthers::Robot_picup_single_for_samecolor(int color,HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
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

int PlayerOthers::Robot_pickup_for_sevencouples(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
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

int PlayerOthers::Robot_pickup_for_fourpeng(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
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

int PlayerOthers::Robot_pickup_for_lowwin(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
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
int PlayerOthers::Robot_pickup_single(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
	int chose_place=-1;
	
	switch(card_array->target)
	{
		case SAME_TIAO_TARGET:
			chose_place=Robot_picup_single_for_samecolor(1,card_array,list1,list2,len1,len2);
			break;
		case SAME_TONG_TARGET:
			chose_place=Robot_picup_single_for_samecolor(0,card_array,list1,list2,len1,len2);
			break;
		case SEVEN_COUPLES_TARGET:
			chose_place=Robot_pickup_for_sevencouples(card_array,list1,list2,len1,len2);
			break;
		case FOUR_PENG_TARGET:
			chose_place=Robot_pickup_for_fourpeng(card_array,list1,list2,len1,len2);
			break;
		default:
			chose_place=Robot_pickup_for_lowwin(card_array,list1,list2,len1,len2);
			break;		
	}
	return chose_place;
}

int PlayerOthers::ChooseCard(HAH *pres,int reseved,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
	//unsigned char ting_flag;
    int show_place = -1;
	int hu_num     = -1;
    
	MRES *res      = new MRES;
	
	for(int i=0;i<MAX_HANDIN_NUM;i++) {
		memset(res->hu_cards[i],ck_NOT_DEFINED,sizeof(CARD_KIND)*9);
    }

	if( _cards->get_ming_info(res) ) {
		for(int k=0;k<MAX_HANDIN_NUM;k++) {
			if(_cards->can_handout(k)) {
			    Card_t s_kind = _cards->get_kind(k);
				if(Robot_check_pickup_card((CARD_KIND)s_kind,list1,list2,len1,len2)!=0) {
					res->hu_cards_num[k]=0;
					continue;
				}
                
				int s_num=0;
				for(int j=0;j<9;j++)
					if(res->hu_cards[k][j]>=0)
					{
						s_num += river_reserved_card(pres,res->hu_cards[k][j]);
						s_num -= pres->list[res->hu_cards[k][j]].same_times;
					}
				res->hu_cards_num[k]=s_num;
			} else {
				res->hu_cards_num[k]=0;
            }
		}
		hu_num=res->hu_cards_num[0];
        
		int l_place    = -1;

		for(int i=0;i<MAX_HANDIN_NUM;i++) {
			if(_cards->can_handout(i)) {
				Card_t s_kind = _cards->get_kind(i);
				if(res->hu_cards_num[i]>=hu_num&&Robot_check_pickup_card((CARD_KIND)s_kind,list1,list2,len1,len2)==0)
					l_place=i;
			}
        }

		show_place = l_place;
		pres->hu_nums=res->hu_cards_num[l_place];
	}

	if(show_place==-1) {
		show_place=Robot_pickup_single(pres,list1,list2,len1,len2);
	}
    
	if(res)
		delete res;

	return show_place;
}

#include "RoundManager.h"

void PlayerOthers::_CollectPosition(KindPosition *cards) {
    Card_t prev = CARD_UNKNOWN;

    for(int i=_cards->FreeStart;i<_cards->size();i++) {
        Card_t kind = _cards->get_kind(i);
        
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

void PlayerOthers::_SetContext(HAH *res,CARD_KIND target1[],CARD_KIND target2[],int *len1,int *len2,RoundManager &context)
{
    memset(&_ctx,0,sizeof(Context_t));

    _ctx.river  = context._gRiver;
    _ctx.remain = TOTAL_CARD_NUM - context._distributedNum;
    _ctx.aim    = _cards->assess_aim();

    int curPlayer = context._curPlayer;
    _ctx.OthersTing[0] = context._players[(curPlayer+1)%3]->_cards->_ting;
    _ctx.OthersTing[1] = context._players[(curPlayer+2)%3]->_cards->_ting;

    _CollectPosition(_ctx.cards);
    if(_cards->_ting) {
        _ctx.huNum = _cards->_ting->cardNum;
    } else {
        _ctx.huNum = 0;
    }
    /*******************************************/
	memset(res,0,sizeof(HAH));
	memset(res->card_in_river,ck_NOT_DEFINED,sizeof(CARD_KIND)*TOTAL_CARD_NUM);

    res->reserved_card_num = TOTAL_CARD_NUM - context._distributedNum;
    
	CARD s_card;

    for(int i=0;i<context._gRiver->size();i++) {
		res->card_in_river[res->river_len++] = (CARD_KIND)context._gRiver->get_kind(i);
    }

	context._players[(curPlayer+1)%3]->_cards->get_hu_cards(target1,len1);
	context._players[(curPlayer+2)%3]->_cards->get_hu_cards(target2,len2);

    for(int i=_cards->FreeStart;i<_cards->size();i++) {
		int time = res->list[_cards->get_kind(i)].same_times++;
		res->list[_cards->get_kind(i)].place[time]=i;
	}

	res->target = _cards->assess_aim();
}

int PlayerOthers::choose_card(RoundManager &context,bool &canKou) {
    if(_cards->IsMing) {
        return _cards->last();
    } else {
        HAH *s_res = new HAH;
        
        CARD_KIND list1[9];
        CARD_KIND list2[9];
        int len1;
        int len2;
        
        canKou = false;
        _SetContext(s_res,list1,list2,&len1,&len2,context);
        
		int index = ChooseCard(s_res,TOTAL_CARD_NUM - context._distributedNum,list1,list2,len1,len2);
		if( index==INVALID ) {
			index = _cards->last();
		}
        
		if(context._actCtrl.decision==aMING && s_res->hu_nums>=6 ) {
			canKou = true;
		}

        return index;
    }
}

