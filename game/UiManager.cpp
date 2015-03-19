
#include "UiManager.h"

/*************************************
        singleton
*************************************/
UiManager* UiManager::_instance = NULL;

UiManager::UiManager(RoundManager *roundManager) {
    _roundManager = roundManager;
}

UiManager::~UiManager() {
}

UiManager *UiManager::getInstance(RoundManager *roundManager) {
    if (_instance==NULL) {
        _instance = new UiManager(roundManager);
    }

    return _instance;
}

void UiManager::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

