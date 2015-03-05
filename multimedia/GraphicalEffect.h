
#ifndef __GRAPHICAL_EFFECT__
#define __GRAPHICAL_EFFECT__

#include "ui/CocosGUI.h"
using namespace CocosDenshion;

class GraphicEffect {
    public:
        static GraphicEffect *getInstance();
        static void destroyInstance();

        TargetedAction *Shade(Sprite* target);
    private:
        GraphicEffect();
        static GraphicEffect *_instance;
};

#endif

