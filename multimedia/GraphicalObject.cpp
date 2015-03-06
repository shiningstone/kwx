
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


