
#include <stdlib.h>

#include "RaceType.h"
#include "Role.h"
#include "Raction.h"

#include "RoundManager.h"

#define INVALID -1

RoundManager::RoundManager() {
    _lastWin.player = INVALID;
    _river = NULL;
    
    _logger = LOGGER_REGISTER("RoundManager");
}

RoundManager::~RoundManager() {
    if(_river) {
        delete _river;
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
    if(_river) {
        delete _river;
    }

	_river = new outCardList;
}

void RoundManager::SetPlayers(Role *players[]) {
	for(int i=0;i<PLAYER_NUMBER;i++) {
		_players[i] = players[i];
	}
}

int RoundManager::Shuffle(int *cardSeq) {
    LOGGER_WRITE("NETWORK: Shuffle should be executed at the server, and it is more reasonable to hide the card sequence to clients");

    for(int i=0;i<TOTAL_CARD_NUM;i++) {
		cardSeq[i]=i;
	}

	for(int j=0;j<2;j++) {//伪随机数列生成
		for(int i=0;i<TOTAL_CARD_NUM;i++) {
			int tmp = cardSeq[i];
			int cur = rand()%TOTAL_CARD_NUM;
			cardSeq[i] = cardSeq[cur];
			cardSeq[cur] = tmp;
		}
	}

    char p[TOTAL_CARD_NUM] = {0};
    for(int i=0;i<TOTAL_CARD_NUM;i++) {
        p[i] = (cardSeq[i])/4;
    }
    LOGGER_WRITE_ARRAY(p,TOTAL_CARD_NUM);

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

