
#include "CardCollection.h"
#include "Player.h"
#include "RoundManager.h"
#include "RaceLayer.h"

#include "StrategyRm.h"
#include "StrategyRmConcrete.h"
/*************************************
        local strategy
*************************************/
StrategyLocalRM::StrategyLocalRM(RoundManager *rm)
    :StrategyRm(rm) {
    PREMIUM_LEAST = 200;
}

void StrategyLocalRM::update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive) {
    int gold[3] = {0};
    CalculateGold(gold,GoldWinner,Gold_kind,whoGive);
    _rm->update_gold(gold);
}

/*************************************
        network strategy
*************************************/
StrategyNetworkRM::StrategyNetworkRM(RoundManager *rm)
    :StrategyRm(rm)
{}

/*************************************
        support
*************************************/
void StrategyLocalRM::CalcAnGangGold(int winner,int gold[3],int continueGang) {
    gold[winner]       = 4*PREMIUM_LEAST*(continueGang);
    gold[(winner+1)%3] = -2*PREMIUM_LEAST*(continueGang);
    gold[(winner+2)%3] = -2*PREMIUM_LEAST*(continueGang);
}

void StrategyLocalRM::CalcMingGangGold(int winner,int giver,int gold[3],int continueGang) {
    if (winner==giver) {
        gold[winner]       = 2*PREMIUM_LEAST*(continueGang);
        gold[(winner+1)%3] = -1*PREMIUM_LEAST*(continueGang);
        gold[(winner+2)%3] = -1*PREMIUM_LEAST*(continueGang);
    } else {
        gold[winner]       =2*PREMIUM_LEAST*(continueGang);
        gold[giver]        =-2*PREMIUM_LEAST*(continueGang);
    }
}

void StrategyLocalRM::CalcSingleWinGold(int gold[3], int winner,int whoGive) {
    auto score = _rm->_players[winner]->get_score();
    gold[winner] = score*PREMIUM_LEAST;
    
    if(whoGive==winner) {
        gold[(winner+1)%3] = -(gold[winner]/2);
        gold[(winner+2)%3] = -(gold[winner]/2);
    } else {
        gold[whoGive] = -gold[winner];
    }
    
    gold[(winner+1)%3] = gold[(winner+1)%3] + gold[(winner+1)%3]*_rm->IsMing((winner+1)%3);
    gold[(winner+2)%3] = gold[(winner+2)%3] + gold[(winner+2)%3]*_rm->IsMing((winner+2)%3);
    gold[winner] = - (gold[(winner+1)%3] + gold[(winner+2)%3]);
}

void StrategyLocalRM::CalcDoubleWinGold(int gold[3], int giver) {
    for(int i=1;i<3;i++) {
        auto score = _rm->_players[(giver+i)%3]->get_score();
        int  ting  = _rm->IsMing((giver+i)%3);

        gold[(giver+i)%3] = score*PREMIUM_LEAST + score*PREMIUM_LEAST*ting;
    }

    gold[giver] = - ((gold[(giver+1)%3] + gold[(giver+2)%3]));
}

void StrategyLocalRM::CalcNoneWinGold(int gold[3], int giver) {
    gold[(giver+1)%3] = PREMIUM_LEAST;
    gold[(giver+2)%3] = PREMIUM_LEAST;
    gold[giver] = - ((gold[(giver+1)%3] + gold[(giver+2)%3]));
}

void StrategyLocalRM::CalcHuGold(int gold[3],const WinInfo_t &win) {
    switch(win.kind) {
        case SINGLE_WIN:
            CalcSingleWinGold(gold,win.winner,win.giver);
            break;
        case DOUBLE_WIN:
            CalcDoubleWinGold(gold,win.giver);
            break;
        case NONE_WIN:
            CalcNoneWinGold(gold,win.winner);
            break;
    }
}

void StrategyLocalRM::CalculateGold(int gold[3],PlayerDir_t GoldWinner,GoldKind_t goldKind,PlayerDir_t whoGive) {
    switch(goldKind) {
        case AN_GANG:
            return CalcAnGangGold(GoldWinner,gold,_rm->_continue_gang_times);
        case MING_GANG:
            return CalcMingGangGold(GoldWinner,whoGive,gold,_rm->_continue_gang_times);
        case HU_WIN:
            return CalcHuGold(gold,_rm->GetWin());
    }
}

/*************************************
        singleton
*************************************/
StrategyRm* StrategyRm::_instance = NULL;

StrategyRm::StrategyRm(RoundManager *rm) {
    _rm = rm;
}

StrategyRm::~StrategyRm() {
}

StrategyRm *StrategyRm::getInstance(RoundManager *rm) {
    if (_instance==NULL) {
        if(rm->_MODE==LOCAL_GAME) {
            _instance = new StrategyLocalRM(rm);
        } else {
            _instance = new StrategyNetworkRM(rm);
        }
    }

    return _instance;
}

void StrategyRm::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

