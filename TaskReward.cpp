#include "TaskReward.h"
#include "EnterRoomScene.h"
#include "ChangeScene.h"
#include "RewardLayer.h"

TaskReward::TaskReward(void)
{
}


TaskReward::~TaskReward(void)
{
}

bool TaskReward::init()
{
	if(!Scene::create())
	{
		return false;
	}
	auto visiableSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	float s_scale=1.189f;

	auto bg=Sprite::create("602.png");
	bg->setPosition(Vec2(origin.x+visiableSize.width/2,origin.y+visiableSize.height/2));
	bg->setScaleX(s_scale);
	bg->setScaleY(s_scale);
	this->addChild(bg,0);

	auto paddle=LayerColor::create(Color4B(47,86,110,100));
	auto sizeOfPaddle=paddle->getContentSize();
	paddle->setContentSize(Size(origin.x+visiableSize.width,origin.y+visiableSize.height*564/716));
	paddle->setPosition(Vec2(0,47));
	this->addChild(paddle,1);

	auto selectEffectOfInfo=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffectOfInfo->setAnchorPoint(Vec2(0.5,0.5));
	selectEffectOfInfo->setPosition(origin.x+visiableSize.width*0.5,origin.y+visiableSize.height*0.93);
	this->addChild(selectEffectOfInfo,1,1);
	selectEffectOfInfo->setVisible(true);

	auto buttonItemBack1 = Sprite::create("12.png"); 
	auto buttonItemBack2 = Sprite::create("12.png"); 
	auto itemOfMenuBack = MenuItemSprite::create(buttonItemBack1,buttonItemBack2,CC_CALLBACK_1(TaskReward::buttonBackToManage, this));
	auto sizeOfBack=itemOfMenuBack->getContentSize();
	auto buttonBack = Menu::create(itemOfMenuBack,NULL);
	buttonBack->setPosition(((sizeOfBack.width/2+1120)/1218)*visiableSize.width+origin.x,((716-12-sizeOfBack.height/2)/716)*visiableSize.height+origin.y);
	this->addChild(buttonBack, 3);


	auto titleOfActivity=Sprite::createWithSpriteFrameName("renwujiangli.png");
	titleOfActivity->setAnchorPoint(Vec2(0.5,0.5));
	titleOfActivity->setPosition(origin.x+0.5*visiableSize.width,origin.y+visiableSize.height*0.93);
	this->addChild(titleOfActivity,3);

	auto flower=Sprite::createWithSpriteFrameName("meihua.png");
	auto sizeOfFlower=flower->getContentSize();
	flower->setAnchorPoint(Vec2(1,0));
	flower->setPosition(origin.x+visiableSize.width*1.0095,origin.y-visiableSize.height*0.028);
	this->addChild(flower,0);

	auto containLayer=RewardLayer::create();
	auto sc = ui::ScrollView::create(); 
	sc->setSize(Size(visiableSize.width,visiableSize.height*0.73));
	sc->setBounceEnabled(true);
	sc->setDirection(ui::ScrollView::Direction::VERTICAL); 
	sc->setPosition(Vec2(origin.x,origin.y+visiableSize.height*0.1));
	this->addChild(sc,4);
	sc->setInnerContainerSize(Size(visiableSize.width,visiableSize.height*1.8));
	sc->addChild(containLayer);
	

	return true;
}

void TaskReward::buttonBackToManage(Ref* pSender)
{
	Director::getInstance()->popScene();
}