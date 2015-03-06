
#ifndef __GAME_LAYOUT__
#define __GAME_LAYOUT__

#include "cocos2d.h"
USING_NS_CC;

class GameLayout{
    public:
        static GameLayout *getInstance();
        static void destroyInstance();
        
        void init();
    private:
        GameLayout();
        static GameLayout *_instance;

        Coordinate _coordinate;
};

#endif
