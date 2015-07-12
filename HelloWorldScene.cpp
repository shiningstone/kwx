#include "HelloWorldScene.h"
#include "IMLoadScene.h"
#include "EnterRoomScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include "VersionUpgrade.h"

#include "game/RaceLayer.h"
#include "game/DataBase.h"
#include "protocol/DbgRequestDesc.h"
#include "protocol/CommonMsg.h"
#include "protocol/DsInstruction.h"
#include "protocol/UsRequest.h"
#include "protocol/KwxMsgLogin.h"
#include "network/KwxEnv.h"
#include "network/KwxMessenger.h"
#include "LoginAndRegister.h"

#include "ChineseChar.h"

#include "utils/DebugCtrl.h"

HelloWorld::HelloWorld()
{
	s_scale=1.189;
	s_no=1;
}
void HelloWorld::onEnterTransitionDidFinish()
{
    Database *aDatabase = Database::getInstance();
    
    if(aDatabase->IsYinYueOn()) {
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/playing.mp3",true);
    } else {
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/playing.mp3",true);
	}
    
	if(!aDatabase->IsYinXiaoOn()) {
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
    }

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

            #if (DEBUG_ENTRANCE==3)
            _showVersionUpgrade(true);
            return;
            #endif
            
#ifndef IGNORE_LOGIN_REQUEST
            EnvVariable *env = EnvVariable::getInstance();

            KwxMessenger *aMessenger = KwxMessenger::getInstance(MSG_LOGIN);
            aMessenger->StartReceiving();
            aMessenger->Send(REQ_LOGIN);
            aMessenger->StopReceiving();
            
            if(aMessenger->_response!=REQUEST_ACCEPTED) {
                string info = string("Error code ") + string(DescErr(aMessenger->_response));
                _showErrorMessage(info);
                return;
            }
            
            if(env->IsReconnectRequired()) {
                KwxMessenger *bMessenger = KwxMessenger::getInstance(MSG_GAME);
                bMessenger->StartReceiving();
                bMessenger->Send(REQ_GAME_SEND_RECONNECT);

                auto scene = Scene::create();
                RaceLayer *layer = RaceLayer::create();
                scene->addChild(layer);
                layer->CreateRace(NETWORK_GAME);
                Director::getInstance()->replaceScene(scene);
                return;
            } else if(aMessenger->_response==VERSION_TOO_OLD) {
                _showVersionUpgrade(true);
                return;
            } else if(aMessenger->_response==NEW_VERSION_AVAILABLE) {
                _showVersionUpgrade(false);
                return;
            } else if(aMessenger->_response==NEW_RES_AVAILABLE) {

            } else if(aMessenger->_response==SERVER_DATA_ERROR) {

            } else {
                KwxHeart::getInstance();
            }
#endif

            bool ifAccountHaved=true;
            if(!ifAccountHaved)
            {
                auto LogIn_Register=new LoginAndRegister(this);
				this->addChild(LogIn_Register,5,LOGIN_REGISTER);
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

#if (DEBUG_ENTRANCE==1)
    layer->CreateRace(NETWORK_GAME);
    Director::getInstance()->replaceScene(scene);
#elif (DEBUG_ENTRANCE==2)
    extern void test_interface();
    test_interface();
#elif (DEBUG_ENTRANCE==4)
    KwxMessenger *aMessenger = KwxMessenger::getInstance(MSG_GAME);
    aMessenger->StartReceiving();
    aMessenger->Send(REQ_DAILY_LOGIN);
    
    //KwxMessenger *bMessenger = KwxMessenger::getInstance(MSG_GAME);
    aMessenger->Send(REQ_GET_DAILY_PRIZE);
#elif (DEBUG_ENTRANCE==5)
    _showErrorMessage("error message");
#else
    layer->CreateRace(LOCAL_GAME);
    Director::getInstance()->replaceScene(scene);
#endif
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

		userDefault->setStringForKey("MyNickName",QueYou);
		userDefault->setIntegerForKey("MyProperty",50000);
		userDefault->setStringForKey("MyLevel",CaiNiao);
		userDefault->setStringForKey("MyPhoto","Head17.png");
		userDefault->setStringForKey("MySex","Boy");
		std::string RobotName[16]={
            ShiYanBingBing,  SuiZhouQingXia,  WuHanLiti,         XiangYangWeiTong,  XiaoGanXinYi,
            YunXiZiYi,       YunXianRuoXuan,  ZuShanZiQi,        FangXianLaoLi,     ShiYanXiaoLi,
            SuiZhouXiaoZhang,WuHanGuanGe,     XiangYangXiaoLiu , XiaoGanXiaoWang,   YunXiLaoYang,
            YunXianXiaoChen, 
        };
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
			userDefault->setStringForKey(robotLevel.c_str(),CaiNiao);
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

void HelloWorld::_showVersionUpgrade(bool forceUpgrade) {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("systemprompt.plist");
    Layer* prompt = new VersionUpgrade(forceUpgrade);
    prompt->setVisible(true);
    this->addChild(prompt,99);
}

#include "SystemMessageHint.h"
void HelloWorld::_showErrorMessage(std::string errorMessage) {
    Layer* prompt = new SystemMessageHint(this,errorMessage,98);
    prompt->setVisible(true);
    this->addChild(prompt,98);
}

