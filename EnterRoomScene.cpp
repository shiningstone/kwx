#include "EnterRoomScene.h"
#include "BaseScene.h"
#include "MyInfoScene.h"
#include "ActivityCenter.h"
#include "TaskReward.h"
#include "SetScene.h"
#include "ShopScene.h"
#include "CommonField.h"
#include "CompetitionField.h"
#include "Wait.h"
#include "RankingScroll.h"
#include "dayByDay.h"

EnterRoom::EnterRoom()
{
	s_scale=1.189;
	s_no=1;
}

EnterRoom::~EnterRoom()
{
}

void EnterRoom::message_callback(Ref* pSender,Widget::TouchEventType type)
{
	auto MyInfo=MyInfoScene::create();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Director::getInstance()->pushScene(MyInfo);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}
void EnterRoom::prize_callback(Ref* pSender,Widget::TouchEventType type)
{
	auto taskReward=TaskReward::create();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Director::getInstance()->pushScene(taskReward);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}
void EnterRoom::rank_callback(Ref* pSender,Widget::TouchEventType type)
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto paddle=LayerColor::create(Color4B(0,0,0,200));
	paddle->setContentSize(Size(visibleSize.width,visibleSize.height));
	paddle->setAnchorPoint(Vec2(0,0));
	paddle->setPosition(Vec2(origin.x,origin.y));

	auto sc=ui::ScrollView::create();
	sc->setSize(visibleSize);
	sc->addChild(paddle);

	auto rankLayer=RankingScroll::create();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		this->addChild(sc,4,23);
		this->addChild(rankLayer,5,201);
		sc->addTouchEventListener(CC_CALLBACK_2(EnterRoom::rankBack_callback,this));
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}

void EnterRoom::rankBack_callback(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	auto sc=(ui::ScrollView*)pSender;
	auto endPoint=sc->getTouchEndPos();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		if(endPoint.x<visibleSize.width*0.12||endPoint.x>visibleSize.width*0.88)
		{
			this->removeChildByTag(201);
			scheduleOnce(schedule_selector(EnterRoom::deletePaddle),0.1f);
		}
		else if(endPoint.y<visibleSize.height*0.1||endPoint.y>visibleSize.height*0.9)
		{
			this->removeChildByTag(201);
			scheduleOnce(schedule_selector(EnterRoom::deletePaddle),0.1f);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}

void EnterRoom::active_callback(Ref* pSender,Widget::TouchEventType type)
{
	auto activityScene=ActivityCenter::create();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Director::getInstance()->pushScene(activityScene);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}
void EnterRoom::mall_callback(Ref* pSender,Widget::TouchEventType type)
{
	auto shopScene=ShopScene::create();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Director::getInstance()->pushScene(shopScene);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}

}
void EnterRoom::set_callback(Ref* pSender,Widget::TouchEventType type)
{
	auto setScene=SetScene::create();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Director::getInstance()->pushScene(setScene);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}

void EnterRoom::back_callback(Ref* pSender,Widget::TouchEventType type)
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
	}
}

void EnterRoom::goldreceiveCallback(Ref* pSender)
{
	cur_status=1;
	UserDefault::getInstance()->setIntegerForKey("cur_sta",cur_status);
	UserDefault::getInstance()->flush();
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this->getChildByTag(100),false);
	this->removeChildByTag(100,true);
}

void EnterRoom::removeDayReward()
{
	this->removeChildByTag(31);
	startEmitter();
	scheduleOnce(schedule_selector(EnterRoom::stopEmitter),1);
	scheduleOnce(schedule_selector(EnterRoom::deletePaddle1),2);
}

void EnterRoom::call_gold_prize()
{
	Size visibleSize=Director::getInstance()->getVisibleSize();//大小和原点
	Vec2 origin=Director::getInstance()->getVisibleOrigin();

	auto paddle=LayerColor::create(Color4B(0,0,0,200));
	paddle->setContentSize(Size(visibleSize.width,visibleSize.height));
	paddle->setAnchorPoint(Vec2(0,0));
	paddle->setPosition(Vec2(origin.x,origin.y));

	auto sc=ui::ScrollView::create();
	sc->setSize(visibleSize);
	sc->addChild(paddle);
	this->addChild(sc,4,24);

	auto layerPerDay=new dayAward(this);
	this->addChild(layerPerDay,5,31);
}

bool EnterRoom::init()
{
	if(!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	Sprite *sprite;
	if(s_no == 1)
		sprite = Sprite::create("loadwin.png");
	else
		sprite = Sprite::create("loadwin2.png");
	// position the sprite on the center of the screen
	sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	sprite->setScaleX(s_scale);
	sprite->setScaleY(s_scale);
	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
	
	auto buttonFreeBox=Button::create("free_box.png","free_box.png","free_box.png",TextureResType::PLIST);
	buttonFreeBox->setAnchorPoint(Vec2(0,0));
	buttonFreeBox->setScale(1.03);
	buttonFreeBox->setPosition(Vec2(origin.x-visibleSize.height*0.005,origin.y+visibleSize.height*0.84));
	buttonFreeBox->addTouchEventListener(CC_CALLBACK_2(EnterRoom::myBaseInfo_callback,this));
	this->addChild(buttonFreeBox,1);

	auto button_bar= Sprite::createWithSpriteFrameName("button_bar.png");
	button_bar->setAnchorPoint(Point(1,0));
	button_bar->setPosition(Point(origin.x+visibleSize.width*1.001,origin.y+visibleSize.height*0.838));
	this->addChild(button_bar,1);
	
	
	float x= origin.x;
	float y= visibleSize.height+ origin.y;
	/////////////////////////////////////////////////////////////////////

	auto goodNameBar=Sprite::createWithSpriteFrameName("shang.png");
	goodNameBar->setAnchorPoint(Vec2(0,1));
	goodNameBar->setPosition(Vec2(origin.x+visibleSize.width*0.1065,origin.y+visibleSize.height*0.997));
	this->addChild(goodNameBar,1);

	auto goodName=Sprite::createWithSpriteFrameName("812.png");
	goodName->setAnchorPoint(Vec2(0,1));
	goodName->setPosition(Vec2(origin.x+visibleSize.width*0.105,origin.y+visibleSize.height*0.999));
	this->addChild(goodName,2);

	auto competeBar=Button::create("middle.png","middle.png","middle.png",TextureResType::PLIST);
	competeBar->setAnchorPoint(Vec2(0,1));
	competeBar->setPosition(Vec2(origin.x+visibleSize.width*0.1065,origin.y+visibleSize.height*0.9423));
	competeBar->addTouchEventListener(CC_CALLBACK_2(EnterRoom::mall_callback,this));
	this->addChild(competeBar,1);

	auto compete=Sprite::createWithSpriteFrameName("jinbi.png");
	compete->setAnchorPoint(Vec2(0,1));
	compete->setPosition(Vec2(origin.x+visibleSize.width*0.107,origin.y+visibleSize.height*0.94));
	compete->setScale(0.5f);
	this->addChild(compete,2);

	auto buttonPlus=Sprite::createWithSpriteFrameName("jiahao.png");
	buttonPlus->setAnchorPoint(Vec2(0,1));
	buttonPlus->setPosition(Vec2(origin.x+visibleSize.width*0.2632,origin.y+visibleSize.height*0.943));
	this->addChild(buttonPlus,2);

	auto TaskBar=Sprite::createWithSpriteFrameName("xia.png");
	TaskBar->setAnchorPoint(Vec2(0,1));
	TaskBar->setPosition(Vec2(origin.x+visibleSize.width*0.1065,origin.y+visibleSize.height*0.891));
	this->addChild(TaskBar,1);

	auto task=Sprite::createWithSpriteFrameName("juan.png");
	task->setAnchorPoint(Vec2(0,1));
	task->setPosition(Vec2(origin.x+visibleSize.width*0.1115,origin.y+visibleSize.height*0.89));
	this->addChild(task,2);

	/////////////////////////////////////////////////////////////////////////////////
	y= visibleSize.height*0.855+origin.y;
	
	auto buttonMessage=ui::Button::create("message.png","message.png","message.png",TextureResType::PLIST);
	buttonMessage->setAnchorPoint(Vec2(0,0));
	buttonMessage->setPosition(Vec2(origin.x+visibleSize.width*0.35,y));
	buttonMessage->addTouchEventListener(CC_CALLBACK_2(EnterRoom::message_callback,this));
	this->addChild(buttonMessage,2);

	auto buttonReward=ui::Button::create("prize.png","prize.png","prize.png",TextureResType::PLIST);
	buttonReward->setAnchorPoint(Vec2(0,0));
	buttonReward->setPosition(Vec2(origin.x+visibleSize.width*0.45,y));
	buttonReward->addTouchEventListener(CC_CALLBACK_2(EnterRoom::prize_callback,this));
	this->addChild(buttonReward,2);

	auto buttonRank=ui::Button::create("cup.png","cup.png","cup.png",TextureResType::PLIST);
	buttonRank->setAnchorPoint(Vec2(0,0));
	buttonRank->setPosition(Vec2(origin.x+visibleSize.width*0.55,y));
	buttonRank->addTouchEventListener(CC_CALLBACK_2(EnterRoom::rank_callback,this));
	this->addChild(buttonRank,2);

	auto buttonActivity=ui::Button::create("activity_box.png","activity_box.png","activity_box.png",TextureResType::PLIST);
	buttonActivity->setAnchorPoint(Vec2(0,0));
	buttonActivity->setPosition(Vec2(origin.x+visibleSize.width*0.65,y));
	buttonActivity->addTouchEventListener(CC_CALLBACK_2(EnterRoom::active_callback,this));
	this->addChild(buttonActivity,2);

	auto buttonMall=ui::Button::create("mall.png","mall.png","mall.png",TextureResType::PLIST);
	buttonMall->setAnchorPoint(Vec2(0,0));
	buttonMall->setPosition(Vec2(origin.x+visibleSize.width*0.75,y));
	buttonMall->addTouchEventListener(CC_CALLBACK_2(EnterRoom::mall_callback,this));
	this->addChild(buttonMall,2);

	auto buttonSet=ui::Button::create("set.png","set.png","set.png",TextureResType::PLIST);
	buttonSet->setAnchorPoint(Vec2(0,0));
	buttonSet->setPosition(Vec2(origin.x+visibleSize.width*0.85,y));
	buttonSet->addTouchEventListener(CC_CALLBACK_2(EnterRoom::set_callback,this));
	this->addChild(buttonSet,2);

	auto buttonBack=ui::Button::create("back.png","back.png","back.png",TextureResType::PLIST);
	buttonBack->setAnchorPoint(Vec2(0,0));
	buttonBack->setPosition(Vec2(origin.x+visibleSize.width*0.932,y));
	buttonBack->addTouchEventListener(CC_CALLBACK_2(EnterRoom::back_callback,this));
	this->addChild(buttonBack,2);

	auto blackbar= Sprite::create("blackbar.png");
	blackbar->setAnchorPoint(Vec2(0.5,0));
	blackbar->setPosition(Point(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.73));
	this->addChild(blackbar,1);

	auto normalButton=Button::create("normal_race.png","normal_race.png","normal_race.png",TextureResType::PLIST);
	normalButton->setAnchorPoint(Vec2(0.5f,0.5f));
	/*normalButton->setPosition(Vec2(origin.x-100,origin.y+visibleSize.height*0.4));
	normalButton->runAction(MoveTo::create(0.2,Vec2(origin.x+visibleSize.width*0.18,origin.y+visibleSize.height*0.4)));*/
	normalButton->setScaleX(0);
	normalButton->setPosition(Vec2(origin.x+visibleSize.width*0.18,origin.y+visibleSize.height*0.4));
	normalButton->addTouchEventListener(CC_CALLBACK_2(EnterRoom::normal_callback,this));
	this->addChild(normalButton,2,17);
	//CallFunc::create(this,callfunc_selector(HelloWorld::addButton))
	auto easeBounceInOut1= EaseBounceIn::create(ScaleTo::create(0.2,1));
	normalButton->runAction(easeBounceInOut1);

	/*auto _gridNodeTarget = NodeGrid::create();
	auto ripple3D= Waves3D::create(2, Size(10,10), 5, 40);
	_gridNodeTarget->runAction(ripple3D);
	this->addChild(_gridNodeTarget);
	_gridNodeTarget->addChild(normalButton);*/

	auto competeButton=Button::create("compete_race.png","compete_race.png","compete_race.png",TextureResType::PLIST);
	competeButton->setAnchorPoint(Vec2(0.5f,0.5f));
	competeButton->setScaleX(0);
	competeButton->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.4));
	competeButton->addTouchEventListener(CC_CALLBACK_2(EnterRoom::compete_callback,this));
	this->addChild(competeButton,2,18);

	auto easeBounceInOut2= EaseBounceIn::create(ScaleTo::create(0.2,1));
	competeButton->runAction(Sequence::create(DelayTime::create(0.2),easeBounceInOut2,NULL));

	auto fastRaceButton=Button::create("fast_race.png","fast_race.png","fast_race.png",TextureResType::PLIST);
	fastRaceButton->setAnchorPoint(Vec2(0.5f,0.5f));
	fastRaceButton->setScaleX(0);
	fastRaceButton->setPosition(Vec2(origin.x+visibleSize.width*0.81,origin.y+visibleSize.height*0.4));
	fastRaceButton->addTouchEventListener(CC_CALLBACK_2(EnterRoom::fastRace_callback,this));
	this->addChild(fastRaceButton,2,19);

	auto easeBounceInOut3= EaseBounceIn::create(ScaleTo::create(0.2,1));
	fastRaceButton->runAction(Sequence::create(DelayTime::create(0.4),easeBounceInOut3,NULL));

	auto shade1= Sprite::createWithSpriteFrameName("shade.png");
	shade1->setScale(0);
	shade1->setOpacity(100);
	shade1->setAnchorPoint(Vec2(0.5f,0.5f));
	shade1->setPosition(Vec2(origin.x+visibleSize.width*0.27,origin.y+visibleSize.height*0.31));
	this->addChild(shade1,1,20);

	auto easeBounceInOut4= EaseBounceOut::create(ScaleTo::create(0.2,1));
	shade1->runAction(Sequence::create(DelayTime::create(0.1),easeBounceInOut4,NULL));

	auto shade2= Sprite::createWithSpriteFrameName("shade.png");
	shade2->setScale(0);
	shade2->setOpacity(100);
	shade2->setAnchorPoint(Vec2(0.5f,0.5f));
	shade2->setPosition(Vec2(origin.x+visibleSize.width*0.59,origin.y+visibleSize.height*0.305));
	this->addChild(shade2,1,21);

	auto easeBounceInOut5= EaseBounceOut::create(ScaleTo::create(0.2,1));
	shade2->runAction(Sequence::create(DelayTime::create(0.3),easeBounceInOut5,NULL));

	auto shade3= Sprite::createWithSpriteFrameName("shade.png");
	shade3->setScale(0);
	shade3->setOpacity(100);
	shade3->setAnchorPoint(Vec2(0.5f,0.5f));
	shade3->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.305));
	this->addChild(shade3,1,22);

	auto easeBounceInOut6= EaseBounceOut::create(ScaleTo::create(0.2,1));
	shade3->runAction(Sequence::create(DelayTime::create(0.5),easeBounceInOut6,NULL));

	//scheduleOnce(schedule_selector(EnterRoom::allButtonAction),0.7);
	scheduleOnce(schedule_selector(EnterRoom::allButtonAction2),0.7);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dengluzhutu.plist");
	auto signRaceButton=Button::create("danjichang.png","danjichang.png","danjichang.png",TextureResType::PLIST);
	signRaceButton->setAnchorPoint(Vec2(0.5,0.5));
	signRaceButton->setPosition(Vec2(visibleSize.width/2 + origin.x,origin.y+visibleSize.height*0.03));
	signRaceButton->addTouchEventListener(CC_CALLBACK_2(EnterRoom::onButtonSignField,this));
	this->addChild(signRaceButton,2);

	auto colorLayer=LayerColor::create(Color4B(255,255,255,0));
	colorLayer->setContentSize(visibleSize);
	colorLayer->setAnchorPoint(Vec2(0,0));
	colorLayer->setPosition(Vec2(0,0));

	auto paddle=ui::ScrollView::create();
	paddle->setSize(visibleSize);
	paddle->setAnchorPoint(Vec2(0,0));
	paddle->setPosition(Vec2(0,0));
	paddle->addChild(colorLayer);
	this->addChild(paddle,4,23);
	scheduleOnce(schedule_selector(EnterRoom::deletePaddle),1.0f);
	call_gold_prize();

    return true;
}

void EnterRoom::normal_callback(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	auto commonButton=(Button*)pSender;
	auto shade=(Sprite*)this->getChildByTag(20);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		commonButton->runAction(ScaleTo::create(0.1,1.1));
		shade->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		shade->runAction(ScaleTo::create(0.1,1));
		commonButton->runAction(Sequence::create(ScaleTo::create(0.1,1),CallFunc::create(this,callfunc_selector(EnterRoom::commonFieldAction)),NULL));
		scheduleOnce(schedule_selector(EnterRoom::entranceToCommonField),0.5);
		commonButton->setTouchEnabled(false);
		break;
	case Widget::TouchEventType::CANCELED:
		shade->runAction(ScaleTo::create(0.1,1));
		commonButton->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void EnterRoom::compete_callback(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	auto competeButton=(Button*)pSender;
	auto shade=(Sprite*)this->getChildByTag(21);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		competeButton->runAction(ScaleTo::create(0.1,1.1));
		shade->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		competeButton->runAction(Sequence::create(ScaleTo::create(0.1,1),CallFunc::create(this,callfunc_selector(EnterRoom::competeFieldAction)),NULL));
		shade->runAction(ScaleTo::create(0.1,1));
		scheduleOnce(schedule_selector(EnterRoom::entranceToCompeteField),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		competeButton->runAction(ScaleTo::create(0.1,1));
		shade->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void EnterRoom::fastRace_callback(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	auto fastRaceButton=(Button*)pSender;
	auto shade=(Sprite*)this->getChildByTag(22);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		fastRaceButton->runAction(ScaleTo::create(0.1,1.1));
		shade->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		fastRaceButton->runAction(Sequence::create(ScaleTo::create(0.1,1),CallFunc::create(this,callfunc_selector(EnterRoom::fastRaceAction)),NULL));
		shade->runAction(ScaleTo::create(0.1,1));
		scheduleOnce(schedule_selector(EnterRoom::entranceToFastRace),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		fastRaceButton->runAction(ScaleTo::create(0.1,1));
		shade->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void EnterRoom::myBaseInfo_callback(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	auto myBaseInfo=BaseScene::create();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Director::getInstance()->pushScene(myBaseInfo);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}

void EnterRoom::onButtonSignField(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	auto origin=Director::getInstance()->getVisibleOrigin();
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto race=Sprite::createWithSpriteFrameName("danjichang.png");
	race->setAnchorPoint(Point(0.5f,0.5f));
	race->setPosition(Point(visibleSize.width/2 + origin.x,origin.y+visibleSize.height*0.03));
	this->addChild(race);
    
	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	race->runAction(spawn);
}

void EnterRoom::commonFieldAction()
{
	auto origin=Director::getInstance()->getVisibleOrigin();
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto normal_race= Sprite::createWithSpriteFrameName("normal_race.png");
	normal_race->setAnchorPoint(Vec2(0.5f,0.5f));
	normal_race->setOpacity(100);
	normal_race->setPosition(Vec2(origin.x+visibleSize.width*0.18,origin.y+visibleSize.height*0.4));
	this->addChild(normal_race);
	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	normal_race->runAction(spawn);
}

void EnterRoom::entranceToCommonField(float t)
{
	auto commonField=CommonField::create();
	Director::getInstance()->replaceScene(commonField);
}

void EnterRoom::entranceToCompeteField(float t)
{
	auto competitionField=CompetitionField::create();
	Director::getInstance()->pushScene(competitionField);
}
void EnterRoom::competeFieldAction()
{
	auto origin=Director::getInstance()->getVisibleOrigin();
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto normal_race= Sprite::createWithSpriteFrameName("compete_race.png");
	normal_race->setOpacity(100);
	normal_race->setAnchorPoint(Vec2(0.5f,0.5f));
	normal_race->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.4));
	this->addChild(normal_race);
	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	normal_race->runAction(spawn);
}
void EnterRoom::entranceToFastRace(float t)
{
	;
}
void EnterRoom::fastRaceAction()
{
	auto origin=Director::getInstance()->getVisibleOrigin();
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto normal_race= Sprite::createWithSpriteFrameName("fast_race.png");
	normal_race->setAnchorPoint(Vec2(0.5f,0.5f));
	normal_race->setOpacity(100);
	normal_race->setPosition(Vec2(origin.x+visibleSize.width*0.81,origin.y+visibleSize.height*0.4));
	this->addChild(normal_race);
	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	normal_race->runAction(spawn);
}

void EnterRoom::allButtonAction(float t)
{
	auto commonButton=(Button*)this->getChildByTag(17);
	auto shade1=(Sprite*)this->getChildByTag(20);
	auto competeButton=(Button*)this->getChildByTag(18);
	auto shade2=(Sprite*)this->getChildByTag(21);
	auto fastRaceButton=(Button*)this->getChildByTag(19);
	auto shade3=(Sprite*)this->getChildByTag(22);
	commonButton->runAction(Sequence::create(ScaleTo::create(0.1,1.1),ScaleTo::create(0.1,1),CallFunc::create(this,callfunc_selector(EnterRoom::commonFieldAction)),NULL));
	shade1->runAction(Sequence::create(ScaleTo::create(0.1,1.1),ScaleTo::create(0.1,1),NULL));
	competeButton->runAction(Sequence::create(ScaleTo::create(0.1,1.1),ScaleTo::create(0.1,1),CallFunc::create(this,callfunc_selector(EnterRoom::competeFieldAction)),NULL));
	shade2->runAction(Sequence::create(ScaleTo::create(0.1,1.1),ScaleTo::create(0.1,1),NULL));
	fastRaceButton->runAction(Sequence::create(ScaleTo::create(0.1,1.1),ScaleTo::create(0.1,1),CallFunc::create(this,callfunc_selector(EnterRoom::fastRaceAction)),NULL));
	shade3->runAction(Sequence::create(ScaleTo::create(0.1,1.1),ScaleTo::create(0.1,1),NULL));
}

void EnterRoom::deletePaddle(float t)
{
	this->removeChildByTag(23);
}

void EnterRoom::deletePaddle1(float t)
{
	this->removeChildByTag(24);
}

void EnterRoom::allButtonAction2(float t)
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	auto commonButton=(Button*)this->getChildByTag(17);
	auto shade1=(Sprite*)this->getChildByTag(20);
	auto competeButton=(Button*)this->getChildByTag(18);
	auto shade2=(Sprite*)this->getChildByTag(21);
	auto fastRaceButton=(Button*)this->getChildByTag(19);
	auto shade3=(Sprite*)this->getChildByTag(22);

	auto moveTo11=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.18,origin.y+visibleSize.height*0.41));
	auto moveTo12=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.18,origin.y+visibleSize.height*0.40));
	auto moveTo13=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.18,origin.y+visibleSize.height*0.39));
	auto moveTo14=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.18,origin.y+visibleSize.height*0.40));
	auto commonButtonAction=Sequence::create(moveTo11,moveTo12,moveTo13,moveTo14,NULL);

	auto moveTo15=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.27,origin.y+visibleSize.height*0.32));
	auto moveTo16=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.27,origin.y+visibleSize.height*0.31));
	auto moveTo17=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.27,origin.y+visibleSize.height*0.30));
	auto moveTo18=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.27,origin.y+visibleSize.height*0.31));
	auto shade1Action=Sequence::create(moveTo15,moveTo16,moveTo17,moveTo18,NULL);

	commonButton->runAction(RepeatForever::create(commonButtonAction));
	shade1->runAction(RepeatForever::create(shade1Action));

	auto moveTo21=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.39));
	auto moveTo22=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.40));
	auto moveTo23=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.41));
	auto moveTo24=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.40));
	auto competeButtonAction=Sequence::create(moveTo21,moveTo22,moveTo23,moveTo24,NULL);

	auto moveTo25=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.59,origin.y+visibleSize.height*0.295));
	auto moveTo26=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.59,origin.y+visibleSize.height*0.305));
	auto moveTo27=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.59,origin.y+visibleSize.height*0.315));
	auto moveTo28=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.59,origin.y+visibleSize.height*0.305));
	auto shade2Action=Sequence::create(moveTo25,moveTo26,moveTo27,moveTo28,NULL);

	competeButton->runAction(RepeatForever::create(competeButtonAction));
	shade2->runAction(RepeatForever::create(shade2Action));

	auto moveTo31=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.81,origin.y+visibleSize.height*0.41));
	auto moveTo32=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.81,origin.y+visibleSize.height*0.40));
	auto moveTo33=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.81,origin.y+visibleSize.height*0.39));
	auto moveTo34=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.81,origin.y+visibleSize.height*0.40));
	auto fastRaceButtonAction=Sequence::create(moveTo31,moveTo32,moveTo33,moveTo34,NULL);

	auto moveTo35=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.315));
	auto moveTo36=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.305));
	auto moveTo37=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.295));
	auto moveTo38=MoveTo::create(1.0,Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.305));
	auto shade3Action=Sequence::create(moveTo35,moveTo36,moveTo37,moveTo38,NULL);

	fastRaceButton->runAction(RepeatForever::create(fastRaceButtonAction));
	shade3->runAction(RepeatForever::create(shade3Action));

}

void EnterRoom::stopEmitter(float t)
{
	_emitter->stopSystem();
	_emitter1->stopSystem();
	_emitter2->stopSystem();
	_emitter3->stopSystem();
	_emitter4->stopSystem();
	_emitter5->stopSystem();
}

void EnterRoom::startEmitter()
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	_emitter= ParticleSnow::create();
    this->addChild(_emitter,4);
    _emitter->setLife(10);
    _emitter->setLifeVar(1);
    // gravity
    _emitter->setGravity(Vec2(0,-10));
    // speed of particles
    _emitter->setSpeed(900);
    _emitter->setSpeedVar(5);
  //  _emitter->setEmissionRate(_emitter->getTotalParticles()/_emitter->getLife());
	//_emitter->initWithFile(
		//("luo1.plist")
	//auto textureCache=Director::getInstance()->getTextureCache()->addImage("luo1.png");
    _emitter->setTexture( Director::getInstance()->getTextureCache()->addImage("44.png"));
	_emitter->setStartSize(70);
	_emitter->setEmissionRate(10);
	//_emitter->setScale(5);
	_emitter->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height));

	_emitter1= ParticleSnow::create();
    this->addChild(_emitter1,4);
    _emitter1->setLife(10);
    _emitter1->setLifeVar(1);
    // gravity
    _emitter1->setGravity(Vec2(0,-10));
    // speed of particles
    _emitter1->setSpeed(900);
    _emitter1->setSpeedVar(50);
    _emitter1->setTexture( Director::getInstance()->getTextureCache()->addImage("11.png"));
	_emitter1->setStartSize(70);
	_emitter1->setEmissionRate(10);
	//_emitter->setScale(5);
	_emitter1->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height));

	_emitter2= ParticleSnow::create();
    this->addChild(_emitter2,4);
    _emitter2->setLife(10);
    _emitter2->setLifeVar(1);
    // gravity
    _emitter2->setGravity(Vec2(0,-10));
    // speed of particles
    _emitter2->setSpeed(900);
    _emitter2->setSpeedVar(50);
    _emitter2->setTexture( Director::getInstance()->getTextureCache()->addImage("22.png"));
	_emitter2->setStartSize(70);
	_emitter2->setEmissionRate(10);
	//_emitter->setScale(5);
	_emitter2->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height));

	_emitter3= ParticleSnow::create();
    this->addChild(_emitter3,4);
    _emitter3->setLife(10);
    _emitter3->setLifeVar(1);
    // gravity
    _emitter3->setGravity(Vec2(0,-10));
    // speed of particles
    _emitter3->setSpeed(900);
    _emitter3->setSpeedVar(50);
    _emitter3->setTexture( Director::getInstance()->getTextureCache()->addImage("33.png"));
	_emitter3->setStartSize(70);
	_emitter3->setEmissionRate(10);
	//_emitter->setScale(5);
	_emitter3->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height));

	_emitter4= ParticleSnow::create();
    this->addChild(_emitter4,4);
    _emitter4->setLife(10);
    _emitter4->setLifeVar(1);
    // gravity
    _emitter4->setGravity(Vec2(0,-10));
    // speed of particles
    _emitter4->setSpeed(900);
    _emitter4->setSpeedVar(50);
    _emitter4->setTexture( Director::getInstance()->getTextureCache()->addImage("55.png"));
	_emitter4->setStartSize(70);
	_emitter4->setEmissionRate(10);
	//_emitter->setScale(5);
	_emitter4->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height));

	_emitter5= ParticleSnow::create();
    this->addChild(_emitter5,4);
    _emitter5->setLife(10);
    _emitter5->setLifeVar(1);
    // gravity
    _emitter5->setGravity(Vec2(0,-10));
    // speed of particles
    _emitter5->setSpeed(900);
    _emitter5->setSpeedVar(50);
    _emitter5->setTexture( Director::getInstance()->getTextureCache()->addImage("66.png"));
	_emitter5->setStartSize(70);
	_emitter5->setEmissionRate(10);
	//_emitter->setScale(5);
	_emitter5->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height));
}