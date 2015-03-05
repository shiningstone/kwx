
#ifndef __GRAPHICAL_EFFECT__
#define __GRAPHICAL_EFFECT__

#include "cocos2d.h"
USING_NS_CC;

#include "./../utils/LogManager.h"

class GraphicEffect {
    public:
        static GraphicEffect *getInstance();
        static void destroyInstance();

        TargetedAction *Shade(Node* target);
    private:
        GraphicEffect();
        static GraphicEffect *_instance;

        Logger *_logger;
};

#endif

