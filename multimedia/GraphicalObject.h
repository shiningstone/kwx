
#ifndef __GRAPHICAL_OBJECT__
#define __GRAPHICAL_OBJECT__

#include "cocos2d.h"
USING_NS_CC;

#include "./../utils/LogManager.h"

typedef enum {
    FREE_CARD,
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
};

#endif

