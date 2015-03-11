
#ifndef __GAME_LAYOUT__
#define __GAME_LAYOUT__

#include "cocos2d.h"
USING_NS_CC;

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

typedef struct {
    cocos2d::Vec2   basePoint;
    cocos2d::Vec2   riverPoint;
    cocos2d::Vec2   zhuangPoint;
}PlayerPos_t;

class GameLayout {
    public:
        static GameLayout *getInstance(cocos2d::Point origin, cocos2d::Size size);
        static void destroyInstance();

        /*************************************
            head image
        *************************************/
        Vec2 AnchorOfHeadBkg(int dir);
        void SetPlayerBkg(PlayerDir_t id, Sprite *bkg);
        /*these should be private finally*/
        cocos2d::Sprite *_playerBkg[3];
        PlayerPos_t      _playerPosi[3];

        /*************************************
            global
        *************************************/
        Vec2 PositionOfMicIcon();
        Vec2 PositionOfMapai(int direction);
        Vec2 PositionOfClock();
        Vec2 PositionOfClockIndicator(PlayerDir_t dir);
        Vec2 AnchorOfZhuang(PlayerDir_t dir);
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
        Vec2 PositionOfMingSignForMe(float zhuangWidth = 0.0);
        Vec2 PositionOfActSign(int dir);

        Vec2 OrigPositionOfGangCard(int i,Size size);
        Vec2 MiddlePositionOfGangCard(int i,Size size);
        Vec2 DestPositionOfGangCard(int i,Size size);
        Vec2 DestPositionOfGangCardInHand(int i,Size gangCard);
        Vec2 MiddlePositionOfGangCardInHand(int i,Vec2 origPos,Size freeCard);
        Vec2 PositionOfMingGangCard(int i,Size size);

        Vec2 PositionOfTingSignBar(PlayerDir_t dir);
        /*************************************
            card in hand
        *************************************/
        Vec2 AnchorOfNormalCard(PlayerDir_t dir);
        Vec2 PositionOfNormalCard(PlayerDir_t dir,int x,int y);
        Vec2 AnchorOfFreeCard(PlayerDir_t dir);
        int  RotateAngleOfCard(PlayerDir_t dir);
        float YOfNextCard(PlayerDir_t dir,float originY,Size cardSize);
        /*************************************
            card in river
        *************************************/
        int  _getRiverLineNo(int i);
        int  _getRiverLineIdx(int i);
        Vec2 OrigPositionOfRiverCard(PlayerDir_t dir,int idx);
        Vec2 Middle1PositionOfRiverCard(PlayerDir_t dir,int idx);
        Vec2 Middle2PositionOfRiverCard(PlayerDir_t dir,int idx);
        Vec2 DestPositionOfRiverCard(PlayerDir_t dir,int idx);
    private:
        static const int RIVER_CARD_IN_LINE_1 = 6;
        static const int RIVER_CARD_IN_LINE_2 = 14;

        GameLayout(cocos2d::Point origin, cocos2d::Size size);
        static GameLayout *_instance;
        Logger *_logger;

        cocos2d::Size  SIZE;
        cocos2d::Point ORIGIN;
};

#endif

