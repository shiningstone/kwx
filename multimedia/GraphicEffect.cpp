
#include "GraphicEffect.h"

GraphicEffect::GraphicEffect() {

}

TargetedAction *GraphicEffect::Shade(Node* target) {
    return TargetedAction::create(target,
        Sequence::create(
            ScaleTo::create(0,1),
            
            Spawn::create(
                FadeOut::create(0.3),
                ScaleTo::create(0.3,1.5),NULL),NULL));
}

Sequence *GraphicEffect::MoveDistributeCard(const Vec2 &orig,const Vec2 &dest) {
    return Sequence::create(
            DelayTime::create(0.4),
            ScaleTo::create(0,1),
            MoveTo::create(0.2f,orig),
            _voice->Speak("sort"),
            Spawn::create(
                ScaleTo::create(0,0),
                MoveTo::create(0,dest),NULL),NULL)
}

/*************************************
    singleton
*************************************/
GraphicEffect *GraphicEffect::_instance = NULL;

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


