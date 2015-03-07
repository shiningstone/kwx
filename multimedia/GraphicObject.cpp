
#include "GraphicObject.h"


/*************************************
    singleton
*************************************/
GObjectFactory *GObjectFactory::_instance = NULL;

GObjectFactory *GObjectFactory::getInstance() {
    if (_instance==NULL) {
        _instance = new GObjectFactory();
    }

    return _instance;
}

void GObjectFactory::destroyInstance() {
    delete _instance;
    _instance = NULL;
}


