#include "RewardLayer.h"


RewardLayer::RewardLayer(void)
{
}


RewardLayer::~RewardLayer(void)
{
}

bool RewardLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	auto visiableSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	this->setContentSize(Size(visiableSize.width,visiableSize.height*564/716));
	for(int i=0;i<8;i++)
	{
		auto taskBar=Sprite::create("shangchengtiao.png");
		taskBar->setAnchorPoint(Vec2(0,1));
		taskBar->setPosition(origin.x+visiableSize.width*0.0255,origin.y+visiableSize.height*(1.8-i*0.2225));
		this->addChild(taskBar,1);

		auto taskImage=Sprite::createWithSpriteFrameName("renwutupian.png");
		taskImage->setAnchorPoint(Vec2(0,1));
		taskImage->setPosition(origin.x+visiableSize.width*0.04,origin.y+visiableSize.height*(1.77-i*0.2225));
		this->addChild(taskImage,2);

		auto nameOfTask=Sprite::createWithSpriteFrameName("renwumingchengtiao.png");
		nameOfTask->setAnchorPoint(Vec2(0,1));
		nameOfTask->setPosition(origin.x+visiableSize.width*0.1626,origin.y+visiableSize.height*(1.75-i*0.2225));
		this->addChild(nameOfTask,2);

		auto completeTip=Sprite::createWithSpriteFrameName("wanchangtishi.png");
		completeTip->setAnchorPoint(Vec2(0.5,0.5));
		completeTip->setPosition(origin.x+visiableSize.width*0.6,origin.y+visiableSize.height*(1.685-i*0.2225));
		//completeTip->setScale(0.8);
		this->addChild(completeTip,2,i);
		completeTip->setScale(1.5);
		completeTip->setVisible(false);

		auto labelImageGet=Sprite::createWithSpriteFrameName("lingqu.png");
		labelImageGet->setRotation(-90);
		labelImageGet->setAnchorPoint(Vec2(0.5,0.5));
		labelImageGet->setPosition(Vec2(origin.x+visiableSize.width*0.85,origin.y+visiableSize.height*(1.683-i*0.2225)));
		this->addChild(labelImageGet,3);

		auto buttonGet=Button::create("baoming anniu2.png","baoming anniu.png","baoming anniu.png",TextureResType::PLIST);
		buttonGet->_ID=i;
		buttonGet->setPosition(Vec2(origin.x+visiableSize.width*0.85,origin.y+visiableSize.height*(1.683-i*0.2225)));
		buttonGet->setScale(0.8);
		/*buttonGet->setTitleText("领取");
		buttonGet->setTitleFontName("黑体");
		buttonGet->setTitleFontSize(35);*/
		buttonGet->addTouchEventListener(CC_CALLBACK_2(RewardLayer::onButtonGet, this));
		this->addChild(buttonGet,2);
	}

	return true;
}

void RewardLayer::onButtonGet(Ref* pSender,Widget::TouchEventType type)
{
	int i=pSender->_ID;
	auto tip=(Sprite*)this->getChildByTag(i);
	auto button=(Button*)pSender;
	auto easeBounceInOut3= EaseBounceOut::create(ScaleTo::create(0.3,0.8));
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		tip->setVisible(true);
		tip->runAction(easeBounceInOut3);
		button->setTouchEnabled(false);
		button->setHighlighted(true);
		//auto 
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}