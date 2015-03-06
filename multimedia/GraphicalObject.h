
#ifndef __GRAPHICAL_OBJECT__
#define __GRAPHICAL_OBJECT__

#include "cocos2d.h"
USING_NS_CC;

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

class TextureFactory {
public:
    static TextureFactory *getInstance();
    static void destroyInstance();
    
    Size RectSize(TextureId_t id);
    Sprite *Create(TextureId_t id);
    Sprite *Create(TextureId_t id,Sprite *son);

protected:
    static CCSpriteBatchNode * _protoType[TEXTURE_NUM];
    static Size              * _rectSize[TEXTURE_NUM];
    
private:
    TextureFactory();
    static TextureFactory *_instance;

    typedef struct {
        TextureId_t texture;
        char        file[32];
    }SrcItem_t;

    SrcItem_t TEXTURE_FILES[TEXTURE_NUM];
};

#endif

