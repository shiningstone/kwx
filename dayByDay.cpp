#include "dayByDay.h"


dayAward::dayAward(Node* p)
{
	parent=(EnterRoom*)p;
	init();
}

bool dayAward::init()
{
	if(!Layer::init())
	{
		return false;
	}
	receiveTime=1;

	Size visibleSize=Director::getInstance()->getVisibleSize();//大小和原点
	Vec2 origin=Director::getInstance()->getVisibleOrigin();

	auto cache=SpriteFrameCache::getInstance();//精灵缓冲帧
	cache->addSpriteFramesWithFile("denglupaihang.plist");

	auto InkScroll=Sprite::createWithSpriteFrame(cache->spriteFrameByName("juanzhou.png"));//卷轴
	InkScroll->setAnchorPoint(Vec2(0.5,0.5));
	InkScroll->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-33));
	this->addChild(InkScroll,1,100);

	//上部
	auto listTitle=Sprite::createWithSpriteFrame(cache->spriteFrameByName("meiridenglujiangli.png"));//每日登陆奖励字体
	listTitle->setAnchorPoint(Vec2(0,1));
	listTitle->setPosition(Vec2(origin.x+visibleSize.width*407/1218,origin.y+visibleSize.height*628/716));
	this->addChild(listTitle,2);

	auto mojiTitle=Sprite::createWithSpriteFrame(cache->spriteFrameByName("biaoti moji.png"));//墨迹
	mojiTitle->setAnchorPoint(Vec2(0,0));
	mojiTitle->setPosition(Vec2(origin.x+visibleSize.width*365/1218,origin.y+visibleSize.height*533/716));
	this->addChild(mojiTitle,1);
	
	auto huahua=Sprite::createWithSpriteFrame(cache->spriteFrameByName("hua.png"));//花花
	huahua->setAnchorPoint(Vec2(0.5,0.5));
	huahua->setScale(0.8,0.8);
	huahua->setPosition(Vec2(origin.x+visibleSize.width*975/1218,origin.y+visibleSize.height*535/716));
	this->addChild(huahua,1);

	Texture2D *pTexture=TextureCache::sharedTextureCache()->addImage("fonts/logrewardNO.png");//精灵帧--数字
	SpriteFrame* d[10];
	int a;
	for(a=0;a<10;a++)
	{
		d[a]=SpriteFrame::createWithTexture(pTexture,Rect(18.5*a,27*0,18.5,27));
	}

	for(a=0;a<8;a++)//金币+已领取
	{
		if(a<=3)
		{
			if(a==0)
			{
				auto muchGold=Button::create("kelingqu.png","yilingqu23.png","yilingqu23.png",UI_TEX_TYPE_PLIST);//金币
				muchGold->setTouchEnabled(false);
				//muchGold->setEnabled(false);
				//muchGold->addTouchEventListener(CC_CALLBACK_2(dayAward::callBack,this));
				muchGold->setAnchorPoint(Vec2(0.5,0.5));
				muchGold->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*465/716));
				this->addChild(muchGold,2,a+1);
			}
			else
			{
				auto muchGold=Button::create("weilingqu.png","yilingqu23.png","yilingqu23.png",UI_TEX_TYPE_PLIST);//金币
				muchGold->setTouchEnabled(false);
				//muchGold->setEnabled(false);
				//muchGold->addTouchEventListener(CC_CALLBACK_2(dayAward::callBack,this));
				muchGold->setAnchorPoint(Vec2(0.5,0.5));
				muchGold->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*465/716));
				this->addChild(muchGold,2,a+1);
			}

			auto receiveEnd=Sprite::createWithSpriteFrame(cache->spriteFrameByName("yilingqu.png"));//已领取
			receiveEnd->setVisible(false);
			receiveEnd->setAnchorPoint(Vec2(0.5,0.5));
			receiveEnd->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*495/716));
			this->addChild(receiveEnd,2,a+11);
			
			auto rightIcon=Sprite::createWithSpriteFrame(cache->spriteFrameByName("gou.png"));//对勾
			rightIcon->setVisible(false);
			rightIcon->setAnchorPoint(Vec2(0.5,0.5));
			rightIcon->setPosition(Vec2(origin.x+visibleSize.width*(360+213*a)/1218,origin.y+visibleSize.height*455/716));
			this->addChild(rightIcon,3,a+110);
			
			if(a==0)
			{
				auto blink = Blink::create(1.5f,1);
				rightIcon->setVisible(true);
				rightIcon->runAction(RepeatForever::create(blink));
			}
		}
		else if(a>3)
		{
			if(a<7)
			{
				auto muchGold=Button::create("weilingqu.png","yilingqu23.png","yilingqu23.png",UI_TEX_TYPE_PLIST);//金币
				muchGold->setTouchEnabled(false);
				//muchGold->addTouchEventListener(CC_CALLBACK_2(dayAward::callBack,this));
				muchGold->setAnchorPoint(Vec2(0.5,0.5));
				muchGold->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*305/716));
				this->addChild(muchGold,2,a+1);
				
				auto receiveEnd=Sprite::createWithSpriteFrame(cache->spriteFrameByName("yilingqu.png"));//已领取
				receiveEnd->setVisible(false);
				receiveEnd->setAnchorPoint(Vec2(0.5,0.5));
				receiveEnd->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*335/716));
				this->addChild(receiveEnd,2,a+11);

				auto rightIcon=Sprite::createWithSpriteFrame(cache->spriteFrameByName("gou.png"));//对勾
				rightIcon->setVisible(false);
				rightIcon->setAnchorPoint(Vec2(0.5,0.5));
				rightIcon->setPosition(Vec2(origin.x+visibleSize.width*(360+213*(a-4))/1218,origin.y+visibleSize.height*295/716));
				this->addChild(rightIcon,3,a+110);
			}
			else
			{
				auto muchGold=Button::create("quanqinjiangjb.png","quanqinjiangjb.png","yilingqu23.png",UI_TEX_TYPE_PLIST);//金币
				muchGold->setTouchEnabled(false);
				//muchGold->addTouchEventListener(CC_CALLBACK_2(dayAward::callBack,this));
				muchGold->setAnchorPoint(Vec2(0.5,0.5));
				muchGold->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*305/716));
				this->addChild(muchGold,2,a+1);

				auto receiveEnd=Sprite::createWithSpriteFrame(cache->spriteFrameByName("yilingqu.png"));//已领取
				receiveEnd->setVisible(false);
				receiveEnd->setAnchorPoint(Vec2(0.5,0.5));
				receiveEnd->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*335/716));
				this->addChild(receiveEnd,2,a+11);

				auto rightIcon=Sprite::createWithSpriteFrame(cache->spriteFrameByName("gou.png"));//对勾
				rightIcon->setVisible(false);
				rightIcon->setAnchorPoint(Vec2(0.5,0.5));
				rightIcon->setPosition(Vec2(origin.x+visibleSize.width*(360+213*(a-4))/1218,origin.y+visibleSize.height*295/716));
				this->addChild(rightIcon,3,a+110);
			}
		}
	}
	
	for(a=0;a<8;a++)//墨迹+天数
	{
		if(a<=3)
		{
			auto mojiBottom=Sprite::createWithSpriteFrame(cache->spriteFrameByName("moji112.png"));
			mojiBottom->setAnchorPoint(Vec2(0.5,0.5));
			mojiBottom->setPosition(Vec2(origin.x+visibleSize.width*(305+213*a)/1218,origin.y+visibleSize.height*385/716));
			this->addChild(mojiBottom,2);

			auto dayNO=Sprite::createWithSpriteFrame(d[a+1]);
			dayNO->setAnchorPoint(Vec2(0.5,0.5));
			dayNO->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*385/716));
			this->addChild(dayNO,3);
		}
		else
		{
			auto mojiBottom=Sprite::createWithSpriteFrame(cache->spriteFrameByName("moji112.png"));
			mojiBottom->setAnchorPoint(Vec2(0.5,0.5));
			mojiBottom->setPosition(Vec2(origin.x+visibleSize.width*(305+213*(a-4))/1218,origin.y+visibleSize.height*225/716));
			this->addChild(mojiBottom,2);
			if(a!=7)
			{
				auto dayNO=Sprite::createWithSpriteFrame(d[a+1]);
				dayNO->setAnchorPoint(Vec2(0.5,0.5));
				dayNO->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*225/716));
				this->addChild(dayNO,3);
			}
		}
	}

	for(a=0;a<8;a++)//天
	{
		if(a<=3)
		{
			auto Bottomtian=Sprite::createWithSpriteFrame(cache->spriteFrameByName("tian.png"));
			Bottomtian->setAnchorPoint(Vec2(0.5,0.5));
			Bottomtian->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*385/716));
			this->addChild(Bottomtian,2);
		}
		else
		{
			if(a<7)
			{
				auto Bottomtian=Sprite::createWithSpriteFrame(cache->spriteFrameByName("tian.png"));
				Bottomtian->setAnchorPoint(Vec2(0.5,0.5));
				Bottomtian->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*225/716));
				this->addChild(Bottomtian,2);
			}
			else
			{
				auto Bottomtian=Sprite::createWithSpriteFrame(cache->spriteFrameByName("quanqinjiang.png"));
				Bottomtian->setAnchorPoint(Vec2(0.5,0.5));
				Bottomtian->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*225/716));
				this->addChild(Bottomtian,2);
			}
		}
	}

	for(a=0;a<8;a++)//获得金币数量
	{
		switch (a)
		{
		case 0:
			{	
				auto rewardNO1=LabelAtlas::create("300","fonts/logRewardGod.png",31,41,'0');
				rewardNO1->setScale(0.8);
				rewardNO1->setAnchorPoint(Vec2(0.5,0.5));
				rewardNO1->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*445/716));
				this->addChild(rewardNO1,2);
			}
			break;
		case 1:
			{		
				auto rewardNO2=LabelAtlas::create("600","fonts/logRewardGod.png",31,41,'0');
				rewardNO2->setScale(0.8);
				rewardNO2->setAnchorPoint(Vec2(0.5,0.5));
				rewardNO2->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*445/716));
				this->addChild(rewardNO2,2);
			}
			break;
		case 2:
			{		
				auto rewardNO3=LabelAtlas::create("700","fonts/logRewardGod.png",31,41,'0');
				rewardNO3->setScale(0.8);
				rewardNO3->setAnchorPoint(Vec2(0.5,0.5));
				rewardNO3->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*445/716));
				this->addChild(rewardNO3,2);
			}
			break;
		case 3:
			{		
				auto rewardNO4=LabelAtlas::create("800","fonts/logRewardGod.png",31,41,'0');
				rewardNO4->setScale(0.8);
				rewardNO4->setAnchorPoint(Vec2(0.5,0.5));
				rewardNO4->setPosition(Vec2(origin.x+visibleSize.width*(290+213*a)/1218,origin.y+visibleSize.height*445/716));
				this->addChild(rewardNO4,2);
			}
			break;
		case 4:
			{		
				auto rewardNO5=LabelAtlas::create("1000","fonts/logRewardGod.png",31,41,'0');
				rewardNO5->setScale(0.8);
				rewardNO5->setAnchorPoint(Vec2(0.5,0.5));
				rewardNO5->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*285/716));
				this->addChild(rewardNO5,2);
			}
			break;
		case 5:
			{		
				auto rewardNO6=LabelAtlas::create("1200","fonts/logRewardGod.png",31,41,'0');
				rewardNO6->setScale(0.8);
				rewardNO6->setAnchorPoint(Vec2(0.5,0.5));
				rewardNO6->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*285/716));
				this->addChild(rewardNO6,2);
			}
			break;
		case 6:
			{		
				auto rewardNO7=LabelAtlas::create("1600","fonts/logRewardGod.png",31,41,'0');
				rewardNO7->setScale(0.8);
				rewardNO7->setAnchorPoint(Vec2(0.5,0.5));
				rewardNO7->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*285/716));
				this->addChild(rewardNO7,2);
			}
			break;
		case 7:		
			{		
				auto rewardNO8=LabelAtlas::create("2000","fonts/logRewardGod.png",31,41,'0');
				rewardNO8->setScale(0.8);
				rewardNO8->setAnchorPoint(Vec2(0.5,0.5));
				rewardNO8->setPosition(Vec2(origin.x+visibleSize.width*(290+213*(a-4))/1218,origin.y+visibleSize.height*285/716));
				this->addChild(rewardNO8,2);
			}
			break;
		default:
			break;
		}
	}



	auto str_label = LabelTTF::create("连续登陆天数越多,获得的礼包越丰厚!未连续登陆将从第1天重新开始哦!","Arial",24);
	str_label->setColor(Color3B(0x8b,0x3e,0x2f));
	//float str_label_x=volume->getTextureRect().size.width/2;
	//float str_label_y=gold_gap_y/2;
	str_label->setAnchorPoint(Vec2(0.5,0.5));
	str_label->setPosition(Point(origin.x+visibleSize.width/2,origin.y+visibleSize.height*180/716));
	this->addChild(str_label, 1);

	auto receive=Button::create("baoming anniu2.png","baoming anniu.png","",UI_TEX_TYPE_PLIST);//领取按钮
	receive->addTouchEventListener(CC_CALLBACK_2(dayAward::callBack,this));
	receive->setScale(0.7);
	receive->setAnchorPoint(Vec2(0.5,0.5));
	receive->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height*120/716));
	this->addChild(receive,2);

	auto receiveFont=Sprite::createWithSpriteFrame(cache->spriteFrameByName("lingqu.png"));
	receiveFont->setRotation(-90);
	receiveFont->setAnchorPoint(Vec2(0.5,0.5));
	receiveFont->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height*120/716));
	this->addChild(receiveFont,2);

	//auto maxtimes=Button::create("zuidafanxing2.png","zuidafanxing.png","zuidafanxing2.png",UI_TEX_TYPE_PLIST);
	//maxtimes->setTouchEnabled(true);
	//maxtimes->addTouchEventListener(CC_CALLBACK_2(dayAward::callBack,this));
	//maxtimes->setAnchorPoint(Vec2(0,0));
	//maxtimes->setPosition(Vec2(origin.x+visibleSize.width*595/1218,origin.y+visibleSize.height*465/716));
	//this->addChild(maxtimes,2);
	//startEmitter(0.5);
	return true;
}
//金币Tag(0-8),已领取Tag(10-18),对勾Tag(110-118)

void dayAward::callBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
    {
    case Widget::TouchEventType::BEGAN:
		break;
           
	case Widget::TouchEventType::MOVED:
		break;       
    case Widget::TouchEventType::ENDED:
	{
		auto buton=(Button*)pSender;
		auto bt=(Button*)this->getChildByTag(1);
		auto lingqu=(Sprite*)this->getChildByTag(receiveTime+10);
		auto gou=(Sprite*)this->getChildByTag(110);
		bt->setHighlighted(true);
		lingqu->setVisible(true);
		gou->stopAllActions();
		gou->setVisible(false);
		bt->setHighlighted(true);
		if(receiveTime>8)
		{
			receiveTime-=8;
			for(int a=1;a<9;a++)
			{
				this->getChildByTag(a)->setVisible(false);
			}
		}
		//scheduleOnce(schedule_selector(dayAward::backToMain),1);
		buton->setTouchEnabled(false);
		scheduleOnce(schedule_selector(dayAward::backToMain),0.1);
	 }
     break;
            
     case Widget::TouchEventType::CANCELED:
         break;
            
     default:
         break;
    }
}

void dayAward::backToMain(float t)
{
	parent->removeDayReward();
}