
#ifndef __GRAPHIC_OBJECT__
#define __GRAPHIC_OBJECT__
#include "cocos2d.h"
USING_NS_CC;

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

class GObjectFactory {
public:
    static GObjectFactory *getInstance();
    static void destroyInstance();
    
    Size RectSize(TextureId_t id);
    Sprite *Create(TextureId_t id);
    Sprite *CreateKind(Card_t type,CardSize_t size = NORMAL);
    Sprite *Create(TextureId_t id,Sprite *son);

protected:
    static CCSpriteBatchNode * _status[TEXTURE_NUM];
    static CCSpriteBatchNode * _kindSmall[CARD_SIZE_NUM];
    static CCSpriteBatchNode * _kindSmallBlack[CARD_SIZE_NUM];
    static CCSpriteBatchNode * _kindMiddle[CARD_SIZE_NUM];
    static CCSpriteBatchNode * _kind[CARD_SIZE_NUM];
    static Size              * _rectSize[TEXTURE_NUM];
    
private:
    GObjectFactory();
    static GObjectFactory *_instance;

    static SrcItem_t TEXTURE_FILES[TEXTURE_NUM];
};

#endif

