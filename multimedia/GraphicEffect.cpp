
#include "./../Resources.h"

#include "VoiceEffect.h"
#include "GraphicObject.h"
#include "GraphicEffect.h"

GraphicEffect::GraphicEffect(VoiceEffect *voice,GObjectFactory *object) {
    _voice  = voice;
    _object = object;
}

void GraphicEffect::Hide(Node *parent,TagId_t buttonTag) {
    Button *button = (Button *)parent->getChildByTag(buttonTag);

    if(button!=NULL) {
        button->setTouchEnabled(false);
        button->_ID = MIDDLE;
        
        button->runAction(Sequence::create(
            ScaleTo::create(0.1,1),NULL));
    }
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
                MoveTo::create(0,dest),NULL),NULL);
}

Node *GraphicEffect::Mask(Sprite *card) {
    auto mask = _object->Create(MING_MASK_CARD);
    
    mask->setAnchorPoint(Vec2(0.5,0.5));
    mask->setPosition(card->getTextureRect().size.width/2,card->getTextureRect().size.height/2);
    card->addChild(mask,2,MING_KOU_MASK);

    return mask;
}

/*************************************
    singleton
*************************************/
GraphicEffect *GraphicEffect::_instance = NULL;

GraphicEffect *GraphicEffect::getInstance(VoiceEffect *voice,GObjectFactory *object) {
    if (_instance==NULL) {
        _instance = new GraphicEffect(voice,object);
    }

    return _instance;
}

void GraphicEffect::destroyInstance() {
    delete _instance;
    _instance = NULL;
}


