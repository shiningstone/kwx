
#include <stdlib.h>

#include "RaceType.h"
#include "Raction.h"

#include "NetRole.h"
#include "RoundManager.h"

RoundManager::RoundManager() {
    _lastWin.player = INVALID;

    _river = NULL;
	for(int i=0;i<TOTAL_CARD_NUM;i++) {
		_unDistributedCards[i]=i;
	}
    _distributedNum = 0;

    for(int i=0;i<PLAYER_NUMBER;i++) {
        _players[i] = NULL;
    }
    
    _logger = LOGGER_REGISTER("RoundManager");
}

RoundManager::~RoundManager() {
    delete _river;
    for(int i=0;i<PLAYER_NUMBER;i++) {
        delete _players[i];
    }
    
    LOGGER_DEREGISTER(_logger);
}

/***********************************************
        winner information
***********************************************/
PlayerDir_t RoundManager::GetLastWinner() {
    if( _lastWin.player==INVALID ) {
        LOGGER_WRITE("NETWORK: Request(last winner) not defined");
        _lastWin.player = MIDDLE;
    }
    return _lastWin.player;
}

void RoundManager::SetWin(WinKind_t kind,int player) {
    _lastWin.kind       = kind;
    _lastWin.player     = (PlayerDir_t)player;
}

void RoundManager::GetWin(WinInfo_t &info) {
    info.kind   = _lastWin.kind;
    info.player = _lastWin.player;
}

bool RoundManager::IsWinner(int no, int curPlayer, int FirstMingPlayer) {
	if((_lastWin.kind==SINGLE_WIN&&
            ((_lastWin.player==curPlayer && _lastWin.player!=no)||
            (_lastWin.player!=curPlayer && no!=_lastWin.player && no!=curPlayer)))
        ||(_lastWin.kind==NONE_WIN && FirstMingPlayer!=-1 && no!=FirstMingPlayer)) {
        return true;
    } else {
        return false;
    }
}

/***********************************************
        river information
***********************************************/
void RoundManager::RecordHandOut(int cardIdx) {
    RecordOutCard(_players[_curPlayer]->get_parter()->get_card_list()->data[cardIdx]);
    _lastHandedOutCard = _players[_curPlayer]->get_parter()->hand_out(cardIdx);
}

void RoundManager::RecordOutCard( Card card ) {
    _river->insertItem(card);

    LOGGER_WRITE("RIVER : ");
    char cards[84] = {0};
    int  i = 0;
    outCardNode *p = _river->head;
    while(p->pNext) {
        cards[i++] = p->pNext->data.kind;
        p = p->pNext;
    }
    LOGGER_WRITE_ARRAY(cards,i);
}

void RoundManager::RenewOutCard() {
    delete _river;
	_river = new outCardList;
}

/***********************************************
        player information
***********************************************/
#include "NetMe.h"
#include "NetPlayer.h"
#include "NetRaceRound.h"

void RoundManager::Init() {
    InitPlayers();
}

void RoundManager::InitPlayers() {
    _cardHolders[0] = new CardHolder();
    _cardHolders[1] = new CardHolder();
    _cardHolders[2] = new CardHolder();

	_players[0] = new NetPlayer();
	_players[1] = new NetMe();
	_players[2] = new NetPlayer();

	_players[0]->set_parter( new NetRRound() );
	_players[1]->set_parter( new NetRRound() );
	_players[2]->set_parter( new NetRRound() );

	_players[0]->get_parter()->set_role_type( INTERNET_PLAYER );
	_players[1]->get_parter()->set_role_type( SINGLE_BOADR_ME );
	_players[2]->get_parter()->set_role_type( INTERNET_PLAYER );
}

void RoundManager::_GenerateIds(int ids[]) {
    ids[0]=rand()%16;
    ids[1]=17;
    
    do {
        ids[2]=rand()%16;
    } while( ids[2]==ids[0] );
}

void RoundManager::LoadPlayerInfo() {
    Database *database = Database::getInstance();
    
    int  ids[3] = {0};
    _GenerateIds(ids);
    
	for(int dir=0;dir<3;dir++)
	{	
        UserProfile_t profile = {0};
        database->GetUserProfile(ids[dir],profile);
        _cardHolders[dir]->Set(&profile);
	}
}

bool RoundManager::IsTing(int id) {
    return _players[id]->get_parter()->get_ting_status();
}

int RoundManager::Shuffle() {
	for(int j=0;j<2;j++) {//α�����������
		for(int i=0;i<TOTAL_CARD_NUM;i++) {
			int tmp = _unDistributedCards[i];
			int cur = rand()%TOTAL_CARD_NUM;
			_unDistributedCards[i] = _unDistributedCards[cur];
			_unDistributedCards[cur] = tmp;
		}
	}
	////////////////////����/////////////////////////
	//[0,3]һ�� [4,7]���� [8,11]���� [12,15]���� [16,19]���� [20,23]���� [24,27]���� [28,31]���� [32,35]���� 
	//[36,39]һͲ [40,43]��Ͳ [44,47]��Ͳ [48,51]��Ͳ [52,55]��Ͳ [56,59]��Ͳ [60,63]��Ͳ [64,67]��Ͳ [68,71]��Ͳ
	//[72,75]���� [76,79]���� [80,83]�װ�
	int oneSeq[14]={0,0,0,8,9,10,12,16,20,24,28,32,36,39};
	int twoSeq[13]={36,36,36,40,40,44,44,48,56,60,64,68,68};
	int threeSeq[13]={36,36,36,40,40,44,44,48,56,60,64,68,68};
	_unDistributedCards[40]=0;
	_unDistributedCards[41]=4;
	_unDistributedCards[42]=8;
	_unDistributedCards[43]=12;
	_unDistributedCards[44]=16;
	_unDistributedCards[45]=20;
	_unDistributedCards[46]=24;
	_unDistributedCards[47]=80;
	_unDistributedCards[48]=80;
	_unDistributedCards[49]=80;
	_unDistributedCards[50]=80;
	_unDistributedCards[51]=80;
	int c=0;
	for(int a=0;a<14;a++)
	{
		if(oneSeq[a]<0||oneSeq[a]>83)
		{
			CCLOG("ERROR CARDS");
			return 0;
		}
		_unDistributedCards[c++]=oneSeq[a];
	}
	for(int a=0;a<13;a++)
	{
		if(twoSeq[a]<0||twoSeq[a]>83)
		{
			CCLOG("ERROR CARDS");
			return 0;
		}
		_unDistributedCards[c++]=twoSeq[a];
	}
	for(int a=0;a<13;a++)
	{
		if(threeSeq[a]<0||threeSeq[a]>83)
		{
			CCLOG("ERROR CARDS");
			return 0;
		}
		_unDistributedCards[c++]=threeSeq[a];
	}


    LOGGER_WRITE("NETWORK: Shuffle should be executed at the server, and it is more reasonable to hide the card sequence to clients");
    char p[TOTAL_CARD_NUM] = {0};
    for(int i=0;i<TOTAL_CARD_NUM;i++) {
        p[i] = (_unDistributedCards[i])/4;
    }
    LOGGER_WRITE_ARRAY(p,TOTAL_CARD_NUM);

    _isGangAsking = false;
    _isQiangGangAsking = false;
    _isDoubleHuAsking = false;
    _isCardFromOthers = false;
    _firstMingNo = INVALID;
    _qiangGangTargetNo = INVALID;
    _otherOneForDouble = INVALID;
    _isWaitDecision = false;
    _isGangHua = false;
    
    CancelEffectCard();

    _lastAction = INVALID;
    _lastActionWithGold = INVALID;
    _lastActionSource = INVALID;

    _curPlayer = GetLastWinner();

    return 0;
}

int RoundManager::NotifyStart() {
    LOGGER_WRITE("NETWORK %s : send notification to server",__FUNCTION__);
    return 0;
}

bool RoundManager::GetReadyStatus(int seatId) {
    LOGGER_WRITE("NETWORK : %s %d",__FUNCTION__,seatId);
    return true;
}

bool RoundManager::WaitUntilAllReady() {
    LOGGER_WRITE("NETWORK : %s",__FUNCTION__);
    while( !GetReadyStatus(0) || !GetReadyStatus(2) ) {
        //delay
    }

    return true;
}

int RoundManager::AllowMovement() {
    LOGGER_WRITE("enable reaction to player's movement",__FUNCTION__);
    return 0;
}

bool RoundManager::WaitForDistribute() {
    LOGGER_WRITE("NETWORK : %s",__FUNCTION__);
    return true;
}

bool RoundManager::WaitForAction() {
    LOGGER_WRITE("NETWORK : %s",__FUNCTION__);
    return true;
}

/****************************************
        effect card
****************************************/
void RoundManager::CancelEffectCard() {
    _curEffectCard.kind = ck_NOT_DEFINED;
    _curEffectCard.status = c_NOT_DEFINDED;
}

void RoundManager::SetEffectCard(int kind,int status) {
    _curEffectCard.kind = (CARD_KIND)kind;
    _curEffectCard.status = (CARD_STATUS)status;
}

bool RoundManager::IsCurEffectCard(Card card) {
    if(card.kind==_curEffectCard.kind && card.status==_curEffectCard.status) {
        return true;
    } else {
        return false;
    }
}
/****************************************
        card handler
****************************************/
int RoundManager::FindGangCards(int dir,int cards[4]) {
    auto list = _players[dir]->get_parter()->get_card_list();
    int matchCardNum = 0;
    int firstMatchCard = 0;
    
    for(int i=list->atcvie_place; i<list->len; i++) {
        for(int j=i+1; j<list->len; j++) {
            if(list->data[i].kind==list->data[j].kind) {
                matchCardNum++;
                if(matchCardNum==3) {
                    firstMatchCard = i;
                    break;
                }
            }
        }
    }

    int idx = 0;
    for(int i=firstMatchCard; i<list->len; i++) {
        if(list->data[firstMatchCard].kind==list->data[i].kind) {
            cards[idx++] = i;
        }
    }

    return 0;
}

