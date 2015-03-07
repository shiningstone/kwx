
#include "GraphicObject.h"

/***********************************************
        global
***********************************************/
float GObjectFactory::SCALE = 1.189;
GameLayout *GObjectFactory::_layout = NULL;

Sprite *GObjectFactory::CreateTable(int option) {
	Sprite *sprite = (option==1)?(Sprite::create("racetable.png")):(Sprite::create("racetable2.png"));
	sprite->setPosition(_layout->Center());
	sprite->setScaleX(SCALE);
	sprite->setScaleY(SCALE);
    
    return sprite;
}

Sprite *GObjectFactory::CreateBackground() {
	auto center_bkg = Sprite::createWithSpriteFrameName("kawuxing.png");
	center_bkg->setPosition(_layout->Center());
    
    return center_bkg;
}

GMenu *GObjectFactory::CreateMenu() {
	auto menu_bkg = Sprite::createWithSpriteFrameName("gongnenganniu.png");
	menu_bkg->setPosition(Vec2(
        SIZE.width/2 + ORIGIN.x, 
        SIZE.height + ORIGIN.y-menu_bkg->getTextureRect().size.height/2));
    
    return new GMenu(menu_bkg);
}

/***********************************************
        card
***********************************************/
CCSpriteBatchNode *GObjectFactory::_status[TEXTURE_NUM] = {NULL};
CCSpriteBatchNode *GObjectFactory::_kindSmall[CARD_MAX] = {NULL};
CCSpriteBatchNode *GObjectFactory::_kindSmallBlack[CARD_MAX] = {NULL};
CCSpriteBatchNode *GObjectFactory::_kindMiddle[CARD_MAX] = {NULL};
CCSpriteBatchNode *GObjectFactory::_kind[CARD_MAX] = {NULL};

Size *GObjectFactory::_rectSize[TEXTURE_NUM] = {NULL};

SrcItem_t GObjectFactory::TEXTURE_FILES[TEXTURE_NUM] = {
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

GObjectFactory::GObjectFactory(cocos2d::Point origin, cocos2d::Size size)
:ORIGIN(origin),SIZE(size) {
    for(int i=0;i<TEXTURE_NUM;i++) {
        char *file = TEXTURE_FILES[i].file;
        
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

Size GObjectFactory::RectSize(TextureId_t id) {
    if(_rectSize[id]==NULL) {
        _rectSize[id] = new Size((Create(id))->getTextureRect().size);
    }

    return *(_rectSize[id]);
}

Sprite *GObjectFactory::Create(TextureId_t id) {
    return Sprite::createWithTexture(_status[id]->getTexture());
}


Sprite *GObjectFactory::CreateKind(Card_t type,CardSize_t size) {
    switch(size) {
        case SMALL:
            return Sprite::createWithTexture(_kindSmall[type]->getTexture());
        case SMALL_BLACK:
            return Sprite::createWithTexture(_kindSmallBlack[type]->getTexture());
        case MIDDLE_SIZE:
            return Sprite::createWithTexture(_kindMiddle[type]->getTexture());
        case NORMAL:
            return Sprite::createWithTexture(_kind[type]->getTexture());
    }
}

Sprite *GObjectFactory::Create(TextureId_t id, Sprite *son) {
    Sprite *parent = Sprite::createWithTexture(_status[id]->getTexture());

    son->setAnchorPoint(Vec2(0.5,0.5));/*anchor point is not set in some cases*/
    son->setPosition( Vec2(
        parent->getTextureRect().size.width/2,
        parent->getTextureRect().size.height*0.4) );
    parent->addChild(son);/*in ming_gang_tip_effect(hide out card), there is extra parameter equals 1*/

    return parent;
}

/*************************************
    menu
*************************************/
#define BAOMING_MENU_BUTTON					1
#define TUOGUAN_MENU_BUTTON					2
#define SHEZHI_MENU_BUTTON					3
#define SHOP_MENU_BUTTON					4
#define GAMEBACK_MENU_BUTTON				5

GMenu::GMenu(Sprite *menuBkg)
:Y(menuBkg->getTextureRect().size.height*3/5),
SPACE(menuBkg->getTextureRect().size.width/11.0f) {
    _bkg = menuBkg;
    _imgSize = -1;
    _itemNum = 0;
    for(int i=0;i<ITEM_NUM;i++) {
        _items[i] = NULL;
    }
}

void GMenu::AddItem(Button *item,Sprite *img) {
    if (_imgSize<0) {/*img is only required when the first item is added*/
        _imgSize = img->getTextureRect().size.width;
    }
    
	float x = SPACE*4/5 + _itemNum * (_imgSize+SPACE);
    
	item->setAnchorPoint(Vec2(0,0.5));
	item->setPosition(Vec2(x,Y));
	_bkg->addChild(item,1,BAOMING_MENU_BUTTON+_itemNum);

    _itemNum++;
}

/*************************************
    singleton
*************************************/
GObjectFactory *GObjectFactory::_instance = NULL;

GObjectFactory *GObjectFactory::getInstance(cocos2d::Point origin, cocos2d::Size size) {
    if (_instance==NULL) {
        _instance = new GObjectFactory(origin,size);
        _layout = GameLayout::getInstance(origin,size);
    }

    return _instance;
}

void GObjectFactory::destroyInstance() {
    for(int i=0;i<TEXTURE_NUM;i++) {
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



