
#include "NetRole.h"
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

Card_t Ai::KouCardKind(int gIdx) {
    return (Card_t)_bufKouCards.group[gIdx].card.kind;
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

