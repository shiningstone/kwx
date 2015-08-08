
#include "ErrorManager.h"

void ErrorManager::set_handle(MsgHandle *handle) {
    _errorHandle = handle;
}

void ErrorManager::notify_error(void *errorMsg) {
    _errorHandle->_msgQueue.push(errorMsg);
}

ErrorManager::ErrorManager() {
    _logger = LOGGER_REGISTER("ErrorManager");
}

ErrorManager::~ErrorManager() {
    LOGGER_DEREGISTER(_logger);
}

/**********************************************
        singleton
**********************************************/
ErrorManager* ErrorManager::_instance = NULL;

ErrorManager *ErrorManager::getInstance() {
    if (_instance==NULL) {
        _instance = new ErrorManager();
    }

    return _instance;
}

void ErrorManager::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

