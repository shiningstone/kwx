
#include "CardCollection.h"
#include "Player.h"
#include "RoundManager.h"
#include "RaceLayer.h"

#include "RmStrategy.h"

/*************************************
        local strategy
*************************************/
void RmStrategy::CalcAnGangGold(int winner,int gold[3]) {
    gold[winner]       = 4*PREMIUM_LEAST*(_rm->_continue_gang_times);
    gold[(winner+1)%3] = -2*PREMIUM_LEAST*(_rm->_continue_gang_times);
    gold[(winner+2)%3] = -2*PREMIUM_LEAST*(_rm->_continue_gang_times);
}

void RmStrategy::CalcMingGangGold(int winner,int giver,int gold[3]) {
    if (winner==giver) {
        gold[winner]       = 2*PREMIUM_LEAST*(_rm->_continue_gang_times);
        gold[(winner+1)%3] = -1*PREMIUM_LEAST*(_rm->_continue_gang_times);
        gold[(winner+2)%3] = -1*PREMIUM_LEAST*(_rm->_continue_gang_times);
    } else {
        gold[winner]       =2*PREMIUM_LEAST*(_rm->_continue_gang_times);
        gold[giver]        =-2*PREMIUM_LEAST*(_rm->_continue_gang_times);
    }
}

void RmStrategy::CalcSingleWinGold(int gold[3], int winner,int whoGive) {
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

void RmStrategy::CalcDoubleWinGold(int gold[3], int giver) {
    for(int i=1;i<3;i++) {
        auto score = _rm->_players[(giver+i)%3]->get_score();
        int  ting  = _rm->IsMing((giver+i)%3);

        gold[(giver+i)%3] = score*PREMIUM_LEAST + score*PREMIUM_LEAST*ting;
    }

    gold[giver] = - ((gold[(giver+1)%3] + gold[(giver+2)%3]));
}

void RmStrategy::CalcNoneWinGold(int gold[3], int giver) {
    gold[(giver+1)%3] = PREMIUM_LEAST;
    gold[(giver+2)%3] = PREMIUM_LEAST;
    gold[giver] = - ((gold[(giver+1)%3] + gold[(giver+2)%3]));
}

void RmStrategy::CalcHuGold(int gold[3]) {
    WinInfo_t win;
    _rm->GetWin(win);
    
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

void RmStrategy::CalculateGold(int gold[3],PlayerDir_t GoldWinner,GoldKind_t goldKind,PlayerDir_t whoGive) {
    switch(goldKind) {
        case AN_GANG:
            return CalcAnGangGold(GoldWinner,gold);
        case MING_GANG:
            return CalcMingGangGold(GoldWinner,whoGive,gold);
        case HU_WIN:
            return CalcHuGold(gold);
    }
}

void RmStrategy::show_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive) {
    int gold[3] = {0};

    CalculateGold(gold,GoldWinner,Gold_kind,whoGive);

    for(int i=0;i<3;i++) {
        Database *database = Database::getInstance();
        int total = _rm->_players[i]->UpdateProperty(gold[i]);
        database->SetProperty(_rm->_players[i]->_profile.id, total);
    }

	for(int id=0;id<3;id++) {
        _rm->_uiManager->GuiUpdateScore(id,_rm->_players[id]->_profile.property);
        _rm->_uiManager->GuiJinBiShow((PlayerDir_t)id,gold[id]);        
	}
}

/*************************************
        network strategy
*************************************/
class NetworkStrategy : public RmStrategy {
public:
    virtual void show_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive);

    NetworkStrategy(RoundManager *rm);
    ~NetworkStrategy();
};

NetworkStrategy::NetworkStrategy(RoundManager *rm)
    :RmStrategy(rm)
{

}

void NetworkStrategy::show_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive) {
}

/*************************************
        singleton
*************************************/
RmStrategy* RmStrategy::_instance = NULL;

RmStrategy::RmStrategy(RoundManager *rm) {
    _rm = rm;

    PREMIUM_LEAST = 200;
}

RmStrategy::~RmStrategy() {
}

RmStrategy *RmStrategy::getInstance(RoundManager *rm) {
    if (_instance==NULL) {
        if(rm->_MODE==LOCAL_GAME) {
            _instance = new RmStrategy(rm);
        } else {
            _instance = new NetworkStrategy(rm);
        }
    }

    return _instance;
}

void RmStrategy::destroyInstance() {
    delete _instance;
    _instance = NULL;
}


