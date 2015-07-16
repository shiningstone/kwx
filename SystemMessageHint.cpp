#include "SystemMessageHint.h"
#include "HelloWorldScene.h"

SystemMessageHint::SystemMessageHint(std::string str,MES_HINT_ACTION curAct)
{
	NoticeMessage=str;
	curMesAction=curAct;
	init();
}
SystemMessageHint::~SystemMessageHint(void)
{
}
bool SystemMessageHint::init()
{
	if(!Layer::init())
	{
		return false;
	}

	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("systemprompt.plist");

	auto mesBkg=Sprite::create("system_Notice.png");
	mesBkg->setAnchorPoint(Vec2(0.5,0.5));
	mesBkg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(mesBkg,0);

	switch (curMesAction)
	{
	case mes_Hint_Ensure_Only:
		HintOnlyEnsure();
		break;
	case mes_Hint_Choose:
		HintWihtChoose();
		break;
	default:
		break;
	}

	auto NotieHint=Sprite::create("system_Notice.png");
	NotieHint->setAnchorPoint(Vec2(0.5,0.5));
	NotieHint->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(NotieHint,1,SYSTEM_MES_HINT_BKG);

	auto MessageFont=LabelTTF::create(NoticeMessage,"Arial",20);
	MessageFont->setAnchorPoint(Vec2(0.5,0.5));
	MessageFont->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(MessageFont,2);

	return true;
}
void SystemMessageHint::OnlyEnsureCall(cocos2d::Ref* pSender,ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto scene=Scene::create();
			auto layer=HelloWorld::create();
			scene->addChild(layer);
			Director::getInstance()->replaceScene(scene);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void SystemMessageHint::HintOnlyEnsure()
{
	auto NoticeHint=(Sprite*)this->getChildByTag(SYSTEM_MES_HINT_BKG);

	auto ensureBtn=Button::create("systemprompt-affirm.png","systemprompt-affirm2.png","systemprompt-affirm2.png",UI_TEX_TYPE_PLIST);
	ensureBtn->setAnchorPoint(Vec2(0.5,0));
	ensureBtn->setPosition(Vec2(visibleSize.width/2,NoticeHint->getPosition().y-NoticeHint->getContentSize().height/2+15));
	ensureBtn->addTouchEventListener(CC_CALLBACK_2(SystemMessageHint::OnlyEnsureCall,this));
	this->addChild(ensureBtn);
}
void SystemMessageHint::HintWihtChoose()
{
	auto NoticeHint=(Sprite*)this->getChildByTag(SYSTEM_MES_HINT_BKG);
	auto HintSize=NoticeHint->getContentSize();

	auto ensureBtn=Button::create("systemprompt-affirm.png","systemprompt-affirm2.png","systemprompt-affirm2.png",UI_TEX_TYPE_PLIST);
	ensureBtn->setAnchorPoint(Vec2(0.5,0));
	ensureBtn->setPosition(Vec2(visibleSize.width/2-HintSize.width*0.2026144,NoticeHint->getPosition().y-NoticeHint->getContentSize().height/2+15));
	ensureBtn->addTouchEventListener(CC_CALLBACK_2(SystemMessageHint::ChooseEnsure,this));
	this->addChild(ensureBtn); 

	auto cancelBtn=Button::create("systemprompt-cancel.png","systemprompt-cancel2.png","systemprompt-cancel2.png",UI_TEX_TYPE_PLIST);
	cancelBtn->setAnchorPoint(Vec2(0.5,0));
	cancelBtn->setPosition(Vec2(visibleSize.width/2+HintSize.width*0.2026144,NoticeHint->getPosition().y-NoticeHint->getContentSize().height/2+15));
	cancelBtn->addTouchEventListener(CC_CALLBACK_2(SystemMessageHint::ChooseCancel,this));
	this->addChild(cancelBtn); 
}
void SystemMessageHint::ChooseEnsure(cocos2d::Ref* pSender,ui::Widget::TouchEventType type)
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
void SystemMessageHint::ChooseCancel(cocos2d::Ref* pSender,ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:	
		{
			auto scene=Scene::create();
			auto layer=HelloWorld::create();
			scene->addChild(layer);
			Director::getInstance()->replaceScene(scene);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}