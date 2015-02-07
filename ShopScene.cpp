#include "ShopScene.h"
#include "EnterRoomScene.h"
#include "ShopLayer.h"
#include "EatIsland.h"


ShopScene::ShopScene(void)
{
}


ShopScene::~ShopScene(void)
{
}

bool ShopScene::init()
{
	if(!Scene::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float s_scale=1.189f;
   
	auto bg=Sprite::create("602.png");
	bg->setAnchorPoint(Vec2(0.5,0.5));
	bg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	bg->setScaleX(s_scale);
	bg->setScaleY(s_scale);
	this->addChild(bg,0);

	auto paddle=LayerColor::create(Color4B(47,86,110,100));
	paddle->setContentSize(Size(origin.x+visibleSize.width,origin.y+visibleSize.height*0.7877));
	paddle->setPosition(Vec2(origin.x,origin.y+visibleSize.height*0.0656));
	this->addChild(paddle,1);

	auto selectEffectOfInfo=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffectOfInfo->setAnchorPoint(Vec2(0,0));
	selectEffectOfInfo->setPosition(origin.x+visibleSize.width*0.3552,origin.y+visibleSize.height*0.866);
	this->addChild(selectEffectOfInfo,1,1);
	selectEffectOfInfo->setVisible(true);

	auto selectEffectOfCheatCode=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffectOfCheatCode->setAnchorPoint(Vec2(0,0));
	selectEffectOfCheatCode->setPosition(origin.x+visibleSize.width*0.6445,origin.y+visibleSize.height*0.866);
	this->addChild(selectEffectOfCheatCode,1,2);
	selectEffectOfCheatCode->setVisible(false);
	

	auto buttonSutraHall=Button::create("cangjingge.png","cangjingge2.png","cangjingge2.png",TextureResType::PLIST);
	buttonSutraHall->setAnchorPoint(Vec2(0,0));
	buttonSutraHall->setPosition(Vec2(origin.x+visibleSize.width*0.3678,origin.y+visibleSize.height*0.9));
	buttonSutraHall->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonSutraHall, this));
	this->addChild(buttonSutraHall,3);

	auto buttonEatIsland=Button::create("tongchidao.png","tongchidao.png","tongchidao.png",TextureResType::PLIST);
	buttonEatIsland->setAnchorPoint(Vec2(0,0));
	buttonEatIsland->setPosition(Vec2(origin.x+visibleSize.width*0.6568,origin.y+visibleSize.height*0.9));
	buttonEatIsland->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonEatIsland, this));
	this->addChild(buttonEatIsland,3);

	auto buttonBack=Button::create("12.png","12.png","12.png",TextureResType::LOCAL);
	buttonBack->setAnchorPoint(Vec2(0,0));
	buttonBack->setPosition(Vec2(origin.x+visibleSize.width*0.92,origin.y+visibleSize.height*0.89));
	buttonBack->addTouchEventListener(CC_CALLBACK_2(ShopScene::onButtonBack, this));
	this->addChild(buttonBack,3);

	auto flower=Sprite::createWithSpriteFrameName("meihua.png");
	auto sizeOfFlower=flower->getContentSize();
	flower->setAnchorPoint(Vec2(1,0));
	flower->setPosition(Vec2(origin.x+visibleSize.width*1.0095,origin.y-visibleSize.height*0.028));
	this->addChild(flower,0);

	auto gold=Sprite::createWithSpriteFrameName("jinbi.png");
	gold->setAnchorPoint(Vec2(0,0));
	gold->setPosition(Vec2(origin.x+visibleSize.width*0.0246,origin.y+visibleSize.height*.887));
	gold->setScale(0.9);
	this->addChild(gold,4);

	auto goldBar=Sprite::createWithSpriteFrameName("bisaijifentiao.png");
	goldBar->setAnchorPoint(Vec2(0,0));
	goldBar->setPosition(Vec2(origin.x+visibleSize.width*0.037,origin.y+visibleSize.height*0.89));
	this->addChild(goldBar,3);

	auto shopLayer=ShopLayer::create();
	this->addChild(shopLayer,3,3);

	return true;
}

void ShopScene::onButtonBack(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Director::getInstance()->popScene();
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void ShopScene::onButtonSutraHall(Ref* pSender,Widget::TouchEventType type)
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto effectOnSutraHall=this->getChildByTag(1);
	auto effectOnEatIsland=this->getChildByTag(2);
	auto shopLayer=ShopLayer::create();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		this->removeChildByTag(3);
		this->addChild(shopLayer,3,3);
		effectOnEatIsland->setVisible(false);
		effectOnSutraHall->setVisible(true);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void ShopScene::onButtonEatIsland(Ref* pSender,Widget::TouchEventType type)
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto effectOnSutraHall=this->getChildByTag(1);
	auto effectOnEatIsland=this->getChildByTag(2);
	auto eatIslandLayer=EatIsland::create();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		this->removeChildByTag(3);
		this->addChild(eatIslandLayer,3,3);
		effectOnEatIsland->setVisible(true);
		effectOnSutraHall->setVisible(false);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}