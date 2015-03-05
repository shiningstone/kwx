
#include "GraphicalEffect.h"

TargetedAction *GraphicEffect::Shade(Sprite* target) {
    return TargetedAction::create(target,
        Sequence::create(
            ScaleTo::create(0,1),
            
            Spawn::create(
                FadeOut::create(0.3),
                ScaleTo::create(0.3,1.5),NULL),NULL));
}

/*************************************
    singleton
*************************************/
GraphicEffect *GraphicEffect::getInstance() {
    if (_instance==NULL) {
        _instance = new GraphicEffect();
    }

    return _instance;
}

void GraphicEffect::destroyInstance() {
    delete _instance;
    _instance = NULL;
}


