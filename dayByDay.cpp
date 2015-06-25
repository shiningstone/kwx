#include "dayByDay.h"
#include "network/KwxEnv.h"


dayAward::dayAward(Node* p)
{
	ReceiveTime=EnvVariable::getInstance()->_dailyLogin.continuousDays;
	RewardGold=EnvVariable::getInstance()->_dailyLogin.dailyReward;
	parent=(EnterRoom*)p;
	init();
}
TargetedAction* dayAward::GoldGetEffect(Vec2 curPos)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Effects_of_day.plist");
	auto centerExplode=Sprite::createWithSpriteFrameName("940000.png");
	centerExplode->setAnchorPoint(Vec2(0.5,0.5));
	centerExplode->setPosition(curPos);
	centerExplode->setScale(0);
	this->addChild(centerExplode,30);
	auto DayGoldAnimate = Animation::create();
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940000.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940001.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940002.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940003.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940004.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940005.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940006.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940007.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940008.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940009.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940010.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940011.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940012.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940013.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940014.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940015.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940016.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940017.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940018.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940019.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940020.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940021.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940022.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940023.png"));
	DayGoldAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("940024.png"));
	DayGoldAnimate->setDelayPerUnit(0.05);
	DayGoldAnimate->setRestoreOriginalFrame(true);
	auto GetGoldaction = Animate::create(DayGoldAnimate);
	auto GetGoldSequence=Sequence::create(ScaleTo::create(0,1),GetGoldaction,ScaleTo::create(0,0),NULL);
	auto GetGoldTarget=TargetedAction::create(centerExplode,GetGoldSequence);
	return GetGoldTarget;
}
void dayAward::receiveTimeCount()
{
	ReceiveTime+=1;
	if(ReceiveTime>8)
		ReceiveTime%=8;
	UserDefault::getInstance()->setIntegerForKey("load_days",ReceiveTime);
	UserDefault::getInstance()->flush();
}
void dayAward::LingQucallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;       
	case Widget::TouchEventType::ENDED:
		{
			auto curGold=(Button*)this->getChildByTag(ReceiveTime);
			auto haveGetted=(Sprite*)this->getChildByTag(GOLD_HAVE_GETTED+ReceiveTime-1);
			auto duiGou=(Sprite*)this->getChildByTag(GOLD_ENABLE_DUIGOU+ReceiveTime-1);

			auto curButton=(Button*)pSender;
			curButton->setTouchEnabled(false);
			curGold->setBright(true);
			curGold->setHighlighted(true);
			haveGetted->setVisible(true);
			duiGou->stopAllActions();
			duiGou->setVisible(false);

			auto GoldGettedEffect=GoldGetEffect(curGold->getPosition());
			auto removeFunc=CallFunc::create([=](){parent->removeChildByTag(THIS_GET_DAYGOLD_BKG);});
			receiveTimeCount();
			this->runAction(Sequence::create(GoldGettedEffect,DelayTime::create(0.4),removeFunc,NULL));
		}
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
bool dayAward::init()
{
	if(!Layer::init())
	{
		return false;
	}
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("denglupaihang.plist");
	visibleSize=Director::getInstance()->getVisibleSize();//大小和原点
	origin=Director::getInstance()->getVisibleOrigin();
	ReceiveTime=UserDefault::getInstance()->getIntegerForKey("load_days");

	auto InkScroll=Sprite::createWithSpriteFrameName("juanzhou.png");//卷轴
	InkScroll->setAnchorPoint(Vec2(0.5,0.5));
	InkScroll->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-33));
	this->addChild(InkScroll,1,JUANZHOU_BKG);

	//上部
	auto mojiTitle=Sprite::createWithSpriteFrameName("biaoti moji.png");//墨迹
	mojiTitle->setAnchorPoint(Vec2(0,0));
	mojiTitle->setPosition(Vec2(origin.x+visibleSize.width*365/1218,origin.y+visibleSize.height*533/716));
	this->addChild(mojiTitle,1);

	auto listTitle=Sprite::createWithSpriteFrameName("meiridenglujiangli.png");//每日登陆奖励字体
	listTitle->setAnchorPoint(Vec2(0,1));
	listTitle->setPosition(Vec2(origin.x+visibleSize.width*407/1218,origin.y+visibleSize.height*628/716));
	this->addChild(listTitle,2);
	
	auto huahua=Sprite::createWithSpriteFrameName("hua.png");//梅花
	huahua->setAnchorPoint(Vec2(0.5,0.5));
	huahua->setScale(0.8,0.8);
	huahua->setPosition(Vec2(origin.x+visibleSize.width*975/1218,origin.y+visibleSize.height*535/716));
	this->addChild(huahua,1);

	for(int a=0;a<8;a++)//金币+已领取
	{
		cocos2d::ui::Button* muchGold;//金币堆
		if(a<7)
			muchGold=Button::create("weilingqu.png","yilingqu23.png","kelingqu.png",UI_TEX_TYPE_PLIST);
		else
			muchGold=Button::create("quanqinjiangjb.png","yilingqu23.png","quanqinjiangjb.png",UI_TEX_TYPE_PLIST);
		muchGold->setTouchEnabled(false);
		muchGold->setAnchorPoint(Vec2(0.5,0.5));
		if(a<4)
			muchGold->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*465/716));
		else
			muchGold->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*305/716));
		if(a<(ReceiveTime-1))
			muchGold->setHighlighted(true);
		else if(a==(ReceiveTime-1))
			muchGold->setBright(false);
		this->addChild(muchGold,2,a+GOLD_PNG_UI);

		auto receiveEnd=Sprite::createWithSpriteFrameName("yilingqu.png");//已领取
		receiveEnd->setVisible(false);
		receiveEnd->setAnchorPoint(Vec2(0.5,0.5));
		if(a<4)
			receiveEnd->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*495/716));
		else
			receiveEnd->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*335/716));
		if(a<(ReceiveTime-1))
			receiveEnd->setVisible(true);
		this->addChild(receiveEnd,3,a+GOLD_HAVE_GETTED);

		auto rightIcon=Sprite::createWithSpriteFrameName("gou.png");//对勾
		rightIcon->setVisible(false);
		rightIcon->setAnchorPoint(Vec2(0.5,0.5));
		if(a<4)
			rightIcon->setPosition(Vec2(origin.x+visibleSize.width*(360+213*a)/1218,origin.y+visibleSize.height*455/716));
		else
			rightIcon->setPosition(Vec2(origin.x+visibleSize.width*(360+213*(a-4))/1218,origin.y+visibleSize.height*295/716));
		if(a==(ReceiveTime-1))
			rightIcon->runAction(RepeatForever::create(Blink::create(1,1)));
		this->addChild(rightIcon,3,a+GOLD_ENABLE_DUIGOU);		

		auto mojiBottom=Sprite::createWithSpriteFrameName("moji112.png");//墨迹
		mojiBottom->setAnchorPoint(Vec2(0.5,0.5));
		if(a<4)
			mojiBottom->setPosition(Vec2(origin.x+visibleSize.width*(305+213*a)/1218,origin.y+visibleSize.height*385/716));
		else
			mojiBottom->setPosition(Vec2(origin.x+visibleSize.width*(305+213*(a-4))/1218,origin.y+visibleSize.height*225/716));
		this->addChild(mojiBottom,2);

		if(a<7)//天数
		{
			char curNo[5];
			std::string curDayNo;
			sprintf(curNo,"%d",a+1);
			curDayNo=curNo;

			auto dayNo=LabelAtlas::create(curDayNo,"fonts/logrewardNO.png",18.5,27,'0');
			dayNo->setAnchorPoint(Vec2(0.5,0.5));
			if(a<4)
				dayNo->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*385/716));
			else
				dayNo->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*225/716));
			this->addChild(dayNo,3);
		}

		if(a<7)//天图片
		{
			auto BottomTian=Sprite::createWithSpriteFrameName("tian.png");
			BottomTian->setAnchorPoint(Vec2(0.5,0.5));
			if(a<4)
				BottomTian->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*385/716));
			else
				BottomTian->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*225/716));
			this->addChild(BottomTian,3);
		}
		else
		{
			auto Bottomtian=Sprite::createWithSpriteFrameName("quanqinjiang.png");
			Bottomtian->setAnchorPoint(Vec2(0.5,0.5));
			Bottomtian->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*225/716));
			this->addChild(Bottomtian,3);
		}

		cocos2d::LabelAtlas* rewardNo;
		switch (a)
		{
		case 0:
			rewardNo=LabelAtlas::create("300","fonts/logRewardGod.png",31,41,'0');
			break;
		case 1:
			rewardNo=LabelAtlas::create("600","fonts/logRewardGod.png",31,41,'0');
			break;
		case 2:
			rewardNo=LabelAtlas::create("700","fonts/logRewardGod.png",31,41,'0');
			break;
		case 3:
			rewardNo=LabelAtlas::create("800","fonts/logRewardGod.png",31,41,'0');
			break;
		case 4:
			rewardNo=LabelAtlas::create("1000","fonts/logRewardGod.png",31,41,'0');
			break;
		case 5:
			rewardNo=LabelAtlas::create("1200","fonts/logRewardGod.png",31,41,'0');
			break;
		case 6:
			rewardNo=LabelAtlas::create("1600","fonts/logRewardGod.png",31,41,'0');
			break;
		case 7:
			rewardNo=LabelAtlas::create("2000","fonts/logRewardGod.png",31,41,'0');
			break;
		default:
			break;
		}
		rewardNo->setScale(0.8);
		rewardNo->setAnchorPoint(Vec2(0.5,0.5));
		if(a<4)
			rewardNo->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*445/716));
		else
			rewardNo->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*285/716));
		this->addChild(rewardNo,3);
	}

	auto str_label = LabelTTF::create("连续登陆天数越多,获得的礼包越丰厚!未连续登陆将从第1天重新开始哦!","Arial",24);
	str_label->setColor(Color3B(0x8b,0x3e,0x2f));
	str_label->setAnchorPoint(Vec2(0.5,0.5));
	str_label->setPosition(Point(origin.x+visibleSize.width/2,origin.y+visibleSize.height*180/716));
	this->addChild(str_label,1);

	auto receive=Button::create("baoming anniu2.png","baoming anniu.png","",UI_TEX_TYPE_PLIST);//领取按钮
	receive->addTouchEventListener(CC_CALLBACK_2(dayAward::LingQucallBack,this));
	receive->setScale(0.7);
	receive->setAnchorPoint(Vec2(0.5,0.5));
	receive->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height*120/716));
	this->addChild(receive,2);

	auto receiveFont=Sprite::createWithSpriteFrameName("lingqu.png");
	receiveFont->setRotation(-90);
	receiveFont->setAnchorPoint(Vec2(0.5,0.5));
	receiveFont->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height*120/716));
	this->addChild(receiveFont,3);

	return true;
}