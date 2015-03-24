
#ifndef __GRAPHICAL_EFFECT__
#define __GRAPHICAL_EFFECT__

#include "cocos2d.h"
USING_NS_CC;

#include "./../utils/LogManager.h"

class VoiceEffect;
class GraphicEffect {
    public:
        static GraphicEffect *getInstance();
        static void destroyInstance();

        TargetedAction *Shade(Node* target);
		Sequence *MoveDistributeCard(const Vec2 &orig,const Vec2 &dest);
    private:
        GraphicEffect();
        static GraphicEffect *_instance;

        VoiceEffect *_voice;
        Logger *_logger;
};

#endif

