
#ifndef __GRAPHICAL_EFFECT__
#define __GRAPHICAL_EFFECT__

#include "cocos2d.h"
USING_NS_CC;

#include "./../utils/LogManager.h"

class VoiceEffect;
class GObjectFactory;
class GraphicEffect {
    public:
        static GraphicEffect *getInstance(VoiceEffect *voice,GObjectFactory *object);
        static void destroyInstance();

        void Hide(Node *parent,TagId_t tag);
        Node *Mask(Sprite *card);
        TargetedAction *Shade(Node* target);
		Sequence *MoveDistributeCard(const Vec2 &orig,const Vec2 &dest);
    private:
        GraphicEffect(VoiceEffect *voice,GObjectFactory *object);
        static GraphicEffect *_instance;

        VoiceEffect    *_voice;
        GObjectFactory *_object;  

        Logger *_logger;
};

#endif

