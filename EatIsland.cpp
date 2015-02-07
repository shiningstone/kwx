#include "EatIsland.h"


EatIsland::EatIsland(void)
{
}


EatIsland::~EatIsland(void)
{
}

bool EatIsland::init()
{
	if(!Layer::init())
	{
		return false;
	}
	auto visiableSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	this->setContentSize(Size(visiableSize.width,visiableSize.height*564/716));
	
	auto sc = ui::ScrollView::create(); 
	sc->setSize(Size(visiableSize.width,visiableSize.height*0.73));
	sc->setBounceEnabled(true);
	sc->setDirection(ui::ScrollView::Direction::VERTICAL); 
	sc->setPosition(Vec2(origin.x,origin.y+visiableSize.height*0.1));
	this->addChild(sc,4);
	sc->setInnerContainerSize(Size(visiableSize.width,visiableSize.height*1.8));
	
	for(int i=0;i<8;i++)
	{
		auto taskBar=Sprite::create("shangchengtiao.png");
		taskBar->setAnchorPoint(Vec2(0,1));
		taskBar->setPosition(visiableSize.width*0.0255,visiableSize.height*(1.8-i*0.2225));
		sc->addChild(taskBar,1);

		auto taskImage=Sprite::createWithSpriteFrameName("renwutupian.png");
		taskImage->setAnchorPoint(Vec2(0,1));
		taskImage->setPosition(visiableSize.width*0.04,visiableSize.height*(1.77-i*0.2225));
		sc->addChild(taskImage,2);

		auto nameOfTask=Sprite::createWithSpriteFrameName("renwumingchengtiao.png");
		nameOfTask->setAnchorPoint(Vec2(0,1));
		nameOfTask->setPosition(visiableSize.width*0.1626,visiableSize.height*(1.75-i*0.2225));
		sc->addChild(nameOfTask,2);

		auto completeTip=Sprite::createWithSpriteFrameName("rexiao.png");
		completeTip->setAnchorPoint(Vec2(0,1));
		completeTip->setPosition(visiableSize.width*0.55,visiableSize.height*(1.74-i*0.2225));
		sc->addChild(completeTip,2);

		auto buttonBuy=Button::create("baoming anniu2.png","baoming anniu.png","baoming anniu.png",TextureResType::PLIST);
		buttonBuy->setPosition(Vec2(visiableSize.width*0.85,visiableSize.height*(1.683-i*0.2225)));
		buttonBuy->setScale(0.8);
		buttonBuy->addTouchEventListener(CC_CALLBACK_2(EatIsland::onButtonBuy,this));
		sc->addChild(buttonBuy,2);

		auto imageOfBuy=Sprite::create("goumai.png");
		imageOfBuy->setAnchorPoint(Vec2(0,0));
		imageOfBuy->setPosition(visiableSize.width*0.895,visiableSize.height*(1.65-i*0.2225));
		imageOfBuy->setRotation(-90);
		sc->addChild(imageOfBuy,3);
	}

	return true;
}

void EatIsland::onButtonBuy(Ref* pSender,Widget::TouchEventType type)
{

}