
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

Sprite *GObjectFactory::Create(TextureId_t id,PlayerDir_t dir,float x,float y) {
    Sprite *card = Create(id);
    card->setAnchorPoint(_layout->AnchorOfNormalCard((PlayerDir_t)dir));
    card->setPosition(_layout->PositionOfNormalCard((PlayerDir_t)dir,x,y));
    
    return card;
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

/* only for LEFT/RIGHT */
void GObjectFactory::LayDownWithFace(PlayerDir_t dir,Sprite *parent,Card_t kind, Card_t isHide) {
    Sprite* cardFace;
    
    if(isHide==HIDE) {
        cardFace = CreateKind((Card_t)kind,SMALL_BLACK);
    } else {
        cardFace = CreateKind((Card_t)kind,SMALL);
    }
    
    cardFace->setPosition(Vec2(
        parent->getTextureRect().size.width/2, 
        parent->getTextureRect().size.height*0.65));
    cardFace->setRotation(_layout->RotateAngleOfCard(dir));
    cardFace->setScale(0.9);
    parent->addChild(cardFace);/* additional parameter 1 if dir==MIDDLE */
}

/* only for MIDDLE */
void GObjectFactory::LayDownWithFace(Sprite *parent,Card_t kind, float height_x) {
    auto cardFace = CreateKind(kind,NORMAL);
    cardFace->setAnchorPoint(Vec2(0.5,0.5));
    cardFace->setPosition(Vec2(parent->getTextureRect().size.width/2,
        parent->getTextureRect().size.height*height_x));
    parent->addChild(cardFace,1);
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

Sprite *GObjectFactory::CreateRiverCard(PlayerDir_t dir,Card_t kind) {
    TextureId_t texture = (dir==MIDDLE) ? OUT_CARD : LR_OUT_CARD;
    
    auto riverCard = Create(texture);
    auto riverCard_kind = CreateKind(kind,SMALL);

    riverCard_kind->setPosition(riverCard->getTextureRect().size.width/2,riverCard->getTextureRect().size.height*0.65);

    if( dir!=MIDDLE ) {
        riverCard_kind->setRotation(_layout->RotateAngleOfCard(dir));
        riverCard_kind->setAnchorPoint(Vec2(0.5,0.5));
        riverCard_kind->setScale(0.9);

        riverCard->setAnchorPoint(Vec2(0.5,0.5));
    } else {
        riverCard->setAnchorPoint(Vec2(0,1));
    }
    
    riverCard->addChild(riverCard_kind);

    return riverCard;
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

LayerColor *GObjectFactory::CreateTingSignBar(PlayerDir_t dir,Card_t *cards,int cardNum) {
	Vec2 curPos = _layout->PositionOfTingSignBar((PlayerDir_t)dir);

	auto LeftSize  = _image("Leftcorner-other.png")->getTextureRect().size;
	auto CardSize  = _image("Husign-other.png")->getTextureRect().size;
	auto RightSize = _image("Rightangle-other.png")->getTextureRect().size;

	Size SignBarSize = Size(LeftSize.width,LeftSize.height+CardSize.height*(cardNum+1)+RightSize.height);

	auto TingSignBar = LayerColor::create();
	TingSignBar->setAnchorPoint(Vec2(0.5,0.5));
	TingSignBar->setContentSize(SignBarSize);
	TingSignBar->ignoreAnchorPointForPosition(false);
	TingSignBar->setPosition(Vec2(curPos.x,curPos.y));

	if(dir==0)
	{
		auto SignLeft=Sprite::createWithSpriteFrameName("Rightangle-other.png");
		SignLeft->setAnchorPoint(Vec2(0,1));
		SignLeft->setPosition(Vec2(0,SignBarSize.height));
		TingSignBar->addChild(SignLeft,1,1);

		auto SignHuBKG=Sprite::createWithSpriteFrameName("Husign-other.png");
		SignHuBKG->setAnchorPoint(Vec2(0,1));
		SignHuBKG->setPosition(Vec2(0,SignBarSize.height-LeftSize.height));
		auto SignHuFont=Sprite::create("smallHu.png");
		SignHuFont->setRotation(90);
		SignHuFont->setScale(0.7);
		SignHuFont->setAnchorPoint(Vec2(0.5,0.5));
		SignHuFont->setPosition(Vec2(CardSize.width/2,CardSize.height/2));
		SignHuBKG->addChild(SignHuFont,1);
		TingSignBar->addChild(SignHuBKG,1,2);

		int k=0;
		for(k=0;k<cardNum;k++)
		{
			auto SingForCards=Sprite::createWithSpriteFrameName("Husign-other.png");
			SingForCards->setAnchorPoint(Vec2(0,1));
			SingForCards->setPosition(Vec2(0,SignBarSize.height-LeftSize.height-CardSize.height*(k+1)));

			auto WinCard=Create(LR_OUT_CARD);
			WinCard->setAnchorPoint(Vec2(0.5,0.5));
			WinCard->setPosition(Vec2(CardSize.width/2,CardSize.height/2));//  /2  0.65
			auto s_card=CreateKind(cards[k],SMALL);
			s_card->setRotation(90);
			s_card->setScale(0.9);
			s_card->setAnchorPoint(Vec2(0.5,0.5));
			s_card->setPosition(Vec2(
                WinCard->getTextureRect().size.width/2,
                WinCard->getTextureRect().size.height*0.65));
			WinCard->addChild(s_card,1);
			SingForCards->addChild(WinCard,1);
			TingSignBar->addChild(SingForCards,1,k+3);
		}

		auto SignRight=Sprite::createWithSpriteFrameName("Leftcorner-other.png");
		SignRight->setAnchorPoint(Vec2(0,1));
		SignRight->setPosition(Vec2(0,SignBarSize.height-LeftSize.height-CardSize.height*(k+1)));
		TingSignBar->addChild(SignRight,1,k+3);
	}
	else if(dir==2)
	{
		auto SignLeft=Sprite::createWithSpriteFrameName("Leftcorner-other.png");
		SignLeft->setAnchorPoint(Vec2(0,0));
		SignLeft->setPosition(Vec2::ZERO);
		TingSignBar->addChild(SignLeft,1,1);

		auto SignHuBKG=Sprite::createWithSpriteFrameName("Husign-other.png");
		SignHuBKG->setAnchorPoint(Vec2(0,0));
		SignHuBKG->setPosition(Vec2(0,LeftSize.height));
		auto SignHuFont=Sprite::create("smallHu.png");
		SignHuFont->setRotation(-90);
		SignHuFont->setScale(0.7);
		SignHuFont->setAnchorPoint(Vec2(0.5,0.5));
		SignHuFont->setPosition(Vec2(CardSize.width/2,CardSize.height/2));
		SignHuBKG->addChild(SignHuFont,1);
		TingSignBar->addChild(SignHuBKG,1,2);

		int k=0;
		for(k=0;k<cardNum;k++)
		{
			auto SingForCards=Sprite::createWithSpriteFrameName("Husign-other.png");
			SingForCards->setAnchorPoint(Vec2(0,0));
			SingForCards->setPosition(Vec2(0,LeftSize.height+CardSize.height*(k+1)));

			auto WinCard=Create(LR_OUT_CARD);
			WinCard->setAnchorPoint(Vec2(0.5,0.5));
			WinCard->setPosition(Vec2(CardSize.width/2,CardSize.height/2));//  /2  0.65
			auto s_card=CreateKind(cards[k],SMALL);
			s_card->setRotation(-90);
			s_card->setScale(0.9);
			s_card->setAnchorPoint(Vec2(0.5,0.5));
			s_card->setPosition(Vec2(
                WinCard->getTextureRect().size.width/2,
                WinCard->getTextureRect().size.height*0.65));
			WinCard->addChild(s_card,1);
			SingForCards->addChild(WinCard,1);
			TingSignBar->addChild(SingForCards,1,k+3);
		}

		auto SignRight=Sprite::createWithSpriteFrameName("Rightangle-other.png");
		SignRight->setAnchorPoint(Vec2(0,0));
		SignRight->setPosition(Vec2(0,LeftSize.height+CardSize.height*(k+1)));
		TingSignBar->addChild(SignRight,1,k+3);
	}
    
    return TingSignBar;
}

LabelAtlas *GObjectFactory::_createNumberSign(int number) {
    char tempForChar[10] = {0};
    
    sprintf(tempForChar,"%d",number);
    std::string tempForStr = tempForChar;

    auto numberSign = LabelAtlas::create(tempForStr,"fonts/NumMingNotice.png",17, 20, '0');
    numberSign->setAnchorPoint(Vec2(0.5,0.5));
    numberSign->setPosition(Vec2(78,62));

    return numberSign;
}

LayerColor *GObjectFactory::CreateTingInfoBar(Vec2 &referPoint, Card_t *cards,int cardNum,int *winTimes,int *remainingNum) {
	auto tishiLeft = _image("Left corner.png")->getTextureRect().size;
	auto tishiHu   = _image("Hu sign.png")->getTextureRect().size;
	auto tishiCard = _image("Card and Zhang.png")->getTextureRect().size;
	auto tishiEnd  = _image("Right angle.png")->getTextureRect().size;

	Size SignBarSize=Size(tishiLeft.width+tishiHu.width+tishiCard.width*cardNum+tishiEnd.width,tishiLeft.height);
    
	float BarWith=SignBarSize.width;
	float x=referPoint.x;

    Vec2 PositionAdjust=Vec2::ZERO;
	Vec2 PointAdjust=Vec2::ZERO;
	if((x-BarWith/2)>=10&&(x+BarWith/2)<=(SIZE.width-10))
	{
		PointAdjust=Vec2(0.5,0.5);
		PositionAdjust=referPoint;
	}
	else if((x-BarWith/2)<10)
	{
		PointAdjust=Vec2((x-10)/BarWith,0.5);
		PositionAdjust=referPoint;
	}
	else if((x+BarWith/2)>(SIZE.width-10))
	{
		PointAdjust=Vec2(1-(SIZE.width-10-x)/BarWith,0.5);
		PositionAdjust=referPoint;
	}

	auto TingSignBar=LayerColor::create();
	TingSignBar->setAnchorPoint(PointAdjust);
	TingSignBar->setContentSize(SignBarSize);
	TingSignBar->ignoreAnchorPointForPosition(false);
	TingSignBar->setPosition(Vec2(PositionAdjust.x,ORIGIN.y+SIZE.height*0.3));

	auto tipPoint=Sprite::createWithSpriteFrameName("Small triangle.png");
	tipPoint->setAnchorPoint(Vec2(0.5,1));
	//tipPoint->getTexture()->setAliasTexParameters();
	tipPoint->setPosition(Vec2(PointAdjust.x*TingSignBar->getContentSize().width,0.5));
	TingSignBar->addChild(tipPoint,1);

	auto SignHead=Sprite::createWithSpriteFrameName("Left corner.png");//�?
	//SignHead->getTexture()->setAliasTexParameters();
	SignHead->setAnchorPoint(Vec2(0,0));
	SignHead->setPosition(Vec2::ZERO);
	TingSignBar->addChild(SignHead,1,1);

	auto SignHuBKG=Sprite::createWithSpriteFrameName("Hu sign.png");//�?
	SignHuBKG->setAnchorPoint(Vec2(0,0));
	SignHuBKG->setPosition(Vec2(SignHead->getTextureRect().size.width,0));
	auto SignHuFont=Sprite::create("smallHu.png");
	SignHuFont->setScale(0.8);
	SignHuFont->setAnchorPoint(Vec2(0.5,0.5));
	SignHuFont->setPosition(Vec2(ORIGIN.x+SignHuBKG->getTextureRect().size.width/2,ORIGIN.y+SignHuBKG->getTextureRect().size.height/2));
	SignHuBKG->addChild(SignHuFont,1);
	TingSignBar->addChild(SignHuBKG,1,2);

	for(int i=0;i<cardNum;i++)//�?
	{
		auto SignForCards=Sprite::createWithSpriteFrameName("Card and Zhang.png");
		//SignForCards->getTexture()->setAliasTexParameters();
		SignForCards->setAnchorPoint(Vec2(0,0));
		SignForCards->setPosition(Vec2(SignHead->getTextureRect().size.width+SignHuBKG->getTextureRect().size.width+SignForCards->getTextureRect().size.width*i,0));

		auto WinCard=Create(PENG_CARD);
		WinCard->setAnchorPoint(Vec2(0.5,0.5));
		WinCard->setPosition(Vec2(10+WinCard->getTextureRect().size.width/2,43));
		auto s_card=CreateKind(cards[i],MIDDLE_SIZE);
		s_card->setAnchorPoint(Vec2(0.5,0.5));
		s_card->setPosition(WinCard->getTextureRect().size.width/2,WinCard->getTextureRect().size.height*0.6);
		WinCard->addChild(s_card,1);
		SignForCards->addChild(WinCard,1);

        auto winTime = _createNumberSign(winTimes[i]);
        winTime->setPosition(Vec2(78,62));
        SignForCards->addChild(winTime,1,1);

		auto fanFont=Sprite::createWithSpriteFrameName("fan.png");
		fanFont->setAnchorPoint(Vec2(0.5,0.5));
		fanFont->setPosition(Vec2(108,62));
		SignForCards->addChild(fanFont,1);

        auto remainNum = _createNumberSign(remainingNum[i]);
        remainNum->setPosition(Vec2(78,24));
        SignForCards->addChild(remainNum,1,1);

		auto zhangFont=Sprite::createWithSpriteFrameName("zhang.png");
		zhangFont->setAnchorPoint(Vec2(0.5,0.5));
		zhangFont->setPosition(Vec2(108,24));
		SignForCards->addChild(zhangFont,1);

		TingSignBar->addChild(SignForCards,1,3+i);
	}
	auto SignEnd=Sprite::createWithSpriteFrameName("Right angle.png");
	//SignEnd->getTexture()->setAliasTexParameters();
	SignEnd->setAnchorPoint(Vec2(1,0));
	SignEnd->setPosition(Vec2(TingSignBar->getContentSize().width,0));
	TingSignBar->addChild(SignEnd,1,cardNum+3);

    return TingSignBar;
}

Sprite *GObjectFactory::CreateTingSign() {
    auto tingSign = Sprite::createWithSpriteFrameName("fanxingtishi.png");
    tingSign->setAnchorPoint(Vec2(0,0));
    tingSign->setPosition(Vec2(ORIGIN.x+SIZE.width*0.9, ORIGIN.y+SIZE.height*0.2));
    return tingSign;
}

Sprite *GObjectFactory::_image(const char *file) {
    return Sprite::createWithSpriteFrameName(file);
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



