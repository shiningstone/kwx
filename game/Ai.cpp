
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
    LOGGER_WRITE("%s",__FUNCTION__);

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

/*************************************
        singleton
*************************************/
Ai* Ai::_instance = NULL;

Ai *Ai::GetInstance(RoundManager *roundManager) {
    if (_instance==NULL) {
        _instance = new Ai(roundManager);
    }

    return _instance;
}

void Ai::DestroyInstance() {
    delete _instance;
    _instance = NULL;
}

