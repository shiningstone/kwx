
#include <stdlib.h>

#include "RaceType.h"
#include "Role.h"
#include "Raction.h"

#include "RoundManager.h"

#define INVALID -1

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
int RoundManager::GetLastWinner() {
    if( _lastWin.player==INVALID ) {
        LOGGER_WRITE("NETWORK: Request(last winner) not defined");
        _lastWin.player = 1;
    }
    return _lastWin.player;
}

void RoundManager::SetWin(WinKind_t kind,int player) {
    _lastWin.kind       = kind;
    _lastWin.player     = player;
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
#include "./../Me.h"
#include "NetPlayer.h"
#include "NetRaceRound.h"

void RoundManager::InitPlayers() {
	_players[0] = new NetPlayer();
	_players[1] = new Me();
	_players[2] = new NetPlayer();

	_players[0]->set_parter( new NetRRound() );
	_players[1]->set_parter( new NetRRound() );
	_players[2]->set_parter( new NetRRound() );

	_players[0]->get_parter()->set_role_type( INTERNET_PLAYER );
	_players[1]->get_parter()->set_role_type( SINGLE_BOADR_ME );
	_players[2]->get_parter()->set_role_type( INTERNET_PLAYER );
}

bool RoundManager::IsTing(int id) {
    return _players[id]->get_parter()->get_ting_status();
}

int RoundManager::Shuffle() {
	for(int j=0;j<2;j++) {//伪随机数列生成
		for(int i=0;i<TOTAL_CARD_NUM;i++) {
			int tmp = _unDistributedCards[i];
			int cur = rand()%TOTAL_CARD_NUM;
			_unDistributedCards[i] = _unDistributedCards[cur];
			_unDistributedCards[cur] = tmp;
		}
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
    
    _curEffectCardKind = INVALID;
    _curEffectCardStatus = INVALID;

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

