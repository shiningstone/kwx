#include "ActivityCenter.h"
#include "ActivityLayer.h"
#include "EnterRoomScene.h"

ActivityCenter::ActivityCenter(void)
{
}


ActivityCenter::~ActivityCenter(void)
{
}

bool ActivityCenter::init()
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
	bg->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.5));
	bg->setScaleX(s_scale);
	bg->setScaleY(s_scale);
	this->addChild(bg,0);

	auto paddle=LayerColor::create(Color4B(47,86,110,100));
	auto sizeOfPaddle=paddle->getContentSize();
	paddle->setContentSize(Size(visibleSize.width,visibleSize.height*0.7877));
	paddle->setAnchorPoint(Vec2(0,0));
	paddle->setPosition(Vec2(origin.x,origin.y+visibleSize.height*0.0656));
	this->addChild(paddle,1);

	auto selectEffectOfInfo=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffectOfInfo->setAnchorPoint(Vec2(0.5,0.5));
	selectEffectOfInfo->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.93));
	this->addChild(selectEffectOfInfo,1,1);
	selectEffectOfInfo->setVisible(true);

	auto backButton=Button::create("12.png","12.png","12.png",TextureResType::LOCAL);
	backButton->setAnchorPoint(Vec2(1,1));
	backButton->setPosition(Vec2(origin.x+visibleSize.width,origin.y+visibleSize.height*0.98));
	backButton->addTouchEventListener(CC_CALLBACK_2(ActivityCenter::onButtonBack,this));
	this->addChild(backButton,2);

	auto titleOfActivity=Sprite::createWithSpriteFrameName("huodongzhongxin.png");
	titleOfActivity->setAnchorPoint(Vec2(0.5,0.5));
	titleOfActivity->setPosition(Vec2(origin.x+0.5*visibleSize.width,origin.y+visibleSize.height*0.93));
	this->addChild(titleOfActivity,3);

	auto flower=Sprite::createWithSpriteFrameName("meihua.png");
	auto sizeOfFlower=flower->getContentSize();
	flower->setAnchorPoint(Vec2(1,0));
	flower->setPosition(Vec2(origin.x+visibleSize.width*1.0095,origin.y-visibleSize.height*0.028));
	this->addChild(flower,0);

	auto photoFrame=Sprite::create("huodongzhonxinkuang.png");
	photoFrame->setAnchorPoint(Vec2(0,0));
	photoFrame->setPosition(Vec2(origin.x+visibleSize.width*0.0082,origin.y+visibleSize.height*0.0698));
	this->addChild(photoFrame,1);

	auto activityLayer=ActivityLayer::create();
	this->addChild(activityLayer,3);

	return true;
}

void ActivityCenter::onButtonBack(Ref* pSender,Widget::TouchEventType type)
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