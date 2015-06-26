#include "ShopLayer.h"


ShopLayer::ShopLayer(void)
{
	MallGoodList=EnvVariable::getInstance()->get_Mall_GoodList();
}

ShopLayer::~ShopLayer(void)
{
}

bool ShopLayer::init()
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
	
	for(int i=0;i<MallGoodList.size();i++)
	{
		char tempChar[100];
		auto taskBar=Sprite::create("shangchengtiao.png");
		taskBar->setAnchorPoint(Vec2(0,1));
		taskBar->setPosition(visiableSize.width*0.0255,visiableSize.height*(1.8-i*0.2225));
		sc->addChild(taskBar,1);

		auto taskImage=Sprite::createWithSpriteFrameName("renwutupian.png");
		taskImage->setAnchorPoint(Vec2(0,1));
		taskImage->setPosition(visiableSize.width*0.04,visiableSize.height*(1.77-i*0.2225));
		sc->addChild(taskImage,2);

		auto goodIcon=Sprite::createWithSpriteFrameName(MallGoodList[i].photoFileName);
		goodIcon->setAnchorPoint(Vec2(0.5,0.5));
		goodIcon->setPosition(Vec2(taskImage->getContentSize().width/2,taskImage->getContentSize().height/2));
		taskImage->addChild(goodIcon);

		auto nameOfTask=Sprite::createWithSpriteFrameName("renwumingchengtiao.png");
		nameOfTask->setAnchorPoint(Vec2(0,1));
		nameOfTask->setPosition(visiableSize.width*0.1626,visiableSize.height*(1.75-i*0.2225));
		sc->addChild(nameOfTask,2);

		auto goodName=LabelTTF::create(MallGoodList[i].GoodsName,"Arial",20);
		goodName->setAnchorPoint(Vec2(0.5,0.5));
		goodName->setPosition(Vec2(nameOfTask->getContentSize().width/2,nameOfTask->getContentSize().height/2));
		nameOfTask->addChild(goodName);

		auto gooodInstruct=LabelTTF::create(MallGoodList[i].GoodsIntroduct,"Arial",20);
		gooodInstruct->setAnchorPoint(Vec2(0,0));
		gooodInstruct->setPosition(Vec2(nameOfTask->getPosition().x+10,taskImage->getPosition().y-taskImage->getContentSize().height));
		sc->addChild(gooodInstruct,2);

		auto completeTip=Sprite::createWithSpriteFrameName("rexiao.png");
		completeTip->setAnchorPoint(Vec2(0,1));
		completeTip->setPosition(visiableSize.width*0.55,visiableSize.height*(1.74-i*0.2225));
		sc->addChild(completeTip,2);

		sprintf(tempChar,"%d",MallGoodList[i].GooldPrice);
		auto sellGold=LabelTTF::create(tempChar,"Arial",20);
		sellGold->setAnchorPoint(Vec2(0.5,0.5));
		sellGold->setPosition(Vec2(completeTip->getPosition().x+completeTip->getContentSize().width*1.5,completeTip->getPosition().y-completeTip->getContentSize().height/2));
		sc->addChild(sellGold,2);

		auto buttonBuy=Button::create("baoming anniu2.png","baoming anniu.png","baoming anniu.png",TextureResType::PLIST);
		buttonBuy->setPosition(Vec2(visiableSize.width*0.85,visiableSize.height*(1.683-i*0.2225)));
		buttonBuy->setScale(0.8);
		buttonBuy->addTouchEventListener(CC_CALLBACK_2(ShopLayer::onButtonBuy,this));
		sc->addChild(buttonBuy,2);

		auto imageOfBuy=Sprite::create("goumai.png");
		imageOfBuy->setAnchorPoint(Vec2(0,0));
		imageOfBuy->setPosition(visiableSize.width*0.895,visiableSize.height*(1.65-i*0.2225));
		imageOfBuy->setRotation(-90);
		sc->addChild(imageOfBuy,3);
	}

	return true;
}

void ShopLayer::onButtonBuy(Ref* pSender,Widget::TouchEventType type)
{

}