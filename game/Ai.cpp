
#include "NetRole.h"
#include "NetRaceLayer.h"
#include "RoundManager.h"
#include "Ai.h"

void Ai::init_target(CardInHand *list,ROBOT_TARGET *target,int hu_len1,int hu_len2)
{
	int couples=0;
	int color;
	int same_color_num=0;
	int color_num;

	for(int i=0;i<3;i++)
	{
		color=i;
		color_num=0;
        
		for(int j=0;j<list->size();j++)
			if(list->get_kind(j)/9==color)
				color_num++;
            
		if(color_num>9)
			break;
	}
	if(color_num>=9)
	{
		same_color_num=color_num;
		for(int j=0;j<list->FreeStart;j++)
			if(list->get_kind(j)/9!=color)
			{
				same_color_num=0;
				break;
			}
	}
	for(int i=list->FreeStart;i<list->size();i++)
	{
		int nums=1;
		for(int k=i+1;k<list->size();k++)
			if(list->get_kind(i)==list->get_kind(k) &&
			 list->get_status(i)==sFREE && sFREE == list->get_status(k))
				nums++;
		if(nums==2||nums==4)
			couples++;
	}
	if(list->FreeStart==0&&couples>=5)
		*target=SEVEN_COUPLES_TARGET;
	else if(same_color_num>=9&&color==0)
		*target=SAME_TIAO_TARGET;
	else if(same_color_num>=9&&color==1)
		*target=SAME_TONG_TARGET;
	else if(couples>3)
		*target=FOUR_PENG_TARGET;
	else
		*target=PI_HU_TARGET;
}

void Ai::_CollectResouce(HAH *res) {
	memset(res,0,sizeof(HAH));
	memset(res->card_in_river,ck_NOT_DEFINED,sizeof(CARD_KIND)*TOTAL_CARD_NUM);

    res->reserved_card_num = TOTAL_CARD_NUM - _roundManager->_distributedNum;
    
	CARD s_card;

    for(int i=0;i<_roundManager->_gRiver->size();i++) {
		res->card_in_river[res->river_len++] = (CARD_KIND)_roundManager->_gRiver->get_kind(i);
    }
}

void Ai::collect_resources(HAH *res,CARD_KIND target1[],CARD_KIND target2[],int *len1,int *len2)
{
	_CollectResouce(res);

	_roundManager->_players[(_roundManager->_curPlayer+1)%3]->get_parter()->get_hu_cards(target1,len1);
	_roundManager->_players[(_roundManager->_curPlayer+2)%3]->get_parter()->get_hu_cards(target2,len2);

    CardInHand *cards = _roundManager->_players[_roundManager->_curPlayer]->_cardsInHand;
    
    for(int i=cards->FreeStart; i<cards->size();i++) {
		int time = res->list[cards->get_kind(i)].same_times++;
		res->list[cards->get_kind(i)].place[time]=i;
	}

	/*init hu target*/
	if( !_roundManager->IsTing(_roundManager->_curPlayer) ) {
        auto list = _roundManager->_players[_roundManager->_curPlayer]->_cardsInHand;
        init_target(list,&res->target,*len1,*len2);
    }
}

int Ai::ChooseWorstCard(bool &kouRequest) {
    HAH *s_res = new HAH;
	int index;
    
	CARD_KIND list1[9];
	CARD_KIND list2[9];
	int len1;
	int len2;

    kouRequest = false;
    
	if(_roundManager->_curPlayer!=MIDDLE) {
		collect_resources(s_res,list1,list2,&len1,&len2);
		_roundManager->_players[_roundManager->_curPlayer]->set_robot_hu_target(s_res->target);
	}

    if( !_roundManager->IsTing(_roundManager->_curPlayer) ) {
		index = _roundManager->_players[_roundManager->_curPlayer]->chose_card(
            s_res,TOTAL_CARD_NUM - _roundManager->_distributedNum,list1,list2,len1,len2);

		if( index==-1 
                || index > _roundManager->_players[_roundManager->_curPlayer]->_cardsInHand->size()-1 ) {
			index = _roundManager->_players[_roundManager->_curPlayer]->_cardsInHand->size()-1;
		}
        
		if(s_res->hu_nums>=6 
            && _roundManager->_actionToDo==a_MING 
            && !_roundManager->IsTing(_roundManager->_curPlayer) ) {
			kouRequest = true;
		}
	} else {
		index = _roundManager->_players[_roundManager->_curPlayer]->_cardsInHand->size()-1;
    }

    return index;
}

/*************************************
        kou card info
*************************************/
void Ai::ClearKouCardInfo() {
    memset(&_bufKouCards,0,sizeof(KouCardInfo_t));
}

void Ai::AddKouCardGroup(Card_t kind,int *idx) {
    _bufKouCards.group[_bufKouCards.num].card.kind   = (CARD_KIND)kind;
    _bufKouCards.group[_bufKouCards.num].card.status = c_KOU_ENABLE;

    _bufKouCards.group[_bufKouCards.num].idxInHand[0] = idx[0];
    _bufKouCards.group[_bufKouCards.num].idxInHand[1] = idx[1];
    _bufKouCards.group[_bufKouCards.num].idxInHand[2] = idx[2];

    _bufKouCards.num++;
}

bool Ai::IsKouCardInclude(Card_t kind) {
    for(int i=0;i<_bufKouCards.num;i++) {
        if(_bufKouCards.group[i].card.kind==kind) {
            return true;
        }
    }

    return false;
}

int  Ai::KouCardGroupNum() {
    return _bufKouCards.num;
}

int  Ai::KouCardIndex(int gIdx,int cIdx) {
    return _bufKouCards.group[gIdx].idxInHand[cIdx];
}

CARD_STATUS Ai::KouCardStatus(int gIdx) {
    return _bufKouCards.group[gIdx].card.status;
}

void Ai::SetKouCardStatus(int gIdx,CARD_STATUS status) {
    _bufKouCards.group[gIdx].card.status = status;
}

void Ai::SwitchGroupStatus(int group,CardInHand *cards) {
    if(KouCardStatus(group)==c_MING_KOU) {
        SetKouCardStatus(group,c_KOU_ENABLE);
        cards->set_status(KouCardIndex(group,0),sKOU_ENABLE);
        cards->set_status(KouCardIndex(group,1),sKOU_ENABLE);
        cards->set_status(KouCardIndex(group,2),sKOU_ENABLE);
    }
    else if(KouCardStatus(group)==c_KOU_ENABLE) {
        SetKouCardStatus(group,c_MING_KOU);
        cards->set_status(KouCardIndex(group,0),sMING_KOU);
        cards->set_status(KouCardIndex(group,1),sMING_KOU);
        cards->set_status(KouCardIndex(group,2),sMING_KOU);
    }
}

void Ai::Refresh(CardInHand *cards) {
    for( int group=0; group<KouCardGroupNum(); group++ ) {
        if(KouCardStatus(group)!=c_MING_KOU) {
            if(_roundManager->_players[MIDDLE]->get_parter()->judge_kou_cards(
                (CARD_KIND)KouCardKind(group), MIDDLE, (CARD_KIND)_roundManager->_otherHandedOut))
            {
                cards->set_status(KouCardIndex(group,0),sKOU_ENABLE);
                cards->set_status(KouCardIndex(group,1),sKOU_ENABLE);
                cards->set_status(KouCardIndex(group,2),sKOU_ENABLE);
                SetKouCardStatus(group,c_KOU_ENABLE);
            } else {
                cards->set_status(KouCardIndex(group,0),sFREE);
                cards->set_status(KouCardIndex(group,1),sFREE);
                cards->set_status(KouCardIndex(group,2),sFREE);
                SetKouCardStatus(group,c_FREE);
            }
        }
    }
}

Card_t Ai::KouCardKind(int gIdx) {
    return (Card_t)_bufKouCards.group[gIdx].card.kind;
}

void Ai::KouCardCheck(PlayerDir_t dir) {
	CardInHand *list = _roundManager->_players[dir]->_cardsInHand;

	ClearKouCardInfo();
        
	for(int i=list->FreeStart; i<list->size(); i++){
	    auto curKind = list->get_kind(i);
        
        if( !IsKouCardInclude((Card_t)curKind) ) {
            int cardIdx[4] = {-1,-1,-1,-1};
            
            if(_FindCards(cardIdx, list, curKind)==3 
                    &&_roundManager->_players[dir]->get_parter()->judge_kou_cards((CARD_KIND)curKind,dir,(CARD_KIND)_roundManager->_otherHandedOut)) {
                
                AddKouCardGroup((Card_t)curKind,cardIdx);

                list->set_status(cardIdx[0],sKOU_ENABLE);
                list->set_status(cardIdx[1],sKOU_ENABLE);
                list->set_status(cardIdx[2],sKOU_ENABLE);
            }
        }
	}
}

int Ai::_FindCards(int idx[],CardInHand *list,Card_t kind) {
    int num = 0;
    
    for(int i=list->FreeStart; i<list->size(); i++) {   
        if(list->get_kind(i)==kind) {
            idx[num++] = i;
        }
    }

    return num;
}

void Ai::MingKouChoose(PlayerDir_t dir) {
	auto list = _roundManager->_players[dir]->_cardsInHand;
    
	for(int i=0;i<KouCardGroupNum();i++)
	{
		if(_roundManager->_players[dir]->get_parter()->judge_kou_cards(
            (CARD_KIND)KouCardKind(i),dir,(CARD_KIND)_roundManager->_otherHandedOut))
		{
			for(int b=0;b<3;b++)
			{
			    list->set_status(KouCardIndex(i,b),sMING_KOU);
			}
		}
	}
}

/*************************************
        card process
*************************************/
Card_t Ai::FindGangCards(int cardIdx[4],CardInHand *cards,Card_t target,int gangType,bool isTing,bool isCardFromOthers) {
    if( gangType & a_AN_GANG || gangType & a_SHOU_GANG ) {
        if(!isTing) {
            /*BUG here : only the first group can be found*/
            for(int i=cards->FreeStart; i<cards->size(); i++) {
                int matchCardNum = 0;
                cardIdx[0]       = i;
                
                for(int j=i+1; j<cards->size(); j++) {
                    if(cards->get_kind(i)==cards->get_kind(j)) {
                        matchCardNum++;
                        cardIdx[matchCardNum] = j;
                        
                        if(matchCardNum==3) {
                            return cards->get_kind(i);
                        }
                    }
                }
            }
        } else {
            cardIdx[3] = cards->size()-1;
        
            int p = 0;
            for(int i=0; i<cards->FreeStart; i++){
                if(cards->get_kind(i)==cards->get_kind(cardIdx[3])) {
                    cardIdx[p++]=i;
                    if(p==3) {
                        return cards->get_kind(i);
                    }
                }
            }
        }
    } else {
		int last = isCardFromOthers ? (cards->size()) : (cards->size()-1);

        int matchCardNum = 0;
		for(int i=0;i<last;i++) {
			if(target==cards->get_kind(i)) {
                cardIdx[matchCardNum++] = i;
                
                if(matchCardNum==3) {
                    return target;
                }
			}
		}
    }

    return CARD_UNKNOWN;
}

int Ai::ReChooseAfterGang(int chosen,int gangIdx[3],bool isCardFromOthers,int gang4) {
    int newChosen = 0;

    if(!isCardFromOthers) {
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

Card_t Ai::FindPengCards(int cardIdx[2],CardInHand *cards,Card_t target) {
    int matchNum = 0;
    
    for(int i=cards->FreeStart;i<cards->size();i++) {
        if(target==cards->get_kind(i)) {
            cardIdx[matchNum++] = i;
            if(matchNum==2) {
                return target;
            }
        }
    }

    return CARD_UNKNOWN;
}

int Ai::ReChooseAfterPeng(int chosen,int pengIdx[2]) {
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

/*************************************
        round info
*************************************/
void Ai::UpdateAtFirstRound(int &actionToDo) {
    if(_roundManager->_MODE==LOCAL_GAME) {
        actionToDo = _roundManager->_players[_roundManager->_curPlayer]->get_parter()->ActiontodoCheckAgain();
    }
}

/*************************************
        singleton
*************************************/
Ai* Ai::_instance = NULL;

Ai::Ai(RoundManager *roundManager) {
    _roundManager = roundManager;
}

Ai::~Ai() {
}

Ai *Ai::getInstance(RoundManager *roundManager) {
    if (_instance==NULL) {
        _instance = new Ai(roundManager);
    }

    return _instance;
}

void Ai::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

