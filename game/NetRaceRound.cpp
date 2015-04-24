
#include "./../utils/UtilBasic.h"

#include "NetRaceRound.h"
#include "CardCollection.h"

USING_NS_CC;

NetRRound::NetRRound(CardInHand *cardInHand,HuFan_t &fan,HuTarget_t aim)
:_fan(fan),_AIM(aim) {
    _cardInHand = cardInHand;
	_fan=0;

    _TingInfo.cardNum = 0;
    _TingInfo.cards   = NULL;

	hu_len=0;
    card_list = new CARD_ARRAY;

    _logger = LOGGER_REGISTER("RaceRound");
}

NetRRound::~NetRRound() {
    ClearTingInfo();
    delete card_list;
    LOGGER_DEREGISTER(_logger);
}

void NetRRound::ClearTingInfo() {
    for(int i=0;i<_TingInfo.cardNum;i++) {

    }
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

long NetRRound::sum_up_score(unsigned int fan) {
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

long NetRRound::cal_score(CARD_KIND kind,bool isCardFromOthers,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua)
{
    _cardInHand->update_statistics((Card_t)kind);
    _fan = _cardInHand->statHuFanMask;
    
    if(is_last_one) {
		_fan|=RH_HAIDILAO;
    }

	if(!isCardFromOthers) {
		_fan|=RH_ZIMO;
	}

	if((continue_gang_times!=0)
        &&(last_action_WithGold==a_MING_GANG
            ||last_action_WithGold==a_AN_GANG
            ||last_action_WithGold==a_QIANG_GANG))
	{
		if(!isCardFromOthers&&last_action_WithGold==a_QIANG_GANG)
			_fan |= RH_QIANGGANG;
		else if(!isCardFromOthers&&isGangHua)
			_fan |= RH_GANGHUA;
		else if(isCardFromOthers)
			_fan |= RH_GANGPAO;
	}

    long score = sum_up_score(_fan);
    
	if(_AIM!=RA_NOTASK)
		task_check(_fan);
    
	return score;
}

int NetRRound::cal_times(Card_t kind) {
    _cardInHand->update_statistics(kind);
    return sum_up_score(_cardInHand->statHuFanMask);
}

int NetRRound::cards_stable(CARD_KIND clist[],int len)
{
    SmartList cards;
    cards.len = len;
    for(int i=0;i<cards.len;i++) {
        cards.kind[i] = (Card_t)clist[i];
    }

    return cards.can_hu();
}

void NetRRound::task_check(unsigned int flag)
{
	if( !(flag&_AIM) )
		_aimDone=0;
}

int NetRRound::hu_check(CARD_KIND newCard)
{
    SmartList cards(*_cardInHand);
    
    cards.len--;                     /*the last should not be included*/
    cards.insert((Card_t)newCard);   /*the last inserted in order*/

    return cards.can_hu();
}

void NetRRound::transform(const SmartList &input,CARD_KIND output[]) {
    for(int i=0;i<input.len;i++) {
        output[i] = (CARD_KIND)input.kind[i];
    }
}

bool NetRRound::ting_check(int index,CARD_KIND cur_card,int kind,CARD_KIND rlist[])
{
    SmartList cards(*_cardInHand,true);
    cards.displace(index-_cardInHand->FreeStart,(Card_t)kind);

    transform(cards,rlist);

    return cards.can_hu();
}

int NetRRound::judge_kou_cards(CARD_KIND card,int no,CARD_KIND otherHandedOut)
{
    SmartList newCards = _cardInHand->_Exclude((Card_t)card);
    
	if(no==MIDDLE) {
		for(int i=0;i<newCards.len;i++) {
			for(int k=0;k<CARD_KIND_MAX;k++) {
                SmartList remain(*_cardInHand);
                remain.displace(i,(Card_t)k);
				if(remain.can_hu()) {
					return true;
                }
			}
        }
	} else {
		for(int i=0;i<newCards.len;i++) {
			if(newCards.kind[i]==otherHandedOut) {
                for(int k=0;k<CARD_KIND_MAX;k++) {
                    SmartList remain(*_cardInHand);
                    remain.displace(i,(Card_t)k);
                    if(remain.can_hu()) {
                        return true;
                    }
                }

                return false;
			}
		}
	}
    
	return false;
}

void NetRRound::get_hu_residueForEvery2(int curArray[MAX_HANDIN_NUM][9]) {
    for(int i=0;i<_TingInfo.cardNum;i++) {
        Card_t huKind = _TingInfo.cards[i].kind;
        int    sameKindInHand = 0;

        for(int j=0;j<_cardInHand->size();j++) {
            /* why this judgement is required ??? */
            if(_cardInHand->get_status(j)==sFREE||_cardInHand->get_status(j)==c_AN_GANG||_cardInHand->get_status(j)==sMING_KOU) {
                if(huKind==_cardInHand->get_kind(j)) {
                    sameKindInHand++;
                }
            }
        }

        curArray[i][huKind] = hu_residueForEvery[i][huKind] - sameKindInHand;
    }
}

void NetRRound::get_hu_residueForEvery(int curArray[MAX_HANDIN_NUM][9])
{
	int temp=0;
    
	for(int a=0;a<MAX_HANDIN_NUM;a++)
	{
		if(hu_cards[a][0]==-1)
			continue;

		for(int b=0;b<9;b++)
		{
			if(hu_cards[a][b]==-1)
				break;

            int num=0;
			for(int c=0;c<_cardInHand->size();c++)
			{
				if(_cardInHand->get_status(c)==sFREE||_cardInHand->get_status(c)==c_AN_GANG||_cardInHand->get_status(c)==sMING_KOU)
					if(hu_cards[a][b]==_cardInHand->get_kind(c))
						num++;
			}
			temp=hu_residueForEvery[a][b]-num;
			curArray[a][b]=temp;
		}
	}
}

unsigned int NetRRound::ming_check2()
{
	unsigned int handoutMask =0;

	hu_places_num=0;

	for(int i=_cardInHand->FreeStart;i<_cardInHand->size();i++)
	{
		if(_cardInHand->get_status(i)==sMING_KOU) {
			continue;
        } else {
            Card_t lastHuCard = CARD_UNKNOWN;
            Card_t curCard    = (CARD_KIND)_cardInHand->get_kind(i);;
            
            if(curCard==lastHuCard && lastHuCard!=CARD_UNKNOWN) {
                handoutMask |= (1<<i);
            } else {
                ClearTingInfo();
            
                TingItem_t *huCards = new TingItem_t[9];
                
                for(int k=0;k<CARD_KIND_MAX;k++) {
                    SmartList cards(*_cardInHand,true);
                    cards.displace(i-_cardInHand->FreeStart,(Card_t)k);

                    if( cards.can_hu() ) {
                        handoutMask |= (1<<i);
                        lastHuCard = curCard;

                        (huCards+_TingInfo.cardNum)->kind = (Card_t)k;
                        (huCards+_TingInfo.cardNum)->fan  = cal_times(k);

                        /* why??? */
                        for(int s_l=0;s_l<_cardInHand->FreeStart;s_l++) {
                            if( _cardInHand->get_kind(s_l)==k&&_cardInHand->get_status(s_l)==sPENG) {
                                (huCards+_TingInfo.cardNum)->fan *= 2;
                                break;
                            }
                        }

                        _TingInfo.cards = huCards;
                        _TingInfo.cardNum++;
                    }
                }
            }
        }
	}
    
	for(int i=0;i<MAX_HANDIN_NUM;i++) {
		if(handoutMask&(1<<i)) {
			hu_places_num++;
        }
    }

    hu_places = handoutMask;

    return handoutMask;
}

unsigned int NetRRound::ming_check()
{
	unsigned int handoutMask =0;

	hu_places_num=0;
	memset(hu_cards_num,0,sizeof(int)*MAX_HANDIN_NUM);
	memset(hu_cards,0xff,sizeof(CARD_KIND)*MAX_HANDIN_NUM*9);
	memset(huTiemsForEveryOne,0,sizeof(int)*MAX_HANDIN_NUM*9);

    CARD_KIND last_card=ck_NOT_DEFINED;

	for(int i=_cardInHand->FreeStart;i<_cardInHand->size();i++)
	{
		if(_cardInHand->get_status(i)==sMING_KOU) {
			continue;
        } else {
            CARD_KIND cur_card = (CARD_KIND)_cardInHand->get_kind(i);;
            
            if(cur_card==last_card)
            {
                handoutMask |= ( ( handoutMask&( 1<<(i-1) ) )<<1 );

                hu_cards_num[i]=hu_cards_num[i-1];
                memcpy(hu_cards[i],hu_cards[i-1],sizeof(CARD_KIND)*9);
                memcpy(huTiemsForEveryOne[i],huTiemsForEveryOne[i-1],sizeof(int)*9);
            } else {
                int huKindIdx = 0;
                
                for(int k=0;k<CARD_KIND_MAX;k++) {
                    CARD_KIND temp_list[MAX_HANDIN_NUM];
                    memset(temp_list,ck_NOT_DEFINED,MAX_HANDIN_NUM*sizeof(CARD_KIND));

                    if(ting_check(i,cur_card,k,temp_list)) {
                        handoutMask |= (1<<i);
                        
                        hu_cards[i][huKindIdx]=CARD_KIND(k);
                        hu_cards_num[i]++;
                        
                        int s_core=cal_times((Card_t)k);

                        for(int s_l=0;s_l<_cardInHand->FreeStart;s_l++) {
                            if( _cardInHand->get_kind(s_l)==CARD_KIND(k)&&_cardInHand->get_status(s_l)==sPENG) {
                                s_core *= 2;
                                break;
                            }
                        }

                        huTiemsForEveryOne[i][huKindIdx++]=s_core;
                    }
                }
                
                last_card = cur_card;
            }
        }
	}
    
	for(int i=0;i<MAX_HANDIN_NUM;i++) {
		if(handoutMask&(1<<i)) {
			hu_places_num++;
        }
    }

    hu_places = handoutMask;

    return handoutMask;
}

unsigned char NetRRound::init(int card_array[],int len,int aim)
{
	int i;

	card_score=0;
	hu_len=0;
	hu_places_num=0;
	memset(hu_cards_num,0,sizeof(int)*MAX_HANDIN_NUM);
	memset(hu_cards,0xff,sizeof(CARD_KIND)*MAX_HANDIN_NUM*9);

    _cardInHand->clear();
	_cardInHand->FreeStart =0;

	CardNode_t node;
	unsigned char ac=a_JUMP;
	for(i=0;i<len;i++)
	{
		node.kind    = (Card_t)(card_array[i]/4);
		node.status  = sFREE;
		node.canPlay = true;
        
		if(i==13) {
			ac = hand_in((CARD_KIND)(card_array[i]/4),0,0,false,a_JUMP,0,false);
        } else {
			_cardInHand->insert_card(node,1);
        }
	}
    
	return ac;
}

unsigned char NetRRound::ActiontodoCheckAgain()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	unsigned char res = 0x0;

    if(_cardInHand->is_shou_gang()) {
        res |= aSHOU_GANG;
    }

	if( ( archive_ming_indexes=ming_check() )!=0 )
		res |= aMING;

	return res;
}

unsigned char NetRRound::hand_in(CARD_KIND kind,unsigned char isCardFromOthers,unsigned char tingStatus,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua)
{
	int num = 0;
	unsigned char res = 0x0;

    CardNode_t *card = new CardNode_t;
	card->kind     = (Card_t)kind;
	card->status   = sFREE;
	card->canPlay = true;
    _cardInHand->push_back(card);

	if(!_cardInHand->IsMing)
	{
		int card_num;
		for(int i=_cardInHand->FreeStart;i<_cardInHand->size();i++)
		{
			card_num=1;
			for(int k=i+1;k<_cardInHand->size();k++)
			{
				if(_cardInHand->get_kind(k)==_cardInHand->get_kind(i))
					card_num++;
			}
			if(card_num==4 &&!isCardFromOthers&&!is_last_one)
			{
				res |= a_SHOU_GANG;
				break;
			}
		}
		for(int i=0;i<_cardInHand->size();i++)
		{
			if(_cardInHand->get_kind(i)==kind)
			{
				if(_cardInHand->get_status(i)==sPENG)
				{
					if(!isCardFromOthers&&!is_last_one)
					{
						res |= a_MING_GANG;
					}
					break;
				}
				else if(_cardInHand->get_status(i)==sFREE)
				{
					num++;
				}
			}
		}
		if(num==3)
		{
			if(!isCardFromOthers&&!is_last_one)
				res |= (a_MING_GANG | a_AN_GANG);
			else if(!is_last_one)
				res |= (a_MING_GANG | a_PENG);
			else if(is_last_one)
				res |= a_PENG;
		}
		else if(num==2&&isCardFromOthers)
		{
				res |= a_PENG;
		}
	}
	else
	{

		for(int i=0;i<_cardInHand->FreeStart;i++)
			if(_cardInHand->get_status(i)==sMING_KOU && _cardInHand->get_kind(i)==kind)
			{
				if(!is_last_one)
				{
					if(!isCardFromOthers)
						res |= (a_MING_GANG | a_AN_GANG);
					else
						res |= a_MING_GANG;
				}
				break;
			}
	}

	if(hu_check(kind)==1)
	{
		card_score=cal_score(kind,isCardFromOthers,is_last_one,last_action_WithGold,continue_gang_times,isGangHua);
		if(!isCardFromOthers||(isCardFromOthers==1&&(tingStatus==1||card_score!=1)))
			res|=a_HU;
	}

	if(!isCardFromOthers)
	{
		if(!_cardInHand->IsMing && !is_last_one)
		{
			if(( archive_ming_indexes=ming_check())!=0 )
				res |= a_MING;
		}
	}

    LOGGER_WRITE("NETWORK : %x %s action %d: kind %d,isCardFromOthers %d,tingStatus %d,is_last_one %d",
        (int)this,__FUNCTION__,res,kind,isCardFromOthers,tingStatus,is_last_one);
	return res;
}
/*if ting place=_cardInHand->size()*/
CARD_KIND NetRRound::hand_out(unsigned int place)
{
	CARD_KIND l_kind = (CARD_KIND)_cardInHand->get_kind(place);

	if( _cardInHand->get_status(place) != sFREE ){
		return ck_NOT_DEFINED;
	}

    _cardInHand->delete_card(place,1);

    return l_kind;
}


void NetRRound::LockAllCards()
{
}

void NetRRound::MingCancel()
{
}


ACT_RES NetRRound::others_action(bool isCardFromOthers,ARRAY_ACTION act,Card_t kind)
{
    LOGGER_WRITE("%x %s : %d (isCardFromOthers=%d)",this,__FUNCTION__,act,isCardFromOthers);

	InsertPlaceForMG=0;

    CardNode_t node;
    node.kind    = kind;
    node.canPlay = false;

	if(act==a_PENG) {
        node.status=sPENG;
        
        _cardInHand->delete_card(_cardInHand->FreeStart,3);
        _cardInHand->insert_card(node,3);
        _cardInHand->FreeStart += 3;
	} else if(act==a_MING_GANG) {
        node.status=sMING_GANG;

        _cardInHand->delete_card(_cardInHand->FreeStart,4);
        _cardInHand->insert_card(node,4);
		_cardInHand->FreeStart += 4;
	} else if(act==a_AN_GANG) {
        node.kind=CARD_UNKNOWN;
        node.status=sAN_GANG;

        _cardInHand->delete_card(_cardInHand->FreeStart,4);
        _cardInHand->insert_card(node,4);
		_cardInHand->FreeStart += 4;
	} else if(act==a_JUMP) {
		if(!isCardFromOthers) {
			node.status  = sFREE;
            node.canPlay = false;
            
			_cardInHand->pop_back();
            _cardInHand->insert_card(node,1);
		}
	}

	return ar_DONE;
}

ACT_RES NetRRound::action(bool isCardFromOther,ARRAY_ACTION act)
{
    LOGGER_WRITE("%x %s : %d (isCardFromOther=%d)",this,__FUNCTION__,act,isCardFromOther);

	InsertPlaceForMG=-1;

	CARD temp_data;
	temp_data.can_play=cps_YES;
	temp_data.kind=(CARD_KIND)_cardInHand->back()->kind;

    CardNode_t node;
    node.kind    = (Card_t)_cardInHand->back()->kind;
    node.canPlay = false;

	if(act==a_PENG) {
        node.status=sPENG;

        for(int i=_cardInHand->FreeStart;i<_cardInHand->size();i++) {
			if(_cardInHand->get_kind(i)==node.kind){
				_cardInHand->delete_card(i,2);
				break;
			}
        }

        _cardInHand->pop_back();
        _cardInHand->insert_card(node,3);
        _cardInHand->FreeStart += 3;
	}
	else if(act==a_KOU) {
        node.status=sMING_KOU;

		for(int i=_cardInHand->size()-1;i>=_cardInHand->FreeStart;i--)
			if(_cardInHand->get_status(i)==sMING_KOU) {
				node.kind=_cardInHand->get_kind(i);
				_cardInHand->delete_card(i,1);
				_cardInHand->insert_card(node,1);
                _cardInHand->FreeStart+=1;
				i++;
			}
	}
	else if(act==a_KOU_CANCEL) {
		node.status  = sFREE;
		node.canPlay = true;
        
		for(int i=_cardInHand->FreeStart-1;i>=0;i--) {
			if(_cardInHand->get_status(i)==sMING_KOU) {
				node.kind = _cardInHand->get_kind(i);
                
				_cardInHand->delete_card(i,1);
				_cardInHand->insert_card(node,1);
				_cardInHand->FreeStart-=1;
			}
        }
	}
	else if(act==a_MING_GANG) {
		node.status = sMING_GANG;

		bool ifFirst=true;
		for(int i=_cardInHand->size()-1;i>=0;i--) {
			if(_cardInHand->get_status(i)==sPENG&&_cardInHand->get_kind(i)==node.kind) {
				if(ifFirst)
					InsertPlaceForMG=i-2;
                
				ifFirst=false;

                _cardInHand->FreeStart--;
			} else if(_cardInHand->get_status(i)==sMING_KOU&& _cardInHand->get_kind(i)==node.kind) {
				_cardInHand->FreeStart--;
            }

			if(_cardInHand->get_kind(i)==node.kind)
				_cardInHand->delete_card(i,1);
		}
        _cardInHand->insert_card(node,4);
		_cardInHand->FreeStart += 4;
	}
	else if(act==a_AN_GANG) {
		_cardInHand->perform(aAN_GANG);
	}
	else if(act==a_SHOU_GANG) {
		int card_num;
		int i;
		for(i=_cardInHand->FreeStart;i<_cardInHand->size();i++)
		{
			card_num=1;
			for(int j=i+1;j<_cardInHand->size();j++)
				if(_cardInHand->get_kind(i)==_cardInHand->get_kind(j))
					card_num++;
			if(card_num==4) {
				node.kind = _cardInHand->get_kind(i);
				break;
			}
		}
        
		for(int k=_cardInHand->size();k>=_cardInHand->FreeStart;k--)
		{
			if(_cardInHand->get_kind(k)==_cardInHand->get_kind(i))
				_cardInHand->delete_card(k,1);
		}

		node.status  = sAN_GANG;
		node.canPlay = false;
        _cardInHand->insert_card(node,4);
		_cardInHand->FreeStart += 4;

        /*WHY*/
		node.kind    = _cardInHand->back()->kind;
		node.status  = sFREE;
		node.canPlay = true;
        
		_cardInHand->pop_back();
        _cardInHand->insert_card(node,1);
	}
	else if(act==a_MING)
	{
		unsigned int active_place_indexes = get_ming_indexes();
        
		for (int i=_cardInHand->FreeStart;i<_cardInHand->size();i++) {
			int m = active_place_indexes&(1<<i);
			if(m>0) {
				_cardInHand->at(i)->canPlay=true;
			} else {
				_cardInHand->at(i)->canPlay=false;
			}
			
		}
	}
	else if(act==a_HU)
	{
		node.status  = sFREE;
		node.canPlay = false;
        
		if(isCardFromOther) {
			_cardInHand->insert_card(node,1);
        } else {
			_cardInHand->pop_back();
            _cardInHand->insert_card(node,1);
		}
	
	}
	else if(act==a_JUMP)
	{
		if(!isCardFromOther) {
			node.status=sFREE;
            
			_cardInHand->pop_back();
            _cardInHand->insert_card(node,1);
		}
	}

	return ar_DONE;
}

void NetRRound::get_hu_cards(CARD_KIND curArray[MAX_HANDIN_NUM][9])
{
	for(int a=0;a<MAX_HANDIN_NUM;a++)
		for(int b=0;b<9;b++)
			curArray[a][b]=hu_cards[a][b];
}
void NetRRound::get_huTiemsForEveryOne(int curArray[MAX_HANDIN_NUM][9])
{
	for(int a=0;a<MAX_HANDIN_NUM;a++)
		for(int b=0;b<9;b++)
			curArray[a][b]=huTiemsForEveryOne[a][b]*2;
}
void NetRRound::get_hu_NumForEveryCard(int curArray[MAX_HANDIN_NUM])
{
	for(int a=0;a<MAX_HANDIN_NUM;a++)
			//curArray[a]=hu_NumForEveryCard[a];
			curArray[a]=hu_cards_num[a];
}

unsigned int NetRRound::get_aim()
{
	return _aimDone;
}

unsigned int NetRRound::get_ming_indexes()
{
	return archive_ming_indexes;
}

int *NetRRound::get_ming_reserved_cards_num(CardList *river)
{
	int i;	
	memset(hu_residueForEvery,0,sizeof(int)*MAX_HANDIN_NUM*9);

	int m=0;
	for(i=0;i<MAX_HANDIN_NUM;i++)
		if(archive_ming_indexes&(1<<i))
		{
			m=0;
			for(int j=0;j<9;j++)
				if(hu_cards[i][j]!=-1)
				{
					int k=1;
					int default_reserved_cards=4;

                    for(int idx=0;idx<river->size();idx++) {
						if(river->get_kind(idx)==hu_cards[i][j])
							default_reserved_cards--;
                    }
					hu_residueForEvery[i][m++]=default_reserved_cards;
					hu_reserved_num[i] += default_reserved_cards;
				}
		}
	return hu_reserved_num;
}

void NetRRound::set_ming_indexes(unsigned int indexesFlag)
{
	archive_ming_indexes=indexesFlag;
}

void NetRRound::set_ting_status(unsigned char flag)
{
    _cardInHand->IsMing = (flag==1);

	hu_len=0;
	for(int i=0;i<21;i++)
		if(hu_check(CARD_KIND(i))==1)
			hucards[hu_len++]=CARD_KIND(i);
}

void NetRRound::get_hu_cards(CARD_KIND c_list[],int *len)
{
	*len=hu_len;
	for(int k=0;k<hu_len;k++)
		c_list[k]=hucards[k];
}

unsigned char NetRRound::get_ting_status()
{
	return _cardInHand->IsMing;
}

long NetRRound::get_card_score()
{
	return card_score;
}

CARD_ARRAY* NetRRound::get_card_list() {
    delete card_list;
    card_list = new CARD_ARRAY;
    
    for(int i=0;i<_cardInHand->size();i++) {
        card_list->data[i].kind = (CARD_KIND)_cardInHand->get_kind(i);
        card_list->data[i].status = (CARD_STATUS)_cardInHand->get_status(i);
        card_list->data[i].can_play = _cardInHand->canPlay(i)?cps_YES:cps_NO;
    }

    card_list->len = _cardInHand->size();
    card_list->atcvie_place = _cardInHand->FreeStart;
    
	return card_list;
}

bool NetRRound::get_Hu_Flag(unsigned int *hu_kind)
{
	*hu_kind=_fan;
	return true;
}

bool NetRRound::get_ming_check_result(MRES *res)
{
	//unsigned int result;
	if(hu_places_num!=0)
	{
		res->hu_places_num=hu_places_num;
		res->hu_places=hu_places;
		for(int i=0;i<MAX_HANDIN_NUM;i++)
			res->hu_cards_num[i]=hu_cards_num[i];
		for(int i=0;i<MAX_HANDIN_NUM;i++)
			for(int j=0;j<hu_cards_num[i];j++)
				res->hu_cards[i][j]=hu_cards[i][j];

		return true;
	}
	else
		return false;
}

