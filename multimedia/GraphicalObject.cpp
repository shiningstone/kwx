
#include "GraphicObject.h"

CCSpriteBatchNode *TextureFactory::_status[TEXTURE_MAX] = {NULL};
CCSpriteBatchNode *TextureFactory::_kindSmall[CARD_MAX] = {NULL};
CCSpriteBatchNode *TextureFactory::_kindSmallBlack[CARD_MAX] = {NULL};
CCSpriteBatchNode *TextureFactory::_kindMiddle[CARD_MAX] = {NULL};
CCSpriteBatchNode *TextureFactory::_kind[CARD_MAX] = {NULL};

Size *TextureFactory::_rectSize[TEXTURE_MAX] = {NULL};

SrcItem_t TextureFactory::TEXTURE_FILES[TEXTURE_MAX] = {
    {FREE_CARD,       "tileImage/me-handin.png" },
    {PENG_CARD,       "tileImage/me-peng.png" },
    {MING_BKG,        "tileImage/Ming-bkg.png" },
    {MING_CARD,       "tileImage/me-ming.png" },
    {AN_GANG_CARD,    "tileImage/me-angang.png" },
    {OUT_CARD,        "tileImage/me-out.png" },
    {IN_CARD,         "tileImage/me-in.png" },
    {L_IN_CARD,       "tileImage/left-handin.png" },
    {LR_OUT_CARD,     "tileImage/lr-out.png" },
    {LR_IN_CARD,      "tileImage/lr-in.png" },
    {LR_AN_GANG_CARD, "tileImage/lr-angang.png" },
    {R_IN_CARD,       "tileImage/right-handin.png" },
    {MING_MASK_CARD,  "tileImage/ming-mask.png" },
    {MING_KOU_CARD,   "tileImage/ming-kou.png" },
};

TextureFactory::TextureFactory() {
    for(int i=0;i<TEXTURE_MAX;i++) {
        char *file = TEXTURE_FILES[i][1];
        
        _status[i] = CCSpriteBatchNode::create(file);
        _status[i]->retain();
    }

    for(int i=0;i<CARD_MAX;i++) {/* is it more efficiency to use SetScale ??? */
        _kindSmall[i] = CCSpriteBatchNode::create(
                        String::createWithFormat("tileImage/tile_Up_%d.png",(int)(i+1))->getCString());
		_kindSmallBlack[i] = CCSpriteBatchNode::create(
                        String::createWithFormat("tileImage/tile_Upblack_%d.png",(int)(i+1))->getCString());
		_kindMiddle[i] = CCSpriteBatchNode::create(
                        String::createWithFormat("tileImage/tile_Set_%d.png",(int)(i+1))->getCString());
		_kind[i] =CCSpriteBatchNode::create(
                        String::createWithFormat("tileImage/tile_%d.png",(int)(i+1))->getCString());

        _kindSmall[i]->retain();
        _kindSmallBlack[i]->retain();
        _kindMiddle[i]->retain();
        _kind[i]->retain();
    }
}

Size TextureFactory::RectSize(TextureId_t id) {
    if(_rectSize[id]==NULL) {
        _rectSize[id] = new Size((Create(id))->getTextureRect());
    }

    return *(_rectSize[id]);
}

Sprite *TextureFactory::Create(TextureId_t id) {
    return Sprite::createWithTexture(_status[id]->getTexture());
}


Sprite *TextureFactory::CreateKind(Card_t type,CardSize_t size) {
    switch(size) {
        case SMALL:
            return Sprite::createWithTexture(_kindSmall[type]->getTexture());
        case SMALL_BLACK:
            return Sprite::createWithTexture(_kindSmallBlack[type]->getTexture());
        case MIDDLE:
            return Sprite::createWithTexture(_kindMiddle[type]->getTexture());
        case NORMAL:
            return Sprite::createWithTexture(_kind[type]->getTexture());
    }
}

Sprite *TextureFactory::Create(TextureId_t id, Sprite *son) {
    Sprite *parent = Sprite::createWithTexture(_status[id]->getTexture());

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
    for(int i=0;i<TEXTURE_MAX;i++) {
        _status[i]->release();
    }
    
    for(int i=0;i<CARD_MAX;i++) {
        _kindSmall[i]->release();
        _kindSmallBlack[i]->release();
        _kindMiddle[i]->release();
        _kind[i]->release();
    }
    
    delete _instance;
    _instance = NULL;
}


