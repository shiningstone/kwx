#include "HelloWorldScene.h"
#include "IMLoadScene.h"
#include "EnterRoomScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include "game/RaceLayer.h"
#include "protocol/CommonMsg.h"
#include "protocol/DsInstruction.h"
#include "protocol/KwxMessenger.h"
#include "protocol/KwxMsgLogin.h"

#include "utils/DebugCtrl.h"

HelloWorld::HelloWorld()
{
	s_scale=1.189;
	s_no=1;
}

HelloWorld::~HelloWorld()
{
	//SimpleAudioEngine::sharedEngine()->end();
	;
}

void HelloWorld::qqLoadCallback(Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	/*auto scene = Scene::create();
	auto layer = new QQLoad(s_scale,s_no);
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);*/
	auto curButton=(Button*)pSender;
	auto VoiceEffect=CallFunc::create([=](){
		SimpleAudioEngine::sharedEngine()->playEffect("Music/ui_click.ogg");	
	});
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			this->runAction(VoiceEffect);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void HelloWorld::imLoadCallback(Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto VoiceEffect=CallFunc::create([=](){
		SimpleAudioEngine::sharedEngine()->playEffect("Music/ui_click.ogg");	
	});
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			this->runAction(VoiceEffect);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}

    KwxMessenger aMessenger = KwxMessenger(MSG_LOGIN);
    RequestLogin aReq;
    aReq.Set();

    aMessenger.StartReceiving();
    aMessenger.Send(aReq);

    auto scene = Scene::create();
	auto layer = IMLoad::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}


void HelloWorld::enterroomCallback(Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonClickVoice=CallFunc::create([=](){
		SimpleAudioEngine::sharedEngine()->playEffect("Music/ui_click.ogg");
	});
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto race=Sprite::createWithSpriteFrameName("danjichang.png");
			race->setAnchorPoint(Point(0.5f,0.5f));
			race->setPosition(Point(visibleSize.width/2 + origin.x,origin.y+visibleSize.height*0.03));
			this->addChild(race);
			auto fadeOut=FadeOut::create(0.3);
			auto easeBounce=ScaleTo::create(0.3,1.3);
			auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
			auto danjiPhoto_action=TargetedAction::create(race,spawn);
			auto enterRoom=CallFunc::create(CC_CALLBACK_0(HelloWorld::enterRoomStandAlone,this));
			this->runAction(Sequence::create(Spawn::create(danjiPhoto_action,ButtonClickVoice,NULL),DelayTime::create(0.3),enterRoom,NULL));
			//scheduleOnce(schedule_selector(HelloWorld::enterRoomStandAlone),0.4);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void HelloWorld::enterRoomStandAlone()
{
	auto scene = Scene::create();
	SpriteFrameCache::getInstance()->removeSpriteFrames();
    TextureCache::sharedTextureCache()->removeAllTextures();

#ifndef NETWORK_GAME_DEBUG
    RaceLayer *layer = RaceLayer::create();
    scene->addChild(layer);
    layer->CreateRace(LOCAL_GAME);
#else
    RaceLayer *layer = RaceLayer::create();
    scene->addChild(layer);
    layer->CreateRace(NETWORK_GAME);
#endif


    Director::getInstance()->replaceScene(scene);
}

void HelloWorld::addButton()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	auto danji_race=Button::create("danjichang.png","danjichang.png","danjichang.png",UI_TEX_TYPE_PLIST);
	danji_race->addTouchEventListener(CC_CALLBACK_2(HelloWorld::enterroomCallback,this));
	danji_race->setAnchorPoint(Vec2(0.5,0.5));
	danji_race->setPosition(Vec2(visibleSize.width/2 + origin.x,origin.y+visibleSize.height*0.03));
	this->addChild(danji_race,1);
	
	auto qq_login=Button::create("QQdenglu1.png","QQdenglu2.png","QQdenglu2.png",UI_TEX_TYPE_PLIST);
	qq_login->setAnchorPoint(Vec2(0,0));
	qq_login->setPosition(Vec2(visibleSize.width*7/12 + origin.x, visibleSize.height/10 + origin.y));
	qq_login->addTouchEventListener(CC_CALLBACK_2(HelloWorld::qqLoadCallback,this));
	this->addChild(qq_login,1);

	auto now_login=Button::create("lijidenglu1.png","lijidenglu2.png","lijidenglu2.png",UI_TEX_TYPE_PLIST);
	now_login->setAnchorPoint(Vec2(0,0));
	now_login->setPosition(Vec2(visibleSize.width*2/12 + origin.x, visibleSize.height/10 + origin.y));
	now_login->addTouchEventListener(CC_CALLBACK_2(HelloWorld::imLoadCallback,this));
	this->addChild(now_login,1);
	///SpriteFrameCache::getInstance()->removeSpriteFrames();
}

bool HelloWorld::init()
{
	if(!Layer::init())
	{
		return false;
	}
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("Music/playing.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("Music/majiang_logo.ogg");

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	Sprite *sprite;
	if(s_no == 1)
		sprite = Sprite::create("loadwin.png");
	else
		sprite = Sprite::create("loadwin2.png");
	sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	sprite->setScaleX(s_scale);
	sprite->setScaleY(s_scale);
	this->addChild(sprite,0);
	//SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/playing.mp3",true);
	//auto cache=SpriteFrameCache::sharedSpriteFrameCache();
	//cache->addSpriteFramesWithFile("dengluzhutu.plist","dengluzhutu.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dengluzhutu.plist");
	auto log_bg1=Sprite::createWithSpriteFrameName("logo.png");
	log_bg1->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height*31/48 + origin.y));
	this->addChild(log_bg1,1);
	log_bg1->setScale(0.1f);

	auto easeBounceInOut1= EaseBounceOut::create(ScaleTo::create(0.5,1));
	//log_bg1->runAction(easeBounceInOut1);
	auto log_bg1_action=TargetedAction::create(log_bg1,easeBounceInOut1);

	auto log_bg2=Sprite::createWithSpriteFrameName("logo.png");
	log_bg2->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height*31/48 + origin.y));
	log_bg2->setOpacity(80);

	this->addChild(log_bg2,1);
	log_bg2->setScale(0.1f);

	auto easeBounceInOut2= EaseBounceOut::create(ScaleTo::create(0.5,1));
	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	auto seqFor2=Sequence::create(easeBounceInOut2,spawn,NULL);
	//log_bg2->runAction(seqFor2);
	auto log_bg2_action=TargetedAction::create(log_bg2,seqFor2);

	auto start_log= Sprite::createWithSpriteFrameName("kwx.png");
	auto lightEffect=Sprite::createWithSpriteFrameName("guang.png");     
	CCClippingNode* cliper = CCClippingNode::create();       
	cliper->setStencil(start_log);  
	cliper->setContentSize(Size(start_log->getContentSize().width,start_log->getContentSize().height));//设置裁剪节点大小
	cliper->setAlphaThreshold(0);  
	cliper->setPosition(ccp(origin.x+visibleSize.width*0.495,origin.y+visibleSize.height*0.57));
	cliper->addChild(start_log,1);
	lightEffect->setPositionX(-start_log->getContentSize().width/2);
	cliper->addChild(lightEffect,2);
	this->addChild(cliper,2);  
	cliper->setScale(0.1f);

	auto easeBounceInOut3= EaseBounceOut::create(ScaleTo::create(0.5,1));
	//cliper->runAction(easeBounceInOut3);
	auto cliper_action=TargetedAction::create(cliper,easeBounceInOut3);

	auto actionTo=MoveTo::create(0.8,Vec2(start_log->getContentSize().width/2,0));
	auto callFunc=CallFunc::create(this,callfunc_selector(HelloWorld::addButton));
	auto seqForLight=Sequence::create(DelayTime::create(0.5),actionTo,DelayTime::create(0.5),callFunc,NULL);
	//lightEffect->runAction(seqForLight);
	auto lightEffect_action=TargetedAction::create(lightEffect,seqForLight);

	auto LogoVoiceEffect=CallFunc::create([=](){SimpleAudioEngine::sharedEngine()->playEffect("Music/majiang_logo.ogg");});
	SimpleAudioEngine::sharedEngine()->preloadEffect("Music/ui_click.ogg");
	auto thisSpaen=Spawn::create(log_bg1_action,log_bg2_action,cliper_action,lightEffect_action,NULL);
	this->runAction(Spawn::create(LogoVoiceEffect,thisSpaen,NULL));

    DeviceInfo_t info;
    _get_device_info(info);
    
    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
