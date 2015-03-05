
#ifndef __LAYOUT__
#define __LAYOUT__

#include "ui/CocosGUI.h"
using namespace CocosDenshion;

class Layout {
    public:
        static Layout *getInstance();
        static void destroyInstance();

        /*************************************
            gold count
        *************************************/
        Vec2 _AnchorOfSign(int dir);
        Vec2 _PositionOfSign(int dir,Size size,Vec2 origin,int xOffset = 0);
        Vec2 _DestOfSign(int dir,Size size,Vec2 origin,int xOffset = 0);
        Vec2 _AnchorOfNumber(int dir);
        Vec2 _PositionOfNumber(int dir,Size size,Vec2 origin);
        Vec2 _DestOfNumber(int dir,Size size,Vec2 origin);
        Vec2 _AnchorOfGold(int dir);
        Vec2 _PositionOfGold(int dir,Size size,Vec2 origin,int xOffset = 0);
        Vec2 _DestOfGold(int dir,Size size,Vec2 origin,int xOffset = 0);
        
        /*************************************
            out card
        *************************************/
        int  _RotateAngleOfOutcard(int dir);
        Vec2 _AnchorOfOutcard(int dir);
        Vec2 _PositionOfOutcard(int dir,Size size,Vec2 origin);

        /*************************************
            action
        *************************************/
        Vec2 _PostionOfActSign(int dir);

        Vec2 _OrigPositionOfGangCard(int i,Size size);
        Vec2 _MiddlePositionOfGangCard(int i,Size size);
        Vec2 _DestPositionOfGangCard(int i,Size size);
        Vec2 _DestPositionOfGangCardInHand(int i,Size gangCard);
        Vec2 _MiddlePositionOfGangCardInHand(int i,Vec2 origPos,Size freeCard);
        Vec2 _PositionOfMingGangCard(int i,Size size);

    private:
        Layout();
        static Layout *_instance;
};

#endif

