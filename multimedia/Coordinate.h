
#ifndef __COORDINATE__
#define __COORDINATE__

#include "cocos2d.h"
USING_NS_CC;

#include "./../utils/LogManager.h"

class Coordinate {
    public:
        static Coordinate *getInstance();
        static void destroyInstance();

        void SetScope(cocos2d::Point origin,cocos2d::Size visibleSize);

        /*************************************
            gold count
        *************************************/
        Vec2 AnchorOfSign(int dir);
        Vec2 PositionOfSign(int dir,int xOffset = 0);
        Vec2 DestOfSign(int dir,int xOffset = 0);
        Vec2 AnchorOfNumber(int dir);
        Vec2 PositionOfNumber(int dir);
        Vec2 DestOfNumber(int dir);
        Vec2 AnchorOfGold(int dir);
        Vec2 PositionOfGold(int dir,int xOffset = 0);
        Vec2 DestOfGold(int dir,int xOffset = 0);
        
        /*************************************
            out card
        *************************************/
        int  RotateAngleOfOutcard(int dir);
        Vec2 AnchorOfOutcard(int dir);
        Vec2 PositionOfOutcard(int dir);

        /*************************************
            action
        *************************************/
        Vec2 Center();
        Vec2 PositionOfMingSign();
        Vec2 PositionOfActSign(int dir);

        Vec2 OrigPositionOfGangCard(int i,Size size);
        Vec2 MiddlePositionOfGangCard(int i,Size size);
        Vec2 DestPositionOfGangCard(int i,Size size);
        Vec2 DestPositionOfGangCardInHand(int i,Size gangCard);
        Vec2 MiddlePositionOfGangCardInHand(int i,Vec2 origPos,Size freeCard);
        Vec2 PositionOfMingGangCard(int i,Size size);

    private:
        Coordinate();
        static Coordinate *_instance;
        Logger *_logger;

        cocos2d::Size  SIZE;
        cocos2d::Point ORIGIN;
};

#endif

