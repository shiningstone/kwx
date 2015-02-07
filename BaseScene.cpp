#include "EnterRoomScene.h"
#include "BaseScene.h"
#include "MyBaseInformation.h"
#include "MyXbox.h"


BaseScene::BaseScene(void)
{
}


BaseScene::~BaseScene(void)
{
}

bool BaseScene::init()
{
	if(!Scene::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float s_scale=1.189f;
   
	auto bg=Sprite::create("602.png");
	bg->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.5));
	bg->setScaleX(s_scale);
	bg->setScaleY(s_scale);
	this->addChild(bg,0);

	auto paddle=LayerColor::create(Color4B(47,86,110,100));
	paddle->setContentSize(Size(visibleSize.width,visibleSize.height*0.7877));
	paddle->setPosition(Vec2(origin.x,origin.y+visibleSize.height*0.0656));
	this->addChild(paddle,1);

	auto selectEffectOfInfo=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffectOfInfo->setAnchorPoint(Vec2(0,0));
	selectEffectOfInfo->setPosition(Vec2(origin.x+visibleSize.width*0.264,origin.y+visibleSize.height*0.8659));
	this->addChild(selectEffectOfInfo,1,1);
	selectEffectOfInfo->setVisible(true);

	auto selectEffectOfCheatCode=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffectOfCheatCode->setAnchorPoint(Vec2(0,0));
	selectEffectOfCheatCode->setPosition(Vec2(origin.x+visibleSize.width*0.5629,origin.y+visibleSize.height*0.8659));
	this->addChild(selectEffectOfCheatCode,1,2);
	selectEffectOfCheatCode->setVisible(false);
	
	auto backButton=Button::create("12.png","12.png","12.png",TextureResType::LOCAL);
	backButton->setAnchorPoint(Vec2(1,1));
	backButton->setPosition(Vec2(origin.x+visibleSize.width,origin.y+visibleSize.height*0.98));
	backButton->addTouchEventListener(CC_CALLBACK_2(BaseScene::buttonBack,this));
	this->addChild(backButton,1);

	auto buttonBaseInfo=Button::create("jibenxinxi1.png","jibenxinxi.png","jibenxinxi.png",TextureResType::PLIST);
	buttonBaseInfo->setAnchorPoint(Vec2(0,0));
	buttonBaseInfo->setPosition(Vec2(origin.x+visibleSize.width*0.268,origin.y+visibleSize.height*0.9));
	buttonBaseInfo->addTouchEventListener(CC_CALLBACK_2(BaseScene::buttonMyInfo, this));
	this->addChild(buttonBaseInfo,2,3);
	buttonBaseInfo->setTouchEnabled(false);

	auto buttonMyXbox=Button::create("wodebeibao.png","wodebeibao2.png","wodebeibao2.png",TextureResType::PLIST);
	buttonMyXbox->setAnchorPoint(Vec2(0,0));
	buttonMyXbox->setPosition(Vec2(origin.x+visibleSize.width*0.5669,origin.y+visibleSize.height*0.9));
	buttonMyXbox->addTouchEventListener(CC_CALLBACK_2(BaseScene::buttonMyXbox, this));
	this->addChild(buttonMyXbox,2,4);

	auto infoLayer=MyBaseInformation::create();
	this->addChild(infoLayer,3,5);
	
	return true;
}

void BaseScene::buttonBack(Ref* pSender,Widget::TouchEventType type)
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

void BaseScene::buttonMyInfo(Ref* pSender,Widget::TouchEventType type)
{
	auto imageOfEffectInfo=this->getChildByTag(1);
	auto imageOfEffectXbox=this->getChildByTag(2);
	auto infoLayer=MyBaseInformation::create();
	auto buttonInfo=(Button*)this->getChildByTag(3);
	auto buttonXbox=(Button*)this->getChildByTag(4);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		imageOfEffectInfo->setVisible(true);
		imageOfEffectXbox->setVisible(false);
		this->removeChildByTag(5,true);
		this->addChild(infoLayer,3,5);
		buttonInfo->setTouchEnabled(false);
		buttonXbox->setTouchEnabled(true);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void BaseScene::buttonMyXbox(Ref* pSender,Widget::TouchEventType type)
{
	auto imageOfEffectInfo=this->getChildByTag(1);
	auto imageOfEffectXbox=this->getChildByTag(2);
	auto xBox=MyXbox::create();
	auto buttonInfo=(Button*)this->getChildByTag(3);
	auto buttonXbox=(Button*)this->getChildByTag(4);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		imageOfEffectInfo->setVisible(false);
		imageOfEffectXbox->setVisible(true);
		this->removeChildByTag(5,true);
		this->addChild(xBox,3,5);
		buttonInfo->setTouchEnabled(true);
		buttonXbox->setTouchEnabled(false);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}