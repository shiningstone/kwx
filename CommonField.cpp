#include "CommonField.h"
#include "ShopScene.h"
#include "EnterRoomScene.h"
#include "BuDaiPiaoLayer.h"
#include "DaiPiaoLayer.h"

CommonField::CommonField(void)
{
}


CommonField::~CommonField(void)
{
}

bool CommonField::init()
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
	selectEffect->setAnchorPoint(Vec2(0,0));
	selectEffect->setPosition(Vec2(origin.x+visibleSize.width*0.41,origin.y+visibleSize.height*0.8687));
	this->addChild(selectEffect,2);
	
	
	auto imageOfCommonField=Sprite::createWithSpriteFrameName("changguichang.png");
	imageOfCommonField->setAnchorPoint(Vec2(0,0));
	imageOfCommonField->setRotation(-90);
	imageOfCommonField->setPosition(Vec2(origin.x+visibleSize.width*0.55,origin.y+visibleSize.height*0.9));
	this->addChild(imageOfCommonField,2);

	auto marketButton=Button::create("mall.png","mall.png","mall.png",TextureResType::PLIST);
	marketButton->setAnchorPoint(Vec2(0,1));
	marketButton->setPosition(Vec2(origin.x+visibleSize.width*0.03,origin.y+visibleSize.height*0.998));
	marketButton->addTouchEventListener(CC_CALLBACK_2(CommonField::onButtonMarket,this));
	this->addChild(marketButton,2);

	auto backButton=Button::create("back.png","back.png","back.png",TextureResType::PLIST);
	backButton->setAnchorPoint(Vec2(1,1));
	backButton->setPosition(Vec2(origin.x+visibleSize.width,origin.y+visibleSize.height*0.998));
	backButton->addTouchEventListener(CC_CALLBACK_2(CommonField::onButtonBack,this));
	this->addChild(backButton,2);

	auto selectBuPiaoEffect=Sprite::createWithSpriteFrameName("moji.png");
	selectBuPiaoEffect->setAnchorPoint(Vec2(0,0));
	selectBuPiaoEffect->setPosition(Vec2(origin.x+visibleSize.width*0.926,origin.y+visibleSize.height*0.48));
	this->addChild(selectBuPiaoEffect,2,1);
	selectBuPiaoEffect->setVisible(true);

	auto selectPiaoEffect=Sprite::createWithSpriteFrameName("moji.png");
	selectPiaoEffect->setAnchorPoint(Vec2(0,0));
	selectPiaoEffect->setPosition(Vec2(origin.x+visibleSize.width*0.926,origin.y+visibleSize.height*0.27));
	this->addChild(selectPiaoEffect,2,2);
	selectPiaoEffect->setVisible(false);

	auto buttonBuPiao=Button::create("bupiao.png","bupiao2.png","bupiao2.png",TextureResType::PLIST);
	buttonBuPiao->setAnchorPoint(Vec2(0,0));
	buttonBuPiao->setPosition(Vec2(origin.x+visibleSize.width*0.94,origin.y+visibleSize.height*0.48));
	buttonBuPiao->addTouchEventListener(CC_CALLBACK_2(CommonField::onButtonBuPiao,this));
	this->addChild(buttonBuPiao,3,3);
	buttonBuPiao->setTouchEnabled(false);
	
	auto buttonDaiPiao=Button::create("daipiao.png","daipiao2.png","daipiao2.png",TextureResType::PLIST);
	buttonDaiPiao->setAnchorPoint(Vec2(0,0));
	buttonDaiPiao->setPosition(Vec2(origin.x+visibleSize.width*0.94,origin.y+visibleSize.height*0.268));
	buttonDaiPiao->addTouchEventListener(CC_CALLBACK_2(CommonField::onButtonDaiPiao,this));
	this->addChild(buttonDaiPiao,3,4);

	auto buDaiPiaoLayer=BuDaiPiaoLayer::create();
	buDaiPiaoLayer->setScale(0);
	this->addChild(buDaiPiaoLayer,2,5);
	buDaiPiaoLayer->runAction(ScaleTo::create(0.3,1));

	return true;
}

void CommonField::onButtonMarket(Ref* pSender,Widget::TouchEventType type)
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

void CommonField::onButtonBack(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			auto scene = Scene::create();
			auto layer = EnterRoom::create();
			scene->addChild(layer);
			Director::getInstance()->replaceScene(scene);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void CommonField::onButtonBuPiao(Ref* pSender,Widget::TouchEventType type)
{
	auto buDaiPiaoLayer=BuDaiPiaoLayer::create();
	auto buttonBuPiao=(Button *)this->getChildByTag(3);
	auto buttonDaiPiao=(Button *)this->getChildByTag(4);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		this->removeChildByTag(5);
		this->addChild(buDaiPiaoLayer,2,5);
		this->getChildByTag(1)->setVisible(true);
		this->getChildByTag(2)->setVisible(false);
		buttonBuPiao->setTouchEnabled(false);
		buttonDaiPiao->setTouchEnabled(true);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void CommonField::onButtonDaiPiao(Ref* pSender,Widget::TouchEventType type)
{
	auto daiPiaoLayer=DaiPiaoLayer::create();
	auto buttonBuPiao=(Button *)this->getChildByTag(3);
	auto buttonDaiPiao=(Button *)this->getChildByTag(4);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		this->removeChildByTag(5);
		this->addChild(daiPiaoLayer,2,5);
		this->getChildByTag(1)->setVisible(false);
		this->getChildByTag(2)->setVisible(true);
		buttonBuPiao->setTouchEnabled(true);
		buttonDaiPiao->setTouchEnabled(false);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}