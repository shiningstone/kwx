
#include "NetRaceRound.h"
#include "CardCollection.h"

USING_NS_CC;

NetRRound::NetRRound(CardInHand *cardInHand) {
	hu_len=0;
	kind_hu=0;
    _cardInHand = cardInHand;
    card_list = new CARD_ARRAY;

    _logger = LOGGER_REGISTER("RaceRound");
}

NetRRound::~NetRRound() {
    delete card_list;
    LOGGER_DEREGISTER(_logger);
}

long NetRRound::cal_score(CARD_KIND kind,bool isCardFromOthers,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua)
{
	int i;
	unsigned char color;
	unsigned char l_four_flag=0;
	bool HuQingYiSe = true;
	bool HuMingSiGui = false;
	bool HuPengPengHu = false;
	unsigned char free_num=0;
	unsigned char couple_num=0;
    
	unsigned char zhong_num=0;
	unsigned char fa_num=0;
	unsigned char bai_num=0;

    unsigned char four_num=0;
	unsigned char last_card_same_num=0;
	unsigned int  hu_flag = 0;
	long score = 1;
	color=kind/9;

    //this process could be moved to _cardInHand
	for(i=0;i<_cardInHand->size();i++)
	{
		if(	_cardInHand->get_kind(i)/9!=color )
		{
			HuQingYiSe = false;
		}
		if(_cardInHand->get_status(i)==sFREE )
		{
			free_num++;
		}
		if( _cardInHand->get_kind(i)==(Card_t)kind )
		{
			if(_cardInHand->get_status(i)==sPENG)
				HuMingSiGui = true;
			last_card_same_num++;
		}
        
		int l_cards=1;
		for(int k=i+1;k<_cardInHand->size();k++)
			if(_cardInHand->get_kind(i)==_cardInHand->get_kind(k) &&
			 _cardInHand->get_status(i)==sFREE && _cardInHand->get_status(k)==sFREE)
				l_cards++;
        if(l_cards==2||l_cards==4)
			couple_num++;

        l_four_flag=1;
        for(int k=i+1;k<_cardInHand->size();k++)
			if(_cardInHand->get_kind(i)==_cardInHand->get_kind(k) &&
			_cardInHand->get_status(k)==sFREE)
				l_four_flag++;
        if(l_four_flag==4)
			four_num++;

		if(_cardInHand->get_kind(i)==ZHONG)
		{
			zhong_num++;
		}
		else if(_cardInHand->get_kind(i)==FA)
		{
			fa_num++;
		}
		else if(_cardInHand->get_kind(i)==BAI)
		{
			bai_num++;
		}
	}
    
	if(free_num!=2)
	{
		int len=_cardInHand->size()-_cardInHand->active_place;
		int l_couple=0;
		for(int k=_cardInHand->active_place;k<_cardInHand->size();k++)
		{
			int l_num=1;
			for(int t=k+1;t<_cardInHand->size();t++)
				if(_cardInHand->get_kind(k)==_cardInHand->get_kind(t))
					l_num++;
                
			if(l_num==3)
				len -= 3;

            l_num=0;

            for(int t=_cardInHand->active_place;t<_cardInHand->size();t++)
				if(_cardInHand->get_kind(k)==_cardInHand->get_kind(t))
					l_num++;

            if(l_num==2)
				l_couple++;
		}
		if(len==2&&l_couple==2)
			HuPengPengHu = true;
	}
	else
	{
		score *=4;
		hu_flag|=RH_SHOUYIZHUA;//手抓一
	}
    
	if(is_last_one)
	{
		score *= 2;
		hu_flag|=RH_HAIDILAO;//海底捞
	}
	if(rr_ting_flag==1)
	{
		score *= 2;
		hu_flag|=RH_MING;//明牌
	}
	if(!isCardFromOthers)
	{
		score *= 2;
		hu_flag|=RH_ZIMO;//自摸
	}
	if(HuQingYiSe)
	{
		score *= 4;
		hu_flag |=RH_QINYISE;//清一色
	}
	if(HuPengPengHu)
	{
		score *= 2;
		hu_flag |=RH_SIPENG;//碰碰胡
	}
	if((continue_gang_times!=0)&&(last_action_WithGold==a_MING_GANG||last_action_WithGold==a_AN_GANG||last_action_WithGold==a_QIANG_GANG))//杠胡
	{
		score *= 2;
		if(!isCardFromOthers&&last_action_WithGold==a_QIANG_GANG)
			hu_flag |= RH_QIANGGANG;
		else if(!isCardFromOthers&&isGangHua)
			hu_flag |= RH_GANGHUA;
		else if(isCardFromOthers)
			hu_flag |= RH_GANGPAO;
	}
	if(zhong_num>=3&&fa_num>=3&&bai_num>=3)//大三元
	{
		score *= 8;
		hu_flag |= RH_DASANYUAN;
	}
	else if( (zhong_num>=3&&fa_num>=3&&bai_num==2) ||
			 (zhong_num==2&&fa_num>=3&&bai_num>=3) ||
			 (zhong_num>=3&&fa_num==2&&bai_num>=3) )
	{
		score *= 4;
		hu_flag |= RH_XIAOSANYUAN;//小三元
	}
	if(last_card_same_num==4)
	{
		if(HuMingSiGui)
		{
			score *= 2;
			hu_flag |= RH_MINGSIGUI;//明四归
		}
		else
		{
			score *= 4;
			hu_flag |= RH_ANSIGUI;//暗四归
		}
	}
	if(couple_num==7&&_cardInHand->size()==HAND_IN_CARD_NUM)
	{
		score *= 4;
		int j;
		if(zhong_num==2&&fa_num==2&&bai_num==2)
		{
			score *= 8;
			hu_flag |= RH_SANYUANQIDUI;//三元七对
		}
		if(four_num==1)
		{
			score *= 2;
			hu_flag |= RH_HAOHUAQIDUI;//豪华七对
		}
		else if(four_num==2)
		{
			score *= 8;
			hu_flag |= RH_CHAOHAOHUAQIDUI;//超豪华七对
		}
		else if(four_num==3)
		{
			score *= 32;
			hu_flag |= RH_CHAOCHAOHAOHUAQIDUI;//超超豪华七对
		}
		else
			hu_flag |= RH_QIDUI;//七对
	}
	else if(kind==ck_WU_TIAO||kind==ck_WU_TONG)
	{
	    if(_cardInHand->IsKaWuXing((Card_t)kind)) {
            score *= 4;
            hu_flag |= RH_KAWUXIN;
        }
	}
    
	kind_hu=hu_flag;
    
	if(rr_aim!=RA_NOTASK)
		task_check(hu_flag);
    
	return score;
}

int NetRRound::cal_times(CARD_KIND kind,CARD_KIND data[],int len)
{
	int i;
	unsigned char color;
	bool HuQingYiSe = true;
	bool HuMingSiGui = false;
	bool HuPengPengHu = false;
	unsigned char couple_num=0;
	unsigned char zhong_num=0;
	unsigned char fa_num=0;
	unsigned char bai_num=0;
	unsigned char four_num=0;
	unsigned char last_card_same_num=0;
	unsigned int  hu_flag = 0;
	long score = 1;
	color=kind/9;

	for(i=0;i<len;i++)
	{
		if(	data[i]/9 != color )
		{
			HuQingYiSe = false;
		}
		if( data[i]==kind )
		{
			last_card_same_num++;
		}

		int l_cards=1;
		for(int k=i+1;k<len;k++)
			if(data[i]==data[k] )
				l_cards++;
		if(l_cards==2||l_cards==4)
			couple_num++;
		if(l_cards==4)
			four_num++;

		if(data[i]==ZHONG)
		{
			zhong_num++;
		}
		else if(data[i]==FA)
		{
			fa_num++;
		}
		else if(data[i]==BAI)
		{
			bai_num++;
		}
	}
	if(len!=2)
	{
		int l_num;
		int l_couple=0;
		int l_len=len;
		for(int k=0;k<l_len;k++)
		{
			l_num=1;
			for(int t=k+1;t<l_len;t++)
				if(data[k]==data[t])
					l_num++;
			if(l_num==3)
				l_len -= 3;
			l_num=0;
			for(int t=0;t<l_len;t++)
				if(data[k]==data[t])
					l_num++;
			if(l_num==2)
				l_couple++;
		}
		if(l_len==2&&l_couple==2)
			HuPengPengHu = true;
	}
	else
		score *= 4;
	if(HuQingYiSe)
		score *= 4;
	if(HuPengPengHu)
		score *= 2;
	if(zhong_num==3&&fa_num==3&&bai_num==3)
		score *= 8;
	else if((zhong_num==3&&fa_num==3&&bai_num==2) ||
			(zhong_num==2&&fa_num==3&&bai_num==3) ||
			(zhong_num==3&&fa_num==2&&bai_num==3))
		score *= 4;
	if(last_card_same_num==4)
		score *= 4;
	if( couple_num==7 && len==HAND_IN_CARD_NUM )
	{
		score *= 4;
		int j;
		if(zhong_num==2&&fa_num==2&&bai_num==2)
			score *= 8;
		if(four_num==1)
			score *= 2;
		else if(four_num==2)
			score *= 8;
		else if(four_num==3)
			score *= 32;
	}
	else if(kind==ck_WU_TIAO||kind==ck_WU_TONG)
	{
		if(_cardInHand->IsKaWuXing((Card_t)kind)) {
            score *= 4;
        }
	}
	return score;
}

int NetRRound::cards_stable(CARD_KIND clist[],int len)
{
    SmartList cards;
    cards.len = len;
    for(int i=0;i<cards.len;i++) {
        cards.kind[i] = (Card_t)clist[i];
    }

    return cards.CardsStable(cards);
}

void NetRRound::task_check(unsigned int flag)
{
	if( !(flag&rr_aim) )
		rr_aim=0;
}

int NetRRound::hu_check(CARD_KIND newCard)
{
    SmartList cards(*_cardInHand);
    cards.len--;                     /*the last should not be included*/
    cards._Insert((Card_t)newCard);  /*the last inserted in order*/

    return cards.CardsStable();
}

void NetRRound::load(const SmartList &input,CARD_KIND output[]) {
    for(int i=0;i<input.len;i++) {
        output[i] = (CARD_KIND)input.kind[i];
    }
}

bool NetRRound::ting_check(int index,CARD_KIND cur_card,int kind,CARD_KIND rlist[])
{
    SmartList cards;
    cards.len = _cardInHand->size()-_cardInHand->active_place;
    for(int i=0;i<cards.len;i++) {
        cards.kind[i] = _cardInHand->at(_cardInHand->active_place+i)->kind;
    }

    cards._Displace(index-_cardInHand->active_place,(Card_t)kind);

    load(cards,rlist);

    return cards.CardsStable();
}

int NetRRound::judge_kou_cards(CARD_KIND card,int no,CARD_KIND otherHandedOut)
{
    SmartList newCards = _cardInHand->_Remove((Card_t)card);
    
	if(no==MIDDLE) {
		for(int i=0;i<newCards.len;i++) {
			for(int k=ck_YI_TIAO;k<=BAI;k++) {
                SmartList remain(*_cardInHand);
                remain._Displace(i,(Card_t)k);
				if(remain.CardsStable()) {
					return true;
                }
			}
        }
	} else {
		int index;
        
		for(int i=0;i<newCards.len;i++) {
			if(newCards.kind[i]==otherHandedOut) {
				index=i;
				break;
			}
		}
        
		for(int k=ck_YI_TIAO;k<=BAI;k++) {
            SmartList remain(*_cardInHand);
            remain._Displace(index,(Card_t)k);
            if(remain.CardsStable()) {
                return true;
            }
		}
	}
    
	return false;
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
unsigned int NetRRound::ming_check()
{
	unsigned int res =0;
	CARD_KIND cur_card/*,last_card*/;
	int active_len=_cardInHand->size()-_cardInHand->active_place;
	int index=_cardInHand->active_place;
	CARD_KIND temp_list[MAX_HANDIN_NUM];
	CARD_KIND last_card=ck_NOT_DEFINED;
	memset(temp_list,ck_NOT_DEFINED,MAX_HANDIN_NUM*sizeof(CARD_KIND));
	//memset(hu_NumForEveryCard,0,sizeof(int)*MAX_HANDIN_NUM);
	int m=0;
	hu_places_num=0;
	memset(hu_cards_num,0,sizeof(int)*MAX_HANDIN_NUM);
	memset(hu_cards,0xff,sizeof(CARD_KIND)*MAX_HANDIN_NUM*9);
	memset(huTiemsForEveryOne,0,sizeof(int)*MAX_HANDIN_NUM*9);
	for(int i=index;i<_cardInHand->size();i++)
	{
		m=0;
		if(_cardInHand->get_status(i)==sMING_KOU)
			continue;
		cur_card=(CARD_KIND)_cardInHand->get_kind(i);
		if(last_card==cur_card)
		{
			res |= ( ( res&( 1<<(i-1) ) )<<1 );
			hu_cards_num[i]=hu_cards_num[i-1];
			memcpy(hu_cards[i],hu_cards[i-1],sizeof(CARD_KIND)*9);
			memcpy(huTiemsForEveryOne[i],huTiemsForEveryOne[i-1],sizeof(int)*9);
			continue;
		}
		for(int k=ck_YI_TIAO;k<=BAI;k++)
		{
			if(ting_check(i,cur_card,k,temp_list))
			{
				res |= (1<<i);
				hu_cards[i][m]=CARD_KIND(k);
				hu_cards_num[i]++;
				int s_core=cal_times(CARD_KIND(k),temp_list,_cardInHand->size()-_cardInHand->active_place);
				for(int s_l=0;s_l<_cardInHand->active_place;s_l++)
					if( _cardInHand->get_kind(s_l)==CARD_KIND(k)&&_cardInHand->get_status(s_l)==sPENG)
					{
						s_core *= 2;
						break;
					}
				huTiemsForEveryOne[i][m++]=s_core;
			}
		}
		last_card=cur_card;
	}
	for(int i=0;i<MAX_HANDIN_NUM;i++)
		if(res&(1<<i))
			hu_places_num++;
	hu_places=res;
	return res;
}

unsigned char NetRRound::init(int card_array[],int len,int aim)
{
	int i;

	rr_aim = aim;
	rr_ting_flag = 0;
	card_score=0;
	hu_len=0;
	hu_places_num=0;
	memset(hu_cards_num,0,sizeof(int)*MAX_HANDIN_NUM);
	memset(hu_cards,0xff,sizeof(CARD_KIND)*MAX_HANDIN_NUM*9);

    _cardInHand->clear();
	_cardInHand->active_place =0;

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
	int card_num;
	for(int i=_cardInHand->active_place;i<_cardInHand->size();i++)
	{
		card_num=1;
		for(int k=i+1;k<_cardInHand->size();k++)
		{
			if(_cardInHand->get_kind(k)==_cardInHand->get_kind(i))
				card_num++;
		}
		if(card_num==4)
		{
			res |= a_SHOU_GANG;
			break;
		}
	}
	if( ( archive_ming_indexes=ming_check() )!=0 )
		res |= a_MING;
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

	if(rr_ting_flag==0)
	{
		int card_num;
		for(int i=_cardInHand->active_place;i<_cardInHand->size();i++)
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

		for(int i=0;i<_cardInHand->active_place;i++)
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
		if(rr_ting_flag==0 && !is_last_one)
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
        
        _cardInHand->delete_card(_cardInHand->active_place,3);
        _cardInHand->insert_card(node,3);
        _cardInHand->active_place += 3;
	} else if(act==a_MING_GANG) {
        node.status=sMING_GANG;

        _cardInHand->delete_card(_cardInHand->active_place,4);
        _cardInHand->insert_card(node,4);
		_cardInHand->active_place += 4;
	} else if(act==a_AN_GANG) {
        node.kind=CARD_UNKNOWN;
        node.status=sAN_GANG;

        _cardInHand->delete_card(_cardInHand->active_place,4);
        _cardInHand->insert_card(node,4);
		_cardInHand->active_place += 4;
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

        for(int i=_cardInHand->active_place;i<_cardInHand->size();i++) {
			if(_cardInHand->get_kind(i)==node.kind){
				_cardInHand->delete_card(i,2);
				break;
			}
        }

        _cardInHand->pop_back();
        _cardInHand->insert_card(node,3);
        _cardInHand->active_place += 3;
	}
	else if(act==a_KOU) {
        node.status=sMING_KOU;

		for(int i=_cardInHand->size()-1;i>=_cardInHand->active_place;i--)
			if(_cardInHand->get_status(i)==sMING_KOU) {
				node.kind=_cardInHand->get_kind(i);
				_cardInHand->delete_card(i,1);
				_cardInHand->insert_card(node,1);
                _cardInHand->active_place+=1;
				i++;
			}
	}
	else if(act==a_KOU_CANCEL) {
		node.status  = sFREE;
		node.canPlay = true;
        
		for(int i=_cardInHand->active_place-1;i>=0;i--) {
			if(_cardInHand->get_status(i)==sMING_KOU) {
				node.kind = _cardInHand->get_kind(i);
                
				_cardInHand->delete_card(i,1);
				_cardInHand->insert_card(node,1);
				_cardInHand->active_place-=1;
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

                _cardInHand->active_place--;
			} else if(_cardInHand->get_status(i)==sMING_KOU&& _cardInHand->get_kind(i)==node.kind) {
				_cardInHand->active_place--;
            }

			if(_cardInHand->get_kind(i)==node.kind)
				_cardInHand->delete_card(i,1);
		}
        _cardInHand->insert_card(node,4);
		_cardInHand->active_place += 4;
	}
	else if(act==a_AN_GANG) {
		_cardInHand->perform(aAN_GANG);
	}
	else if(act==a_SHOU_GANG) {
		int card_num;
		int i;
		for(i=_cardInHand->active_place;i<_cardInHand->size();i++)
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
        
		for(int k=_cardInHand->size();k>=_cardInHand->active_place;k--)
		{
			if(_cardInHand->get_kind(k)==_cardInHand->get_kind(i))
				_cardInHand->delete_card(k,1);
		}

		node.status  = sAN_GANG;
		node.canPlay = false;
        _cardInHand->insert_card(node,4);
		_cardInHand->active_place += 4;

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
        
		for (int i=_cardInHand->active_place;i<_cardInHand->size();i++) {
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

unsigned int NetRRound::get_aim()
{
	return rr_aim;
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
	rr_ting_flag=flag;
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
	return rr_ting_flag;
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
    card_list->atcvie_place = _cardInHand->active_place;
    
	return card_list;
}

bool NetRRound::get_Hu_Flag(unsigned int *hu_kind)
{
	*hu_kind=kind_hu;
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

