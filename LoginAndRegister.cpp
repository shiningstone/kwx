#include "LoginAndRegister.h"
#include "game/RaceLayer.h"

LoginAndRegister::LoginAndRegister(Node* p)
{
	parentNode=p;
}
LoginAndRegister::~LoginAndRegister(void)
{
}
bool LoginAndRegister::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();
	timing=0;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("register-login.plist");

	auto SwallowLayer=LayerColor::create();
	SwallowLayer->setContentSize(Size(visibleSize.width,visibleSize.height));
	SwallowLayer->setColor(ccWHITE);
	SwallowLayer->setOpacity(50);
	SwallowLayer->ignoreAnchorPointForPosition(false);
	SwallowLayer->setAnchorPoint(Vec2(0.5,0.5));
	SwallowLayer->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(SwallowLayer,1);
	EventListenerTouchOneByOne* SwallowListener=EventListenerTouchOneByOne::create();
	SwallowListener->setSwallowTouches(true);
	SwallowListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	SwallowListener->onTouchMoved=[=](Touch* touch, Event* event){};
	SwallowListener->onTouchEnded=[=](Touch* touch, Event* event){};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(SwallowListener,SwallowLayer);

	auto LogBak=Sprite::create("register-LoginBackground.png");
	auto BakSize=LogBak->getContentSize();
	LogBak->setAnchorPoint(Vec2(0.5,0.5));
	LogBak->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	SwallowLayer->addChild(LogBak,1,LOGIN_ACCOUNT);

	auto LogFont=Sprite::createWithSpriteFrameName("Login-yxzhdl.png");
	LogFont->setAnchorPoint(Vec2(0.5,0.5));
	LogFont->setPosition(Vec2(BakSize.width/2,BakSize.height*0.9));
	LogBak->addChild(LogFont,2);

	auto backBtn=Button::create("register-Return.png","register-Return.png","register-Return.png",UI_TEX_TYPE_PLIST);
	backBtn->setAnchorPoint(Vec2(0.5,0.5));
	backBtn->setPosition(Vec2(backBtn->getContentSize().width,BakSize.height*0.9));
	backBtn->addTouchEventListener(CC_CALLBACK_2(LoginAndRegister::LogInWithAccountBack,this));
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
	chooseBtn->addTouchEventListener(CC_CALLBACK_2(LoginAndRegister::RememberChoose,this));
	LogBak->addChild(chooseBtn,2);

	auto chooseedFlag=Sprite::createWithSpriteFrameName("register-Selected.png");
	chooseedFlag->setAnchorPoint(Vec2(0.5,0.5));
	chooseedFlag->setPosition(Vec2(chooseBtn->getContentSize().width/2,chooseBtn->getContentSize().height/2));
	chooseedFlag->setVisible(false);
	chooseBtn->addChild(chooseedFlag,1,CHOOSE_FLAG);

	auto AccountRegist=Button::create("register-botton.png","register-botton2.png","register-botton2.png",UI_TEX_TYPE_PLIST);
	AccountRegist->setAnchorPoint(Vec2(0.5,0.5));
	AccountRegist->setPosition(Vec2(BakSize.width*0.334-40,BakSize.height*0.163));
	AccountRegist->addTouchEventListener(CC_CALLBACK_2(LoginAndRegister::RegistCall,this));
	LogBak->addChild(AccountRegist,2);

	auto LogInGame=Button::create("Login-botton.png","Login-botton2.png","Login-botton2.png",UI_TEX_TYPE_PLIST);
	LogInGame->setAnchorPoint(Vec2(0.5,0.5));
	LogInGame->setPosition(Vec2(BakSize.width*0.74,BakSize.height*0.163));
	LogInGame->addTouchEventListener(CC_CALLBACK_2(LoginAndRegister::LogInGame,this));
	LogBak->addChild(LogInGame,2);

	return true;
}

void LoginAndRegister::RegisteredAccount()
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
	backBtn->addTouchEventListener(CC_CALLBACK_2(LoginAndRegister::RegisterBack,this));
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

	auto getCode=Button::create("register-message.png","register-message2.png","register-message3.png",UI_TEX_TYPE_PLIST);
	getCode->setAnchorPoint(Vec2(0,0.5));
	getCode->setPosition(Vec2(RegistCode->getPosition().x+RegistCode->getContentSize().width+15,RegistCode->getPosition().y));
	getCode->addTouchEventListener(CC_CALLBACK_2(LoginAndRegister::GetVerificationCode,this));
	RegistBak->addChild(getCode,1,MESSAGE_COUNT);

	auto NumLabel=LabelTTF::create("60","Arial",20);
	NumLabel->setAnchorPoint(Vec2(0.5,0.5));
	NumLabel->setPosition(Vec2(getCode->getContentSize().width/2,getCode->getContentSize().height/2));
	getCode->setVisible(false);
	getCode->addChild(NumLabel,1,COUNT_NUM);

	auto SubmitRegist=Button::create("lijidenglu1.png","lijidenglu2.png","lijidenglu2.png",UI_TEX_TYPE_PLIST);//Ìá½»×¢²á
	SubmitRegist->setAnchorPoint(Vec2(0.5,0.5));
	SubmitRegist->setPosition(Vec2(BakSize.width/2+10,BakSize.height*0.16));
	RegistBak->addChild(SubmitRegist,1);
}
void LoginAndRegister::LogInWithAccountBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
				if(parentNode->getChildByTag(MESSAGE_COUNT))
					parentNode->removeChildByTag(MESSAGE_COUNT,true);
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
void LoginAndRegister::RememberChoose(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
void LoginAndRegister::RegistCall(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
void LoginAndRegister::LogInGame(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
#ifndef DEBUG_ENTRANCE
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
void LoginAndRegister::RegisterBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
void LoginAndRegister::GetVerificationCode(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto curBtn=(Button *)pSender;
			curBtn->setTouchEnabled(false);
			curBtn->setBright(true);
			curBtn->getChildByTag(COUNT_NUM)->setVisible(true);
			timing=60;
			schedule(schedule_selector(LoginAndRegister::updateNum),1.0f);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void LoginAndRegister::updateNum(float delta)
{
	char tempChar[5];
	auto messageCount=(LabelTTF*)(this->getChildByTag(REGISTER_LAYER)->getChildByTag(MESSAGE_COUNT)->getChildByTag(COUNT_NUM));
	--timing;
	if(timing<0)
	{
		timing=60;
		messageCount->setVisible(false);
		sprintf(tempChar,"%d",timing);
		messageCount->setString(tempChar);

		auto curBtn=(Button*)(this->getChildByTag(REGISTER_LAYER)->getChildByTag(MESSAGE_COUNT));
		curBtn->setTouchEnabled(true);
		this->unschedule(schedule_selector(LoginAndRegister::updateNum));
		return;
	}
	sprintf(tempChar,"%d",timing);
	messageCount->setString(tempChar);
}