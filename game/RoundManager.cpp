
#include <stdlib.h>

#include "RaceType.h"

#include "RoundManager.h"

#include "./../utils/LogManager.h"

RoundManager::RoundManager() {
    _logger = LOGGER_REGISTER("RoundManager");
}

RoundManager::~RoundManager() {
    LOGGER_DEREGISTER(_logger);
}

int RoundManager::GetLastWinner() {
    LOGGER_WRITE("NETWORK: Request(last winner) not defined");
    _lastWinner = 1;
    return _lastWinner;
}

int RoundManager::Shuffle(int *cardSeq) {
    LOGGER_WRITE("NETWORK: Shuffle should be executed at the server");

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
    LOGGER_WRITE("NETWORK : send notification to server",__FUNCTION__);
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
