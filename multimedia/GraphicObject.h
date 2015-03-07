
#ifndef __GRAPHIC_OBJECT__
#define __GRAPHIC_OBJECT__
#include "cocos2d.h"
USING_NS_CC;

#include "./../utils/LogManager.h"

typedef enum {
    FREE_CARD,
    GOBJECT_NUM,
}GObjectId_t;

class GObjectFactory {
public:
    static GObjectFactory *getInstance();
    static void destroyInstance();
    
    Size RectSize(GObjectId_t id);
    Sprite *Create(GObjectId_t id);

protected:
    static CCSpriteBatchNode * _protoType[GOBJECT_NUM];
    static Size              * _rectSize[GOBJECT_NUM];
    
private:
    GObjectFactory();
    static GObjectFactory *_instance;
};


#endif

