
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

        cocos2d::Layer *_employer;
        Coordinate _coordinate;

        void _addTable(int option);
        void _addBackground();
        void _cache(const char *file);
};

#endif
