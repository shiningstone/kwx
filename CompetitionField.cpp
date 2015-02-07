#include "CompetitionField.h"
#include "EnterRoomScene.h"
#include "ShopScene.h"
#include "CompetitionFieldLayer.h"


CompetitionField::CompetitionField(void)
{
}


CompetitionField::~CompetitionField(void)
{
}

bool CompetitionField::init()
{
	if(!Scene::init())
	{
		return false;
	}

	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	float s_scale=1.189f;

	auto bg=Sprite::create("602.png");
	bg->setAnchorPoint(Vec2(0.5,0.5));
	bg->setScaleX(s_scale);
	bg->setScaleY(s_scale);
	bg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(bg,0);

	auto paddle=LayerColor::create(Color4B(47,86,110,100));
	paddle->setContentSize(Size(origin.x+visibleSize.width,origin.y+visibleSize.height*0.1396));
	paddle->setAnchorPoint(Vec2(0,1));
	paddle->setPosition(Vec2(origin.x,origin.y+visibleSize.height*0.865));
	this->addChild(paddle,1);

	auto selectEffect=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffect->setAnchorPoint(Vec2(0.5,0));
	selectEffect->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.8687));
	this->addChild(selectEffect,2);
	
	auto imageOfCompetitionField=Sprite::createWithSpriteFrameName("bisaichang.png");
	imageOfCompetitionField->setAnchorPoint(Vec2(0.5,0));
	imageOfCompetitionField->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.9));
	this->addChild(imageOfCompetitionField,3);

	auto marketButton=Button::create("mall.png","mall.png","mall.png",TextureResType::PLIST);
	marketButton->setAnchorPoint(Vec2(0,1));
	marketButton->setPosition(Vec2(origin.x+visibleSize.width*0.03,origin.y+visibleSize.height*0.998));
	marketButton->addTouchEventListener(CC_CALLBACK_2(CompetitionField::onButtonMarket,this));
	this->addChild(marketButton,2);

	auto backButton=Button::create("back.png","back.png","back.png",TextureResType::PLIST);
	backButton->setAnchorPoint(Vec2(1,1));
	backButton->setPosition(Vec2(origin.x+visibleSize.width,origin.y+visibleSize.height*0.998));
	backButton->addTouchEventListener(CC_CALLBACK_2(CompetitionField::onButtonBack,this));
	this->addChild(backButton,2);

	auto helpButton=Button::create("bangzhu5.png","bangzhu5.png","bangzhu5.png",TextureResType::PLIST);
	helpButton->setAnchorPoint(Vec2(0,1));
	helpButton->setPosition(Vec2(origin.x+visibleSize.width*0.13,origin.y+visibleSize.height*0.998));
	helpButton->addTouchEventListener(CC_CALLBACK_2(CompetitionField::onButtonHelp,this));
	this->addChild(helpButton,2);

	auto competitionField=CompetitionFieldLayer::create();
	competitionField->setScale(0);
	this->addChild(competitionField,2);
	competitionField->runAction(ScaleTo::create(0.3,1));

	return true;
}

void CompetitionField::onButtonMarket(Ref* pSender,Widget::TouchEventType type)
{
	auto shop=ShopScene::create();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Director::getInstance()->pushScene(shop);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void CompetitionField::onButtonBack(Ref* pSender,Widget::TouchEventType type)
{
    auto layer = EnterRoom::create();
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
void CompetitionField::onButtonHelp(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}