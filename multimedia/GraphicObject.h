
#ifndef __GRAPHIC_OBJECT__
#define __GRAPHIC_OBJECT__

#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

#include "GameLayout.h"

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

typedef enum {
    FREE_CARD,
    PENG_CARD,
    MING_BKG,
    MING_CARD,
    AN_GANG_CARD,
    OUT_CARD,
    IN_CARD,
    L_IN_CARD,
    LR_OUT_CARD,
    LR_IN_CARD,
    LR_AN_GANG_CARD,
    R_IN_CARD,
    MING_MASK_CARD,
    MING_KOU_CARD,
    
    TEXTURE_NUM,
}TextureId_t;

typedef enum {
    SMALL,
    SMALL_BLACK,
    MIDDLE_SIZE,
    NORMAL,
    
    CARD_SIZE_NUM,
}CardSize_t;

typedef struct {
    TextureId_t texture;
    char        file[32];
}SrcItem_t;

/**********************************************************
    this is just a tool for adding items conveniently
**********************************************************/
class GMenu {
public:
    GMenu(Sprite *menuBkg);
    void AddItem(Button *item, Sprite *img=NULL);
    
    Sprite *_bkg;

private:
    static const int ITEM_NUM = 6;

    const float  Y;
    const float  SPACE;

    float  _imgSize;
    int    _itemNum;
    Sprite *_items[ITEM_NUM];
};

typedef enum {
     MENUBTN_BAOMING = 1,
     MENUBTN_TUOGUAN,
     MENUBTN_SHEZHI,
     MENUBTN_SHOP,
     MENUBTN_GAMEBACK,

     BTN_START,
}MenuButtonId_t;

class GObjectFactory {
public:
    static GObjectFactory *getInstance(cocos2d::Point origin, cocos2d::Size size);
    static void destroyInstance();

    /* global */
    Sprite *CreateTable(int option=1);
    Sprite *CreateBackground();
    GMenu  *CreateMenu();
    Button *CreateButton(MenuButtonId_t id);
    Sprite *CreateMicIcon();
    Sprite *CreateModeFont(GameMode_t mode = LOCAL_GAME);
    Sprite *CreateMaPai(int i);
    Sprite *CreateMingSign(PlayerDir_t dir);
    Sprite *CreateZhuangSign();
    Sprite *CreateClock();
    Sprite *CreateClockIndicator(PlayerDir_t dir);
    Sprite *CreatePlayerPointer();
    LayerColor *CreateTingSignBar(PlayerDir_t dir,Card_t *cards,int cardNum);
    LayerColor *CreateMingSignBar(Vec2 &referPoint, Card_t *cards,int cardNum,int *winTimes,int *remainingNum);
    /* player */
    Sprite *CreateHeadBkg(PlayerDir_t dir);
    
    /* card */
    Size RectSize(TextureId_t id);
    Sprite *Create(TextureId_t id);
    Sprite *CreateKind(Card_t type,CardSize_t size = NORMAL);
    Sprite *Create(TextureId_t id,Sprite *son);

protected:
    static float SCALE;

    static CCSpriteBatchNode * _status[TEXTURE_NUM];
    static CCSpriteBatchNode * _kindSmall[CARD_MAX];
    static CCSpriteBatchNode * _kindSmallBlack[CARD_MAX];
    static CCSpriteBatchNode * _kindMiddle[CARD_MAX];
    static CCSpriteBatchNode * _kind[CARD_MAX];
    static Size              * _rectSize[TEXTURE_NUM];

    Sprite *_image(const char *file);
    LabelAtlas *_createNumberSign(int number);
private:
    GObjectFactory(cocos2d::Point origin, cocos2d::Size size);
    static GObjectFactory *_instance;
    static GameLayout     *_layout;
    const  cocos2d::Point  ORIGIN;
    const  cocos2d::Size   SIZE;

    static SrcItem_t TEXTURE_FILES[TEXTURE_NUM];
};

#endif

