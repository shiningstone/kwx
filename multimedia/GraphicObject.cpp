
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
	_bkg->addChild(item,1,MENUBTN_BAOMING + _itemNum);

    _itemNum++;
}

Button *GObjectFactory::CreateButton(MenuButtonId_t id) {
    switch(id) {
        case MENUBTN_BAOMING:
            return Button::create("baomingbisai2.png","baomingbisai2.png","baomingbisai2.png",UI_TEX_TYPE_PLIST);
        case MENUBTN_TUOGUAN:
            return Button::create("tuoguan.png","tuoguan1.png","tuoguan1.png",UI_TEX_TYPE_PLIST);
        case MENUBTN_SHEZHI:
            return Button::create("shezhi.png","shezhi.png","shezhi.png",UI_TEX_TYPE_PLIST);
        case MENUBTN_SHOP:
            return Button::create("shangcheng2.png","shangcheng2.png","shangcheng2.png",UI_TEX_TYPE_PLIST);
        case MENUBTN_GAMEBACK:
            return Button::create("fanhui.png","fanhui.png","fanhui.png",UI_TEX_TYPE_PLIST);

        case BTN_START:
            Button *button = Button::create("kaishiyouxi1.png","kaishiyouxi2.png","kaishiyouxi2.png",UI_TEX_TYPE_PLIST);
            button->setAnchorPoint(Vec2(0.5,0.5));
            button->setPosition(Vec2(ORIGIN.x+SIZE.width/2, ORIGIN.y+SIZE.height/2));
            return button; 
    }
}


Sprite *GObjectFactory::CreateHeadBkg(PlayerDir_t dir) {
    Sprite *sprite;

    switch(dir) {
        case LEFT:
            sprite = Sprite::createWithSpriteFrameName("touxiangxinxikuang2.png");
            sprite->setAnchorPoint( _layout->AnchorOfHeadBkg(0) );
            sprite->setPosition(Vec2(
                ORIGIN.x + SIZE.width*387/1218 - sprite->getContentSize().width,
                ORIGIN.y + SIZE.height/2 + sprite->getContentSize().height/2));
            break;
        case MIDDLE:
            sprite=Sprite::createWithSpriteFrameName("touxiangxinxikuang.png");
            sprite->setAnchorPoint( _layout->AnchorOfHeadBkg(1) );
            sprite->setPosition(Vec2(
                ORIGIN.x + SIZE.width/2 - sprite->getContentSize().width/2,
                ORIGIN.y + SIZE.height*144/716 - sprite->getContentSize().height*0.5));
            break;
        case RIGHT:
            sprite=Sprite::createWithSpriteFrameName("touxiangxinxikuang2.png");
            sprite->setAnchorPoint( _layout->AnchorOfHeadBkg(2) );
            sprite->setPosition(Vec2(
                ORIGIN.x + SIZE.width*851/1218 + sprite->getContentSize().width*0.8,
                ORIGIN.y + SIZE.height/2 + sprite->getContentSize().height/2));
            break;
    }

    return sprite;
}

Sprite *GObjectFactory::CreateMicIcon() {
	auto mic = Sprite::createWithSpriteFrameName("maikefeng.png");

    mic->setAnchorPoint(Point(0.0f,0.0f));
	mic->setPosition( _layout->PositionOfMicIcon() );
	mic->setScale(0);
    
    return mic;
}

Sprite *GObjectFactory::CreateModeFont(GameMode_t mode) {
    Sprite *font;

    if(mode==LOCAL_GAME) {
        font = Sprite::createWithSpriteFrameName("danjiban.png");
        font->setOpacity(100);
        font->setAnchorPoint( Vec2(1,0) );
        font->setPosition( Vec2(ORIGIN.x+SIZE.width-10, ORIGIN.y+10) );
    } else {

    }

    return font;
}

Sprite *GObjectFactory::CreateMaPai(int direction) {
    char *file[4] = {
        "shuban.png","hengban.png","shuban.png","hengban.png"
    };
    
	auto mapai = Sprite::createWithSpriteFrameName( file[direction] );
	mapai->setAnchorPoint(Vec2(0.5,0.5));
	mapai->setScale(0.8);
    mapai->setPosition(_layout->PositionOfMapai(direction));

    return mapai;
}

Sprite *GObjectFactory::CreateMingSign(PlayerDir_t dir) {
    Sprite *sign;
    
    switch(dir) {
        case LEFT:
            sign = Sprite::create("tileImage/Tile_mingOther.png");
            sign->setAnchorPoint(Vec2(0.5,0));
            sign->setPosition(Vec2(ORIGIN.x + SIZE.width*85/1218,ORIGIN.y + SIZE.height*536/716));
            break;
        case RIGHT:
            sign = Sprite::create("tileImage/Tile_mingOther.png");
            sign->setAnchorPoint(Vec2(0.5,0));
            sign->setPosition(Vec2(ORIGIN.x + SIZE.width*1140/1218,ORIGIN.y + SIZE.height*536/716));
            break;
        case MIDDLE:
            sign = Sprite::create("tileImage/Tile_mingMe.png");
            sign->setAnchorPoint(Vec2(0,0.5));
    }

    sign->setVisible(false);
    return sign;
}

Sprite *GObjectFactory::CreateZhuangSign() {
    Sprite *zhuang = Sprite::create("tileImage/zhuang.png");
    zhuang->setScale(0);
    return zhuang;
}

Sprite *GObjectFactory::CreateClock() {
    Sprite *clock = Sprite::createWithSpriteFrameName("naozhongzhong.png");
    clock->setAnchorPoint(Vec2(0.5,0.5));
    clock->setPosition(Vec2(ORIGIN.x+SIZE.width*0.5,ORIGIN.y+SIZE.height*0.65));
    clock->setVisible(false);
    return clock;
}

Sprite *GObjectFactory::CreateClockIndicator(PlayerDir_t dir) {
    Sprite *tag;

    switch(dir) {
        case LEFT:
            tag = Sprite::createWithSpriteFrameName("naozhongzhishizuo.png");
            break;
        case MIDDLE:
            tag = Sprite::createWithSpriteFrameName("naozhongzhishixia.png");
            break;
        case RIGHT:
            tag = Sprite::createWithSpriteFrameName("naozhongzhishiyou.png");
            break;
    }

    tag->setAnchorPoint(Vec2(0.5,0.5));
    tag->setPosition(_layout->PositionOfClockIndicator(dir));
    tag->setVisible(false);

    return tag;
}

Sprite *GObjectFactory::CreatePlayerPointer() {
	auto pointer = Sprite::create("tileImage/tile_pointer.png");
	pointer->setPosition(Vec2(ORIGIN.x+SIZE.width/2,ORIGIN.y+SIZE.height/2));
	pointer->setVisible(false);
    return pointer;
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



