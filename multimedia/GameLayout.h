
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
        Vec2 AnchorOfHeadBkg(PlayerDir_t dir);
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
        Vec2 AnchorOfSign(PlayerDir_t dir);
        Vec2 PositionOfSign(PlayerDir_t dir,int xOffset = 0);
        Vec2 DestOfSign(PlayerDir_t dir,int xOffset = 0);
        Vec2 AnchorOfNumber(PlayerDir_t dir);
        Vec2 PositionOfNumber(PlayerDir_t dir);
        Vec2 DestOfNumber(PlayerDir_t dir);
        Vec2 AnchorOfGold(PlayerDir_t dir);
        Vec2 PositionOfGold(PlayerDir_t dir,int xOffset = 0);
        Vec2 DestOfGold(PlayerDir_t dir,int xOffset = 0);
        /*************************************
            out card
        *************************************/
        int  RotateAngleOfOutcard(PlayerDir_t dir);
        Vec2 AnchorOfOutcard(PlayerDir_t dir);
        Vec2 PositionOfOutcard(PlayerDir_t dir);
        /*************************************
            action
        *************************************/
        Vec2 Center();
        Vec2 PositionOfMingSign();
        Vec2 PositionOfMingSignForMe(float zhuangWidth = 0.0);
        Vec2 PositionOfActSign(PlayerDir_t dir);

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
        int  _cardsInLine(int line);
        int  _lineNo(int i);
        int  _lineIdx(int i);
        int  _viewIdx(int i);
        Vec2 OrigPositionOfRiverCard(PlayerDir_t dir,int idx);
        Vec2 Middle1PositionOfRiverCard(PlayerDir_t dir,int idx);
        Vec2 Middle2PositionOfRiverCard(PlayerDir_t dir,int idx);
        Vec2 DestPositionOfRiverCard(PlayerDir_t dir,int idx);
        
        void GetBizerPoints(ccBezierConfig &cfg, int riverIdx, const Vec2 &location);
    private:
        static const int RIVER_CARD_IN_LINE_1 = 6;
        static const int RIVER_CARD_IN_LINE_2 = 8;

        GameLayout(cocos2d::Point origin, cocos2d::Size size);
        static GameLayout *_instance;
        Logger *_logger;

        cocos2d::Size  SIZE;
        cocos2d::Point ORIGIN;
};

#endif

