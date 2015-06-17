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
void HelloWorld::onEnterTransitionDidFinish()
{
	set_userDefault();
	bool ifYinYueOn;
	bool ifYinXiaoOn;
	bool ifZhenDongOn;
	bool ifFangYanOn; 
	auto userDefault=UserDefault::getInstance();
	ifYinYueOn=userDefault->getBoolForKey("ifYinYueOn");
	ifYinXiaoOn=userDefault->getBoolForKey("ifYinXiaoOn");
	ifZhenDongOn=userDefault->getBoolForKey("ifZhenDongOn");
	ifFangYanOn=userDefault->getBoolForKey("ifFangYanOn");
	if(ifYinYueOn)
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/playing.mp3",true);
	else
	{
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/playing.mp3",true);
	}
	if(!ifYinXiaoOn)
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);

}
HelloWorld::~HelloWorld()
{
	//SimpleAudioEngine::sharedEngine()->end();
	;
}

void HelloWorld::qqLoadCallback(Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto curButton=(Button*)pSender;
			auto VoiceEffect=CallFunc::create([=](){
				SimpleAudioEngine::sharedEngine()->playEffect("Music/ui_click.ogg");	
			});

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
void HelloWorld::LogInWithAccountBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto HideCur=CallFunc::create([=](){
				if(this->getChildByTag(LOGIN_ACCOUNT))
				{
					auto curBkg=this->getChildByTag(LOGIN_ACCOUNT);
					curBkg->runAction(FadeOut::create(0.2));
				}
			});
			auto DeleteCur=CallFunc::create([=](){
				if(this->getChildByTag(LOGIN_ACCOUNT))
					this->removeChildByTag(LOGIN_ACCOUNT,true);
			});
			this->runAction(Sequence::create(HideCur,DeleteCur,NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void HelloWorld::RememberChoose(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curBtn=(Button *)pSender;
			auto curFlag=(Sprite *)curBtn->getChildByTag(CHOOSE_FLAG);
			if(curFlag->isVisible())
				curFlag->setVisible(false);
			else
				curFlag->setVisible(true);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void HelloWorld::RegistCall(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto curFunc=CallFunc::create([=](){
				RegisteredAccount();
			});
			this->runAction(curFunc);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void HelloWorld::LogInGame(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto scene = Scene::create();
			SpriteFrameCache::getInstance()->removeSpriteFrames();
			TextureCache::sharedTextureCache()->removeAllTextures();
			RaceLayer *layer = RaceLayer::create();
			scene->addChild(layer);
#ifndef NETWORK_GAME_DEBUG
			layer->CreateRace(LOCAL_GAME);//NETWORK_GAME
#else
			layer->CreateRace(NETWORK_GAME);
#endif

			Director::getInstance()->replaceScene(scene);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void HelloWorld::RegisterBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto HideCur=CallFunc::create([=](){
				if(this->getChildByTag(REGISTER_LAYER))
				{
					auto curBkg=this->getChildByTag(REGISTER_LAYER);
					curBkg->runAction(FadeOut::create(0.2));
				}
			});
			auto DeleteCur=CallFunc::create([=](){
				if(this->getChildByTag(REGISTER_LAYER))
					this->removeChildByTag(REGISTER_LAYER,true);
			});
			this->runAction(Sequence::create(HideCur,DeleteCur,NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void HelloWorld::GetVerificationCode(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void HelloWorld::LogInWithAccount()
{
	auto LogBak=Sprite::create("register-LoginBackground.png");
	auto BakSize=LogBak->getContentSize();
	LogBak->setAnchorPoint(Vec2(0.5,0.5));
	LogBak->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(LogBak,3,LOGIN_ACCOUNT);

	auto SwallowLayer=LayerColor::create();
	SwallowLayer->setContentSize(Size(visibleSize.width,visibleSize.height));
	//SwallowLayer->setColor(Color3B(28,120,135));
	SwallowLayer->setColor(ccWHITE);
	SwallowLayer->setOpacity(50);
	SwallowLayer->ignoreAnchorPointForPosition(false);
	SwallowLayer->setAnchorPoint(Vec2(0.5,0.5));
	SwallowLayer->setPosition(Vec2(BakSize.width/2,BakSize.height/2));
	LogBak->addChild(SwallowLayer,1);
	EventListenerTouchOneByOne* SwallowListener=EventListenerTouchOneByOne::create();
	SwallowListener->setSwallowTouches(true);
	SwallowListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	SwallowListener->onTouchMoved=[=](Touch* touch, Event* event){};
	SwallowListener->onTouchEnded=[=](Touch* touch, Event* event){};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(SwallowListener,SwallowLayer);

	auto LogFont=Sprite::createWithSpriteFrameName("Login-yxzhdl.png");
	LogFont->setAnchorPoint(Vec2(0.5,0.5));
	LogFont->setPosition(Vec2(BakSize.width/2,BakSize.height*0.9));
	LogBak->addChild(LogFont,2);

	auto backBtn=Button::create("register-Return.png","register-Return.png","register-Return.png",UI_TEX_TYPE_PLIST);
	backBtn->setAnchorPoint(Vec2(0.5,0.5));
	backBtn->setPosition(Vec2(backBtn->getContentSize().width,BakSize.height*0.9));
	backBtn->addTouchEventListener(CC_CALLBACK_2(HelloWorld::LogInWithAccountBack,this));
	LogBak->addChild(backBtn,2);

	auto Account=Sprite::createWithSpriteFrameName("Login-mobile.png");
	Account->setAnchorPoint(Vec2(0.5,0.5));
	Account->setPosition(Vec2(BakSize.width/2,BakSize.height*0.7));
	LogBak->addChild(Account,2);

	auto KeyForAccount=Sprite::createWithSpriteFrameName("Login-mm.png");
	KeyForAccount->setAnchorPoint(Vec2(0.5,0.5));
	KeyForAccount->setPosition(Vec2(BakSize.width/2,BakSize.height*0.5));
	LogBak->addChild(KeyForAccount,2);

	auto chooseBtn=Button::create("register-Selectbox.png","register-Selectbox.png","register-Selectbox.png",UI_TEX_TYPE_PLIST);
	chooseBtn->setAnchorPoint(Vec2(0.5,0.5));
	chooseBtn->setPosition(Vec2(BakSize.width*0.13,BakSize.height*0.37));
	chooseBtn->addTouchEventListener(CC_CALLBACK_2(HelloWorld::RememberChoose,this));
	LogBak->addChild(chooseBtn,2);

	auto chooseedFlag=Sprite::createWithSpriteFrameName("register-Selected.png");
	chooseedFlag->setAnchorPoint(Vec2(0.5,0.5));
	chooseedFlag->setPosition(Vec2(chooseBtn->getContentSize().width/2,chooseBtn->getContentSize().height/2));
	chooseBtn->addChild(chooseedFlag,1,CHOOSE_FLAG);

	auto AccountRegist=Button::create("register-botton.png","register-botton2.png","register-botton2.png",UI_TEX_TYPE_PLIST);
	AccountRegist->setAnchorPoint(Vec2(0.5,0.5));
	AccountRegist->setPosition(Vec2(BakSize.width*0.334-40,BakSize.height*0.163));
	AccountRegist->addTouchEventListener(CC_CALLBACK_2(HelloWorld::RegistCall,this));
	LogBak->addChild(AccountRegist,2);

	auto LogInGame=Button::create("Login-botton.png","Login-botton2.png","Login-botton2.png",UI_TEX_TYPE_PLIST);
	LogInGame->setAnchorPoint(Vec2(0.5,0.5));
	LogInGame->setPosition(Vec2(BakSize.width*0.74,BakSize.height*0.163));
	LogInGame->addTouchEventListener(CC_CALLBACK_2(HelloWorld::LogInGame,this));
	LogBak->addChild(LogInGame,2);
}
void HelloWorld::RegisteredAccount()
{
	auto RegistBak=Sprite::create("register-LoginBackground.png");
	auto BakSize=RegistBak->getContentSize();
	RegistBak->setAnchorPoint(Vec2(0.5,0.5));
	RegistBak->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(RegistBak,4,REGISTER_LAYER);

	auto RegistFont=Sprite::createWithSpriteFrameName("Login-yxzhdl.png");
	RegistFont->setAnchorPoint(Vec2(0.5,0.5));
	RegistFont->setPosition(Vec2(BakSize.width/2,BakSize.height*0.9));
	RegistBak->addChild(RegistFont,1);

	auto backBtn=Button::create("register-Return.png","register-Return.png","register-Return.png",UI_TEX_TYPE_PLIST);
	backBtn->setAnchorPoint(Vec2(0.5,0.5));
	backBtn->setPosition(Vec2(backBtn->getContentSize().width,BakSize.height*0.9));
	backBtn->addTouchEventListener(CC_CALLBACK_2(HelloWorld::RegisterBack,this));
	RegistBak->addChild(backBtn,1);

	auto RegistNum=Sprite::createWithSpriteFrameName("register-mobile.png");
	RegistNum->setAnchorPoint(Vec2(0,0.5));
	RegistNum->setPosition(Vec2(BakSize.width*0.064,BakSize.height*0.7));
	RegistBak->addChild(RegistNum,1);

	auto RegistKey=Sprite::createWithSpriteFrameName("register-mm.png");
	RegistKey->setAnchorPoint(Vec2(0,0.5));
	RegistKey->setPosition(Vec2(BakSize.width*0.064,BakSize.height*0.55));
	RegistBak->addChild(RegistKey,1);

	auto RegistCode =Sprite::createWithSpriteFrameName("register-yzm.png");
	RegistCode->setAnchorPoint(Vec2(0,0.5));
	RegistCode->setPosition(Vec2(BakSize.width*0.064,BakSize.height*0.382));
	RegistBak->addChild(RegistCode,1);

	auto getCode=Button::create("register-message.png","register-message2.png","register-message2.png",UI_TEX_TYPE_PLIST);
	getCode->setAnchorPoint(Vec2(0,0.5));
	getCode->setPosition(Vec2(RegistCode->getPosition().x+RegistCode->getContentSize().width+15,RegistCode->getPosition().y));
	getCode->addTouchEventListener(CC_CALLBACK_2(HelloWorld::GetVerificationCode,this));
	RegistBak->addChild(getCode,1);

	auto SubmitRegist=Button::create("lijidenglu1.png","lijidenglu2.png","lijidenglu2.png",UI_TEX_TYPE_PLIST);
	SubmitRegist->setAnchorPoint(Vec2(0.5,0.5));
	SubmitRegist->setPosition(Vec2(BakSize.width/2+10,BakSize.height*0.16));
	RegistBak->addChild(SubmitRegist,1);
}

void HelloWorld::imLoadCallback(Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto curButton=(Button*)pSender;
			auto VoiceEffect=CallFunc::create([=](){
				SimpleAudioEngine::sharedEngine()->playEffect("Music/ui_click.ogg");	
			});

			curButton->setTouchEnabled(false);
			this->runAction(VoiceEffect);
            
#ifndef IGNORE_LOGIN_REQUEST
    		KwxMessenger aMessenger = KwxMessenger(MSG_LOGIN);
    		RequestLogin aReq;
    		aReq.Set();

   			aMessenger.StartReceiving();
    		aMessenger.Send(aReq);
#endif
			bool ifAccountHaved=true;
			if(!ifAccountHaved)
			{
				LogInWithAccount();
			}
			else
			{
				auto scene = Scene::create();
				auto layer = IMLoad::create();
				scene->addChild(layer);
				Director::getInstance()->replaceScene(scene);
			}
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}


void HelloWorld::enterroomCallback(Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto curButton=(Button*)pSender;
			auto ButtonClickVoice=CallFunc::create([=](){
				SimpleAudioEngine::sharedEngine()->playEffect("Music/ui_click.ogg");
			});

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
	RaceLayer *layer = RaceLayer::create();
    scene->addChild(layer);
#ifndef NETWORK_GAME_DEBUG
    layer->CreateRace(LOCAL_GAME);//NETWORK_GAME
#else
    layer->CreateRace(NETWORK_GAME);
#endif

    Director::getInstance()->replaceScene(scene);
}

void HelloWorld::addButton()
{
	auto danji_race=Button::create("danjichang.png","danjichang.png","danjichang.png",UI_TEX_TYPE_PLIST);
	danji_race->addTouchEventListener(CC_CALLBACK_2(HelloWorld::enterroomCallback,this));
	danji_race->setAnchorPoint(Vec2(0.5,0.5));
	danji_race->setPosition(Vec2(visibleSize.width/2 + origin.x,origin.y+visibleSize.height*0.03));
	this->addChild(danji_race,1);
	
	auto qq_login=Button::create("QQdenglu1.png","QQdenglu2.png","QQdenglu2.png",UI_TEX_TYPE_PLIST);
	qq_login->setAnchorPoint(Vec2(0,0));
	qq_login->setPosition(Vec2(visibleSize.width*7/12 + origin.x, visibleSize.height/10 + origin.y));
	qq_login->addTouchEventListener(CC_CALLBACK_2(HelloWorld::qqLoadCallback,this));
	this->addChild(qq_login,1,QQ_LOG_IN);

	auto now_login=Button::create("lijidenglu1.png","lijidenglu2.png","lijidenglu2.png",UI_TEX_TYPE_PLIST);
	now_login->setAnchorPoint(Vec2(0,0));
	now_login->setPosition(Vec2(visibleSize.width*2/12 + origin.x, visibleSize.height/10 + origin.y));
	now_login->addTouchEventListener(CC_CALLBACK_2(HelloWorld::imLoadCallback,this));
	this->addChild(now_login,1,LIJI_LOGIN);
	///SpriteFrameCache::getInstance()->removeSpriteFrames();
}

void HelloWorld::set_userDefault()
{
	auto userDefault=UserDefault::getInstance();//UserDefault判断
	if(userDefault->getBoolForKey("userDefault")==false)
	{
		userDefault->setBoolForKey("userDefault",true);
		userDefault->setIntegerForKey("load_days",1);
		userDefault->setIntegerForKey("cur_sta",1);
		userDefault->setIntegerForKey("load_time",7352);

		userDefault->setStringForKey("MyNickName","雀友");
		userDefault->setIntegerForKey("MyProperty",50000);
		userDefault->setStringForKey("MyLevel","菜鸟");
		userDefault->setStringForKey("MyPhoto","Head17.png");
		userDefault->setStringForKey("MySex","Boy");
		std::string RobotName[16]={"十堰冰冰","随州青霞","武汉丽缇","襄阳韦彤","孝感歆艺","郧西子怡","郧县若瑄",
			"竹山紫琪","房县老李","十堰小李","随州小张","武汉关哥","襄阳小刘","孝感小王","郧西老杨","郧县小陈"};
		char buffer[80];
		for(int i=0;i<16;i++)
		{
			sprintf(buffer,"Robot%d",i);
			std::string robotName=buffer;
			sprintf(buffer,"PropertyOfRobot%d",i);
			std::string robotProperty=buffer;
			sprintf(buffer,"LevelOfRobot%d",i);
			std::string robotLevel=buffer;
			sprintf(buffer,"PhotoOfRobot%d",i);
			std::string robotPhoto=buffer;
			sprintf(buffer,"SexOfRobot%d",i);
			std::string robotSex=buffer;
			sprintf(buffer,"Head%d.png",i+1);
			std::string robotPath=buffer;
			userDefault->setStringForKey(robotName.c_str(),RobotName[i]);
			userDefault->setIntegerForKey(robotProperty.c_str(),50000);
			userDefault->setStringForKey(robotLevel.c_str(),"菜鸟");
			userDefault->setStringForKey(robotPhoto.c_str(),robotPath);
			if(i<9)
				userDefault->setStringForKey(robotSex.c_str(),"Girl");
			else
				userDefault->setStringForKey(robotSex.c_str(),"Boy");
		}
		sprintf(buffer,"Language%d",0);
		std::string  languageKind=buffer;
		userDefault->setStringForKey(languageKind.c_str(),"PuTongBan");

		userDefault->setBoolForKey("ifYinYueOn",true);
		userDefault->setBoolForKey("ifYinXiaoOn",true);
		userDefault->setBoolForKey("ifZhenDongOn",false);
		userDefault->setBoolForKey("ifFangYanOn",false);
		UserDefault::getInstance()->flush();
	}
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

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dengluzhutu.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("register-login.plist");

	auto log_bg1=Sprite::createWithSpriteFrameName("logo.png");
	log_bg1->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height*31/48 + origin.y));
	this->addChild(log_bg1,1);
	log_bg1->setScale(0.1f);

	auto easeBounceInOut1= EaseBounceOut::create(ScaleTo::create(0.5,1));
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
	auto cliper_action=TargetedAction::create(cliper,easeBounceInOut3);

	auto actionTo=MoveTo::create(0.8,Vec2(start_log->getContentSize().width/2,0));
	auto callFunc=CallFunc::create(this,callfunc_selector(HelloWorld::addButton));
	auto seqForLight=Sequence::create(DelayTime::create(0.5),actionTo,DelayTime::create(0.5),callFunc,NULL);
	auto lightEffect_action=TargetedAction::create(lightEffect,seqForLight);

	auto LogoVoiceEffect=CallFunc::create([=](){SimpleAudioEngine::sharedEngine()->playEffect("Music/majiang_logo.ogg");});
	SimpleAudioEngine::sharedEngine()->preloadEffect("Music/ui_click.ogg");
	auto thisSpawn=Spawn::create(log_bg1_action,log_bg2_action,cliper_action,lightEffect_action,NULL);
	this->runAction(Spawn::create(LogoVoiceEffect,thisSpawn,NULL));
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
