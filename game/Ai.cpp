
#include "CardCollection.h"
#include "Player.h"
#include "RaceLayer.h"
#include "RoundManager.h"
#include "Ai.h"

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

    int curPlayer     = _roundManager->_curPlayer;
    CardInHand *cards = _roundManager->_players[curPlayer]->_cards;

    _roundManager->_players[(curPlayer+1)%3]->_cards->collect_ming_info();
	_roundManager->_players[(curPlayer+1)%3]->_cards->get_hu_cards(target1,len1);
    _roundManager->_players[(curPlayer+2)%3]->_cards->collect_ming_info();
	_roundManager->_players[(curPlayer+2)%3]->_cards->get_hu_cards(target2,len2);

    for(int i=cards->FreeStart;i<cards->size();i++) {
		int time = res->list[cards->get_kind(i)].same_times++;
		res->list[cards->get_kind(i)].place[time]=i;
	}

	/*init hu target*/
	if( !_roundManager->IsTing(curPlayer) ) {
		res->target = _roundManager->_players[curPlayer]->_cards->assess_aim();
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

    int curPlayer = _roundManager->_curPlayer;
    
	if(curPlayer!=MIDDLE) {
		collect_resources(s_res,list1,list2,&len1,&len2);
		_roundManager->_players[curPlayer]->set_robot_hu_target(s_res->target);
	}

    if( !_roundManager->IsTing(curPlayer) ) {
		index = _roundManager->_players[curPlayer]->chose_card(
            s_res,TOTAL_CARD_NUM - _roundManager->_distributedNum,list1,list2,len1,len2);

		if( index==INVALID || index>_roundManager->_players[curPlayer]->_cards->last() ) {
			index=_roundManager->_players[curPlayer]->_cards->last();
		}
        
		if(s_res->hu_nums>=6 
            && _roundManager->_actionToDo==a_MING 
            && !_roundManager->IsTing(curPlayer) ) {
			kouRequest = true;
		}
	} else {
		index = _roundManager->_players[curPlayer]->_cards->last();
    }

    return index;
}

/*************************************
        round info
*************************************/
void Ai::UpdateAtFirstRound(int &actionToDo) {
    if(_roundManager->_MODE==LOCAL_GAME) {
        actionToDo = _roundManager->_players[_roundManager->_curPlayer]->ActiontodoCheckAgain();
    }
}

/*************************************
        GAME CALCULATION
*************************************/
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

long Ai::sum_up_score(unsigned int fan) {
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
        if(roundManager==NULL) {
            return NULL;
        }

        _instance = new Ai(roundManager);
    }

    return _instance;
}

void Ai::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

