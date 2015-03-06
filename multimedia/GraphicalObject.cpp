
#include "GraphicObject.h"

CCSpriteBatchNode *TextureFactory::_protoType[TEXTURE_NUM] = {NULL};
Size *TextureFactory::_rectSize[TEXTURE_NUM] = {NULL};

TextureFactory::TextureFactory() {
    _protoType[FREE_CARD] = CCSpriteBatchNode::create("tileImage/me-handin.png");
    _protoType[FREE_CARD]->retain();
}

Size TextureFactory::RectSize(TextureId_t id) {
    if(_rectSize[id]==NULL) {
        _rectSize[id] = new Size((Create(id))->getTextureRect());
    }

    return *(_rectSize[id]);
}

Sprite *TextureFactory::Create(TextureId_t id) {
    return Sprite::createWithTexture(_protoType[id]->getTexture());
}

Sprite *TextureFactory::Create(TextureId_t id, Sprite *son) {
    Sprite *parent = Sprite::createWithTexture(_protoType[id]->getTexture());

    son->setAnchorPoint(Vec2(0.5,0.5));/*anchor point is not set in some cases*/
    son->setPosition( Vec2(
        parent->getTextureRect().size.width/2,
        parent->getTextureRect().size.height*0.4) );
    parent->addChild(son);/*in ming_gang_tip_effect(hide out card), there is extra parameter equals 1*/

    return parent;
}

/*************************************
    singleton
*************************************/
TextureFactory *TextureFactory::_instance = NULL;

TextureFactory *TextureFactory::getInstance() {
    if (_instance==NULL) {
        _instance = new TextureFactory();
    }

    return _instance;
}

void TextureFactory::destroyInstance() {
    for(int i=0;i<TEXTURE_NUM;i++) {
        _protoType[i]->release();
    }
    
    delete _instance;
    _instance = NULL;
}


