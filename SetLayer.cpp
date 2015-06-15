#include "SetLayer.h"

SetLayer::SetLayer(void)
{
}


SetLayer::~SetLayer(void)
{
}

bool SetLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	auto visiableSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	this->setContentSize(Size(visiableSize.width,visiableSize.height*564/716));

	voiceOnFlag=true;
	voiceEffectOnFlag=true;
	vibrationOnFlag=true;

	auto imageOfVoice=Sprite::createWithSpriteFrameName("shengyin.png");
	imageOfVoice->setAnchorPoint(Vec2(0,0));
	imageOfVoice->setPosition(origin.x+visiableSize.width*0.055,origin.y+visiableSize.height*0.6828);
	this->addChild(imageOfVoice,1);

	auto imageOfVoiceEffect=Sprite::createWithSpriteFrameName("yinxiao.png");
	imageOfVoiceEffect->setAnchorPoint(Vec2(0,0));
	imageOfVoiceEffect->setPosition(origin.x+visiableSize.width*0.055,origin.y+visiableSize.height*0.4028);
	this->addChild(imageOfVoiceEffect,1);

	auto imageOfVibration=Sprite::createWithSpriteFrameName("zhendong.png");
	imageOfVibration->setAnchorPoint(Vec2(0,0));
	imageOfVibration->setPosition(origin.x+visiableSize.width*0.055,origin.y+visiableSize.height*0.1228);
	this->addChild(imageOfVibration,1);

	auto imageOfPlayCard=Sprite::createWithSpriteFrameName("Dialect.png");
	imageOfPlayCard->setAnchorPoint(Vec2(0,0));
	imageOfPlayCard->setPosition(origin.x+visiableSize.width*0.476,origin.y+visiableSize.height*0.6828);
	this->addChild(imageOfPlayCard,1);

	auto imageOfHelp=Sprite::createWithSpriteFrameName("bangzhu.png");
	imageOfHelp->setAnchorPoint(Vec2(0,0));
	imageOfHelp->setPosition(origin.x+visiableSize.width*0.4737,origin.y+visiableSize.height*0.4028);
	this->addChild(imageOfHelp,1);

	auto imageOfAccountNumber=Sprite::createWithSpriteFrameName("zhanghao.png");
	imageOfAccountNumber->setAnchorPoint(Vec2(0,0));
	imageOfAccountNumber->setPosition(origin.x+visiableSize.width*0.48,origin.y+visiableSize.height*0.1228);
	this->addChild(imageOfAccountNumber,1);

	auto BgOfButton1=Button::create("yidakai.png","yidakai.png","yidakai.png",TextureResType::PLIST);
	BgOfButton1->setAnchorPoint(Vec2(0,0));
	BgOfButton1->setPosition(Vec2(origin.x+visiableSize.width*0.18,origin.y+visiableSize.height*0.683));
	BgOfButton1->addTouchEventListener(CC_CALLBACK_2(SetLayer::onButtonVoice, this));
	this->addChild(BgOfButton1,1);

	auto voiceSwitchButton=Button::create("kaiguan.png","kaiguan2.png","kaiguan2.png",TextureResType::PLIST);
	voiceSwitchButton->setAnchorPoint(Vec2(0,0));
	voiceSwitchButton->setPosition(Vec2(origin.x+visiableSize.width*0.25,origin.y+visiableSize.height*0.677));
	voiceSwitchButton->addTouchEventListener(CC_CALLBACK_2(SetLayer::onButtonVoice, this));
	this->addChild(voiceSwitchButton,1,1);

	auto BgOfButton2=Button::create("yidakai.png","yidakai.png","yidakai.png",TextureResType::PLIST);
	BgOfButton2->setAnchorPoint(Vec2(0,0));
	BgOfButton2->setPosition(Vec2(origin.x+visiableSize.width*0.18,origin.y+visiableSize.height*0.403));
	BgOfButton2->addTouchEventListener(CC_CALLBACK_2(SetLayer::onButtonVoiceEffect, this));
	this->addChild(BgOfButton2,1);

	auto voiceEffectButton=Button::create("kaiguan.png","kaiguan2.png","kaiguan2.png",TextureResType::PLIST);
	voiceEffectButton->setAnchorPoint(Vec2(0,0));
	voiceEffectButton->setPosition(Vec2(origin.x+visiableSize.width*0.25,origin.y+visiableSize.height*0.397));
	voiceEffectButton->addTouchEventListener(CC_CALLBACK_2(SetLayer::onButtonVoiceEffect, this));
	this->addChild(voiceEffectButton,1,2);

	auto BgOfButton3=Button::create("yidakai.png","yidakai.png","yidakai.png",TextureResType::PLIST);
	BgOfButton3->setAnchorPoint(Vec2(0,0));
	BgOfButton3->setPosition(Vec2(origin.x+visiableSize.width*0.18,origin.y+visiableSize.height*0.123));
	BgOfButton3->addTouchEventListener(CC_CALLBACK_2(SetLayer::onButtonVibration, this));
	this->addChild(BgOfButton3,1);

	auto VibrationButton=Button::create("kaiguan.png","kaiguan2.png","kaiguan2.png",TextureResType::PLIST);
	VibrationButton->setAnchorPoint(Vec2(0,0));
	VibrationButton->setPosition(Vec2(origin.x+visiableSize.width*0.25,origin.y+visiableSize.height*0.117));
	VibrationButton->addTouchEventListener(CC_CALLBACK_2(SetLayer::onButtonVibration, this));
	this->addChild(VibrationButton,1,3);
	

	auto onceClickButton=Button::create("bottn-Mandarin2.png","bottn-Mandarin.png","bottn-Mandarin.png",TextureResType::PLIST);
	onceClickButton->setAnchorPoint(Vec2(0,0));
	onceClickButton->setPosition(Vec2(origin.x+visiableSize.width*0.5977,origin.y+visiableSize.height*0.6594));
	onceClickButton->addTouchEventListener(CC_CALLBACK_2(SetLayer::onButtonOnceClick, this));
	this->addChild(onceClickButton,2,4);
	onceClickButton->setHighlighted(true);
	onceClickButton->setTouchEnabled(false);

	auto doubleClickButton=Button::create("bottn-Dialect2.png","bottn-Dialect.png","bottn-Dialect.png",TextureResType::PLIST);
	doubleClickButton->setAnchorPoint(Vec2(0,0));
	doubleClickButton->setPosition(Vec2(origin.x+visiableSize.width*0.763-4,origin.y+visiableSize.height*0.66));
	doubleClickButton->addTouchEventListener(CC_CALLBACK_2(SetLayer::onButtonDoubleClick, this));
	this->addChild(doubleClickButton,2,5);
	doubleClickButton->setHighlighted(false);
	doubleClickButton->setTouchEnabled(true);

	auto helpButton=Button::create("chakanbangzhu1.png","chakanbangzhu2.png","chakanbangzhu2.png",TextureResType::PLIST);
	helpButton->setAnchorPoint(Vec2(0,0));
	helpButton->setPosition(Vec2(origin.x+visiableSize.width*0.5977,origin.y+visiableSize.height*0.37));
	this->addChild(helpButton,2);

	auto backLoadButton=Button::create("tuichudenglu1.png","tuichudenglu2.png","tuichudenglu2.png",TextureResType::PLIST);
	backLoadButton->setAnchorPoint(Vec2(0,0));
	backLoadButton->setPosition(Vec2(origin.x+visiableSize.width*0.5977,origin.y+visiableSize.height*0.0806));
	this->addChild(backLoadButton);
	
	return true;
}

void SetLayer::onButtonVoice(Ref *pSender, Widget::TouchEventType type)
{
	auto visiableSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	auto button=(Button*)this->getChildByTag(1);
	auto startPosition=button->getTouchStartPos();
	auto movePosition=button->getTouchEndPos();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		if(voiceOnFlag)
		{
			auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.16,origin.y+visiableSize.height*0.677));
			button->runAction(action);
			voiceOnFlag=false;
		}
		else
		{
			auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.25,origin.y+visiableSize.height*0.677));
			button->runAction(action);
			voiceOnFlag=true;
		}
		break;
	case Widget::TouchEventType::CANCELED:
		if(voiceOnFlag)
		{
			if((movePosition.x-startPosition.x)<0)
			{
				auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.16,origin.y+visiableSize.height*0.677));
				button->runAction(action);
				voiceOnFlag=false;
			}
		}
		else
		{
			if((movePosition.x-startPosition.x)>0)
			{
				auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.25,origin.y+visiableSize.height*0.677));
				button->runAction(action);
				voiceOnFlag=true;
			}
		}
		break;
	default:
		break;
	}
}

void SetLayer::onButtonVoiceEffect(Ref* pSender,Widget::TouchEventType type)
{
	auto visiableSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	auto button=(Button*)this->getChildByTag(2);
	auto startPosition=button->getTouchStartPos();
	auto movePosition=button->getTouchEndPos();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		if(voiceEffectOnFlag)
		{
			auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.16,origin.y+visiableSize.height*0.397));
			button->runAction(action);
			voiceEffectOnFlag=false;
		}
		else
		{
			auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.25,origin.y+visiableSize.height*0.397));
			button->runAction(action);
			voiceEffectOnFlag=true;
		}
		break;
	case Widget::TouchEventType::CANCELED:
		if(voiceEffectOnFlag)
		{
			if((movePosition.x-startPosition.x)<0)
			{
				auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.16,origin.y+visiableSize.height*0.397));
				button->runAction(action);
				voiceEffectOnFlag=false;
			}
		}
		else
		{
			if((movePosition.x-startPosition.x)>0)
			{
				auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.25,origin.y+visiableSize.height*0.397));
				button->runAction(action);
				voiceEffectOnFlag=true;
			}
		}
		break;
	default:
		break;
	}
}

void SetLayer::onButtonVibration(Ref* pSender,Widget::TouchEventType type)
{
	auto visiableSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	auto button=(Button*)this->getChildByTag(3);
	auto startPosition=button->getTouchStartPos();
	auto movePosition=button->getTouchEndPos();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		if(vibrationOnFlag)
		{
			auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.16,origin.y+visiableSize.height*0.117));
			button->runAction(action);
			vibrationOnFlag=false;
		}
		else
		{
			auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.25,origin.y+visiableSize.height*0.117));
			button->runAction(action);
			vibrationOnFlag=true;
		}
		break;
	case Widget::TouchEventType::CANCELED:
		if(vibrationOnFlag)
		{
			if((movePosition.x-startPosition.x)<0)
			{
				auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.16,origin.y+visiableSize.height*0.117));
				button->runAction(action);
				vibrationOnFlag=false;
			}
		}
		else
		{
			if((movePosition.x-startPosition.x)>0)
			{
				auto action=MoveTo::create(0.1f,Vec2(origin.x+visiableSize.width*0.25,origin.y+visiableSize.height*0.117));
				button->runAction(action);
				vibrationOnFlag=true;
			}
		}
		break;
	default:
		break;
	}
}

void SetLayer::onButtonOnceClick(Ref* pSender,Widget::TouchEventType type)
{
	auto buttonOneClick=(Button*)this->getChildByTag(4);
	auto buttonDoubleClice=(Button*)this->getChildByTag(5);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		buttonOneClick->setHighlighted(true);
		buttonOneClick->setTouchEnabled(false);
		buttonDoubleClice->setHighlighted(false);
		buttonDoubleClice->setTouchEnabled(true);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void SetLayer::onButtonDoubleClick(Ref* pSender,Widget::TouchEventType type)
{
	auto buttonOneClick=(Button*)this->getChildByTag(4);
	auto buttonDoubleClice=(Button*)this->getChildByTag(5);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		buttonOneClick->setHighlighted(false);
		buttonOneClick->setTouchEnabled(true);
		buttonDoubleClice->setHighlighted(true);
		buttonDoubleClice->setTouchEnabled(false);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void SetLayer::onButtonInterface(Ref* pSender)
{

}