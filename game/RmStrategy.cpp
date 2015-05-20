
#include "CardCollection.h"
#include "Player.h"
#include "RoundManager.h"
#include "RaceLayer.h"

#include "RmStrategy.h"

/*************************************
        local strategy
*************************************/
void RmStrategy::update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive) {
    _rm->update_gold(GoldWinner,Gold_kind,whoGive);
}

/*************************************
        network strategy
*************************************/
class NetworkStrategy : public RmStrategy {
public:
    virtual void update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive);

    NetworkStrategy(RoundManager *rm);
    ~NetworkStrategy();
};

NetworkStrategy::NetworkStrategy(RoundManager *rm)
    :RmStrategy(rm)
{

}

void NetworkStrategy::update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive) {
}

/*************************************
        singleton
*************************************/
RmStrategy* RmStrategy::_instance = NULL;

RmStrategy::RmStrategy(RoundManager *rm) {
    _rm = rm;
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


