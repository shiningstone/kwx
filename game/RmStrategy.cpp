
#include "RoundManager.h"
#include "RmStrategy.h"

/*************************************
        local strategy
*************************************/
void RmStrategy::update_gold_account() {

}

/*************************************
        network strategy
*************************************/
class NetworkStrategy : public RmStrategy {
public:
    virtual void update_gold_account();

    NetworkStrategy(RoundManager *rm);
    ~NetworkStrategy();
};

NetworkStrategy::NetworkStrategy(RoundManager *rm)
    :RmStrategy(rm)
{

}

void NetworkStrategy::update_gold_account() {

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


