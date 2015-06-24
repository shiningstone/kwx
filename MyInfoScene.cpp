#include "MyInfoScene.h"
#include "EnterRoomScene.h"
#include "MyInfo.h"
#include "SystemNotice.h"

MyInfoScene::MyInfoScene()
{
}

MyInfoScene::~MyInfoScene(void)
{
}

bool MyInfoScene::init()
{
	if(!Scene::init())
	{
		return false;
	}

	visiableSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();
	float s_scale=1.189f;

	auto bg=Sprite::create("602.png");
	bg->setPosition(Vec2(origin.x+visiableSize.width/2,origin.y+visiableSize.height/2));//609	358
	bg->setScaleX(s_scale);
	bg->setScaleY(s_scale);
	this->addChild(bg,0);

	auto paddle=LayerColor::create(Color4B(47,86,110,100));
	auto sizeOfPaddle=paddle->getContentSize();
	paddle->setContentSize(Size(visiableSize.width,visiableSize.height*564/716));
	paddle->setPosition(Vec2(origin.x,origin.y+visiableSize.height*0.0656));//0	46.9696007
	this->addChild(paddle,1);

	auto selectEffectOfInfo=Sprite::createWithSpriteFrameName("moji12.png");
	auto sizeOfEffect=selectEffectOfInfo->getContentSize();
	selectEffectOfInfo->setPosition(origin.x+visiableSize.width*397/1218,origin.y+visiableSize.height*669.5/716);//397.000031	669.5
	selectEffectOfInfo->setVisible(true);
	this->addChild(selectEffectOfInfo,1,MY_MESSAGE_MOJI);

	auto selectEffectOfCheatCode=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffectOfCheatCode->setPosition(origin.x+visiableSize.width*807/1218,origin.y+visiableSize.height*669.5/716);//807	669.5
	selectEffectOfCheatCode->setVisible(false);
	this->addChild(selectEffectOfCheatCode,1,SYSTEM_MES_MOJI);

	auto MyMessage=Button::create("wodexinxi.png","wodexinxi2.png","wodexinxi2.png",UI_TEX_TYPE_PLIST);
	MyMessage->setTouchEnabled(false);
	MyMessage->addTouchEventListener(CC_CALLBACK_2(MyInfoScene::buttonMyInfo,this));
	MyMessage->setPosition(Vec2(origin.x+visiableSize.width*395.5/1218,origin.y+visiableSize.height*668/716));
	this->addChild(MyMessage,3,MY_MESSAGE_BUTT);

	auto SystemMes=Button::create("xitonggonggao.png","xitonggonggao2.png","xitonggonggao2.png",UI_TEX_TYPE_PLIST);
	SystemMes->addTouchEventListener(CC_CALLBACK_2(MyInfoScene::buttonSystemNotice,this));
	SystemMes->setPosition(Vec2(origin.x+visiableSize.width*805/1218,origin.y+visiableSize.height*668/716));
	SystemMes->setHighlighted(true);
	this->addChild(SystemMes,3,SYSTEM_MES_BUTT);

	auto Back=Button::create("12.png","12.png","12.png",UI_TEX_TYPE_LOCAL);
	Back->addTouchEventListener(CC_CALLBACK_2(MyInfoScene::buttonBack,this));
	Back->setPosition(Vec2(origin.x+visiableSize.width*1170/1218,origin.y+visiableSize.height*671/716));
	this->addChild(Back,3);

	auto flower=Sprite::createWithSpriteFrameName("meihua.png");
	auto sizeOfFlower=flower->getContentSize();
	flower->setAnchorPoint(Vec2(1,0));
	flower->setPosition(origin.x+visiableSize.width*1.0095,origin.y-visiableSize.height*0.028);//1229.57104		-20.0480003
	this->addChild(flower,0);

	auto photoFrame=Sprite::create("huodongzhonxinkuang.png");
	photoFrame->setAnchorPoint(Vec2(0,0));
	photoFrame->setPosition(Vec2(origin.x+visiableSize.width*0.0082,origin.y+visiableSize.height*0.0698));//9.98760033		49.9767990
	this->addChild(photoFrame,2);

	auto myInfo=MyInfo::create();
	this->addChild(myInfo,4,CURENT_UI_LAYER);

	return true;
}

void MyInfoScene::buttonBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		Director::getInstance()->popScene();
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void MyInfoScene::buttonMyInfo(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curButton=(Button*)pSender;
			curButton->setTouchEnabled(false);
			curButton->setHighlighted(false);
			auto curMoji=this->getChildByTag(MY_MESSAGE_MOJI);
			curMoji->setVisible(true);

			auto anOtherBut=(Button*)this->getChildByTag(SYSTEM_MES_BUTT);
			anOtherBut->setTouchEnabled(true);
			anOtherBut->setHighlighted(true);
			auto anOtherMJ=this->getChildByTag(SYSTEM_MES_MOJI);
			anOtherMJ->setVisible(false);

			auto myInfo=MyInfo::create();
			this->removeChildByTag(CURENT_UI_LAYER,true);
			this->addChild(myInfo,4,CURENT_UI_LAYER);
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

void MyInfoScene::buttonSystemNotice(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curButton=(Button*)pSender;
			curButton->setTouchEnabled(false);
			curButton->setHighlighted(false);
			auto curMoji=this->getChildByTag(SYSTEM_MES_MOJI);
			curMoji->setVisible(true);

			auto anOtherBut=(Button*)this->getChildByTag(MY_MESSAGE_BUTT);
			anOtherBut->setTouchEnabled(true);
			anOtherBut->setHighlighted(true);
			auto anOtherMJ=this->getChildByTag(MY_MESSAGE_MOJI);
			anOtherMJ->setVisible(false);

			auto myInfo=SystemNotice::create();
			this->removeChildByTag(CURENT_UI_LAYER,true);
			this->addChild(myInfo,4,CURENT_UI_LAYER);
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