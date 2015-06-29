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

	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();
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
	selectBuPiaoEffect->setVisible(true);
	selectBuPiaoEffect->setAnchorPoint(Vec2(0,0));
	selectBuPiaoEffect->setPosition(Vec2(origin.x+visibleSize.width*0.926,origin.y+visibleSize.height*0.48));
	this->addChild(selectBuPiaoEffect,2,NO_PIAO_CP_MOJI);

	auto selectPiaoEffect=Sprite::createWithSpriteFrameName("moji.png");
	selectPiaoEffect->setVisible(false);
	selectPiaoEffect->setAnchorPoint(Vec2(0,0));
	selectPiaoEffect->setPosition(Vec2(origin.x+visibleSize.width*0.926,origin.y+visibleSize.height*0.27));
	this->addChild(selectPiaoEffect,2,WITH_PIAO_CP_MJ);

	auto buttonBuPiao=Button::create("bupiao.png","bupiao2.png","bupiao2.png",TextureResType::PLIST);
	buttonBuPiao->setTouchEnabled(false);
	buttonBuPiao->setAnchorPoint(Vec2(0,0));
	buttonBuPiao->setPosition(Vec2(origin.x+visibleSize.width*0.94,origin.y+visibleSize.height*0.48));
	buttonBuPiao->addTouchEventListener(CC_CALLBACK_2(CommonField::onButtonBuPiao,this));
	this->addChild(buttonBuPiao,3,NO_PIAO_BUTTON_);
	
	auto buttonDaiPiao=Button::create("daipiao.png","daipiao2.png","daipiao2.png",TextureResType::PLIST);
	buttonDaiPiao->setHighlighted(true);
	buttonDaiPiao->setAnchorPoint(Vec2(0,0));
	buttonDaiPiao->setPosition(Vec2(origin.x+visibleSize.width*0.94,origin.y+visibleSize.height*0.268));
	buttonDaiPiao->addTouchEventListener(CC_CALLBACK_2(CommonField::onButtonDaiPiao,this));
	this->addChild(buttonDaiPiao,3,WITH_PIAO_BUTTO);

	auto buDaiPiaoLayer=BuDaiPiaoLayer::create();
	buDaiPiaoLayer->setScale(0);
	buDaiPiaoLayer->ignoreAnchorPointForPosition(false);
	buDaiPiaoLayer->setAnchorPoint(Vec2(0.5,0));
	buDaiPiaoLayer->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y/*+visibleSize.height*0.15*/));
	this->addChild(buDaiPiaoLayer,2,CURREN_UI_LAYER);
	buDaiPiaoLayer->runAction(ScaleTo::create(0.3,1));

	return true;
}

void CommonField::onButtonMarket(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			auto shop=ShopScene::create();
			Director::getInstance()->pushScene(shop);
		}
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
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		{
			auto curButton=(Button*)pSender;
			curButton->setTouchEnabled(false);
			curButton->setHighlighted(false);
			auto curMoji=this->getChildByTag(NO_PIAO_CP_MOJI);
			curMoji->setVisible(true);

			auto AnotherBt=(Button*)this->getChildByTag(WITH_PIAO_BUTTO);
			AnotherBt->setTouchEnabled(true);
			AnotherBt->setHighlighted(true);
			auto anotherMJ=this->getChildByTag(WITH_PIAO_CP_MJ);
			anotherMJ->setVisible(false);

			auto buDaiPiaoLayer=BuDaiPiaoLayer::create();
			//buDaiPiaoLayer->setScale(0.8);
			buDaiPiaoLayer->ignoreAnchorPointForPosition(false);
			buDaiPiaoLayer->setAnchorPoint(Vec2(0.5,0));
			buDaiPiaoLayer->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y/*+visibleSize.height*0.15*/));
			this->removeChildByTag(CURREN_UI_LAYER,true);
			this->addChild(buDaiPiaoLayer,2,CURREN_UI_LAYER);
		}
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

void CommonField::onButtonDaiPiao(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		{
			auto curButton=(Button*)pSender;
			curButton->setTouchEnabled(false);
			curButton->setHighlighted(false);
			auto curMoji=this->getChildByTag(WITH_PIAO_CP_MJ);
			curMoji->setVisible(true);

			auto AnotherBt=(Button*)this->getChildByTag(NO_PIAO_BUTTON_);
			AnotherBt->setTouchEnabled(true);
			AnotherBt->setHighlighted(true);
			auto anotherMJ=this->getChildByTag(NO_PIAO_CP_MOJI);
			anotherMJ->setVisible(false);

			auto daiPiaoLayer=DaiPiaoLayer::create();
			daiPiaoLayer->ignoreAnchorPointForPosition(false);
			daiPiaoLayer->setAnchorPoint(Vec2(0.5,0));
			daiPiaoLayer->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y/*+visibleSize.height*0.15*/));
			this->removeChildByTag(CURREN_UI_LAYER,true);
			this->addChild(daiPiaoLayer,2,CURREN_UI_LAYER);
		}
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