
#include "NetRole.h"
#include "NetRaceLayer.h"
#include "RoundManager.h"
#include "Ai.h"

void Ai::_CollectResouce(HAH *res) {
	memset(res,0,sizeof(HAH));
	memset(res->card_in_river,ck_NOT_DEFINED,sizeof(CARD_KIND)*TOTAL_CARD_NUM);

    res->reserved_card_num = TOTAL_CARD_NUM - _roundManager->_distributedNum;
    
	CARD s_card;
	int i = 1;
	while(_roundManager->_river->getCard(s_card,i++)==true)
		res->card_in_river[res->river_len++]=s_card.kind;
}

void Ai::collect_resources(HAH *res,CARD_KIND target1[],CARD_KIND target2[],int *len1,int *len2)
{
	_CollectResouce(res);

	_roundManager->_players[(_roundManager->_curPlayer+1)%3]->get_parter()->get_hu_cards(target1,len1);
	_roundManager->_players[(_roundManager->_curPlayer+2)%3]->get_parter()->get_hu_cards(target2,len2);

    for(int i=_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->atcvie_place;
        i<_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->len;i++) {
		int time = res->list[_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->data[i].kind].same_times++;
		res->list[_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->data[i].kind].place[time]=i;
	}

	/*init hu target*/
	if( !_roundManager->IsTing(_roundManager->_curPlayer) ) {
		_roundManager->_players[_roundManager->_curPlayer]->init_target(&res->target,*len1,*len2);
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
    
	if(_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_role_type()==SINGLE_BOARD_ROBOT) {
		collect_resources(s_res,list1,list2,&len1,&len2);
		_roundManager->_players[_roundManager->_curPlayer]->set_robot_hu_target(s_res->target);
	}
    
	if(_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_role_type()==INTERNET_PLAYER) {
		collect_resources(s_res,list1,list2,&len1,&len2);
		_roundManager->_players[_roundManager->_curPlayer]->set_robot_hu_target(s_res->target);
	}

    if( !_roundManager->IsTing(_roundManager->_curPlayer) ) {
		index = _roundManager->_players[_roundManager->_curPlayer]->chose_card(
            s_res,TOTAL_CARD_NUM - _roundManager->_distributedNum,list1,list2,len1,len2);

		if( index==-1 || index>_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->len-1 ) {
			index=_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->len-1;
		}
        
		if(s_res->hu_nums>=6 
            && _roundManager->_actionToDo==a_MING 
            && !_roundManager->IsTing(_roundManager->_curPlayer) ) {
			kouRequest = true;
		}
	} else {
		index = _roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->len-1;
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

void Ai::SwitchGroupStatus(int group,CARD_ARRAY *cards) {
    if(KouCardStatus(group)==c_MING_KOU) {
        SetKouCardStatus(group,c_KOU_ENABLE);
        cards->data[KouCardIndex(group,0)].status=c_KOU_ENABLE;
        cards->data[KouCardIndex(group,1)].status=c_KOU_ENABLE;
        cards->data[KouCardIndex(group,2)].status=c_KOU_ENABLE;
    }
    else if(KouCardStatus(group)==c_KOU_ENABLE) {
        SetKouCardStatus(group,c_MING_KOU);
        cards->data[KouCardIndex(group,0)].status=c_MING_KOU;
        cards->data[KouCardIndex(group,1)].status=c_MING_KOU;
        cards->data[KouCardIndex(group,2)].status=c_MING_KOU;
    }
}

void Ai::Refresh(CARD_ARRAY *cards) {
    for( int group=0; group<KouCardGroupNum(); group++ ) {
        if(KouCardStatus(group)!=c_MING_KOU) {
            if(_roundManager->_players[MIDDLE]->get_parter()->judge_kou_cards(
                (CARD_KIND)KouCardKind(group), MIDDLE, (CARD_KIND)_roundManager->_otherHandedOut))
            {
                cards->data[KouCardIndex(group,0)].status=c_KOU_ENABLE;
                cards->data[KouCardIndex(group,1)].status=c_KOU_ENABLE;
                cards->data[KouCardIndex(group,2)].status=c_KOU_ENABLE;
                SetKouCardStatus(group,c_KOU_ENABLE);
            } else {
                cards->data[KouCardIndex(group,0)].status=c_FREE;
                cards->data[KouCardIndex(group,1)].status=c_FREE;
                cards->data[KouCardIndex(group,2)].status=c_FREE;
                SetKouCardStatus(group,c_FREE);
            }
        }
    }
}

Card_t Ai::KouCardKind(int gIdx) {
    return (Card_t)_bufKouCards.group[gIdx].card.kind;
}

void Ai::KouCardCheck(PlayerDir_t dir) {
	CARD_ARRAY *list = _roundManager->_players[dir]->get_parter()->get_card_list();

	ClearKouCardInfo();
        
	for(int i=list->atcvie_place; i<list->len; i++){
	    auto curKind = list->data[i].kind;
        
        if( !IsKouCardInclude((Card_t)curKind) ) {
            int cardIdx[4] = {-1,-1,-1,-1};
            
            if(_FindCards(cardIdx, list, curKind)==3 
                &&_roundManager->_players[dir]->get_parter()->judge_kou_cards(curKind,dir,(CARD_KIND)_roundManager->_otherHandedOut)) {
                
                AddKouCardGroup((Card_t)curKind,cardIdx);
                
                list->data[cardIdx[0]].status=c_KOU_ENABLE;
                list->data[cardIdx[1]].status=c_KOU_ENABLE;
                list->data[cardIdx[2]].status=c_KOU_ENABLE;
            }
        }
	}
}

int Ai::_FindCards(int idx[],CARD_ARRAY *list,CARD_KIND kind) {
    int num = 0;
    
    for(int i=list->atcvie_place; i<list->len; i++) {   
        if(list->data[i].kind==kind) {
            idx[num++] = i;
        }
    }

    return num;
}

void Ai::MingKouChoose(PlayerDir_t dir) {
	auto list=_roundManager->_players[dir]->get_parter()->get_card_list();
    
	for(int i=0;i<KouCardGroupNum();i++)
	{
		if(_roundManager->_players[dir]->get_parter()->judge_kou_cards(
            (CARD_KIND)KouCardKind(i),dir,(CARD_KIND)_roundManager->_otherHandedOut))
		{
			for(int b=0;b<3;b++)
			{
				list->data[KouCardIndex(i,b)].status=c_MING_KOU;
			}
		}
	}
}

/*************************************
        card process
*************************************/
Card_t Ai::FindGangCards(int cardIdx[4],CARD_ARRAY *cards,Card_t target,int gangType,bool isTing,bool isCardFromOthers) {
    if( gangType & a_AN_GANG || gangType & a_SHOU_GANG ) {
        if(!isTing) {
            /*BUG here : only the first group can be found*/
            for(int i=cards->atcvie_place; i<cards->len; i++) {
                int matchCardNum = 0;
                cardIdx[0]       = i;
                
                for(int j=i+1; j<cards->len; j++) {
                    if(cards->data[i].kind==cards->data[j].kind) {
                        matchCardNum++;
                        cardIdx[matchCardNum] = j;
                        
                        if(matchCardNum==3) {
                            return (Card_t)cards->data[i].kind;
                        }
                    }
                }
            }
        } else {
            cardIdx[3] = cards->len-1;
        
            int p = 0;
            for(int i=0; i<cards->atcvie_place; i++){
                if(cards->data[i].kind==cards->data[cardIdx[3]].kind) {
                    cardIdx[p++]=i;
                    if(p==3) {
                        return (Card_t)cards->data[i].kind;
                    }
                }
            }
        }
    } else {
		int last = isCardFromOthers ? (cards->len) : (cards->len-1);

        int matchCardNum = 0;
		for(int i=0;i<last;i++) {
			if(target==cards->data[i].kind) {
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

Card_t Ai::FindPengCards(int cardIdx[2],CARD_ARRAY *cards,Card_t target) {
    int matchNum = 0;
    
    for(int i=cards->atcvie_place;i<cards->len;i++) {
        if(target==(Card_t)cards->data[i].kind) {
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

