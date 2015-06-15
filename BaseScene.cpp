#include "EnterRoomScene.h"
#include "BaseScene.h"
#include "MyBaseInformation.h"
#include "MyXbox.h"


BaseScene::BaseScene(void)
{
}
BaseScene::~BaseScene(void)
{
}
bool BaseScene::init()
{
	if(!Scene::init())
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
	float s_scale=1.189f;
   
	auto bg=Sprite::create("602.png");
	bg->setScaleX(s_scale);
	bg->setScaleY(s_scale);
	bg->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.5));
	this->addChild(bg,0);

	auto paddle=LayerColor::create(Color4B(47,86,110,100));
	paddle->setContentSize(Size(visibleSize.width,visibleSize.height*0.7877));
	paddle->setPosition(Vec2(origin.x,origin.y+visibleSize.height*0.0656));
	this->addChild(paddle,1);

	auto selectEffectOfInfo=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffectOfInfo->setAnchorPoint(Vec2(0,0));
	selectEffectOfInfo->setPosition(Vec2(origin.x+visibleSize.width*0.264,origin.y+visibleSize.height*0.8659));
	this->addChild(selectEffectOfInfo,1,MY_INFO_MOJI);
	selectEffectOfInfo->setVisible(true);

	auto selectEffectOfCheatCode=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffectOfCheatCode->setAnchorPoint(Vec2(0,0));
	selectEffectOfCheatCode->setPosition(Vec2(origin.x+visibleSize.width*0.5629,origin.y+visibleSize.height*0.8659));
	this->addChild(selectEffectOfCheatCode,1,MY_BAG_MOJI);
	selectEffectOfCheatCode->setVisible(false);
	
	auto backButton=Button::create("12.png","12.png","12.png",TextureResType::LOCAL);
	backButton->setAnchorPoint(Vec2(1,1));
	backButton->setPosition(Vec2(origin.x+visibleSize.width,origin.y+visibleSize.height*0.98));
	backButton->addTouchEventListener(CC_CALLBACK_2(BaseScene::buttonBack,this));
	this->addChild(backButton,1);

	auto buttonBaseInfo=Button::create("jibenxinxi.png","jibenxinxi1.png","jibenxinxi.png",TextureResType::PLIST);
	buttonBaseInfo->setAnchorPoint(Vec2(0,0));
	buttonBaseInfo->setPosition(Vec2(origin.x+visibleSize.width*0.268,origin.y+visibleSize.height*0.9));
	buttonBaseInfo->addTouchEventListener(CC_CALLBACK_2(BaseScene::buttonMyInfo, this));
	this->addChild(buttonBaseInfo,2,MY_INFO_BUTON);
	buttonBaseInfo->setTouchEnabled(false);
	buttonBaseInfo->setHighlighted(true);

	auto buttonMyXbox=Button::create("wodebeibao2.png","wodebeibao.png","wodebeibao2.png",TextureResType::PLIST);
	buttonMyXbox->setAnchorPoint(Vec2(0,0));
	buttonMyXbox->setPosition(Vec2(origin.x+visibleSize.width*0.5669,origin.y+visibleSize.height*0.9));
	buttonMyXbox->addTouchEventListener(CC_CALLBACK_2(BaseScene::buttonMyXbox, this));
	this->addChild(buttonMyXbox,2,MY_BAG_BUTTON);

	auto infoLayer=MyBaseInformation::create();
	this->addChild(infoLayer,3,CUR_HAVED_LAYER);
	
	return true;
}

void BaseScene::buttonBack(Ref* pSender,Widget::TouchEventType type)
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
	default:
		break;
	}
}

void BaseScene::buttonMyInfo(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		{
			auto infoMoJi=this->getChildByTag(MY_INFO_MOJI);
			auto infoBut=(Button*)this->getChildByTag(MY_INFO_BUTON);
			auto BagMoJi=this->getChildByTag(MY_BAG_MOJI);
			auto BagBut=(Button*)this->getChildByTag(MY_BAG_BUTTON);
			infoBut->setTouchEnabled(false);
			auto infoLayer=MyBaseInformation::create();
			infoMoJi->setVisible(true);
			BagMoJi->setVisible(false);
			infoBut->setHighlighted(true);
			BagBut->setTouchEnabled(true);
			BagBut->setHighlighted(false);
			this->removeChildByTag(CUR_HAVED_LAYER,true);
			this->addChild(infoLayer,3,CUR_HAVED_LAYER);

		}
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void BaseScene::buttonMyXbox(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		{
			auto imageOfEffectInfo=this->getChildByTag(MY_INFO_MOJI);
			auto imageOfEffectXbox=this->getChildByTag(MY_BAG_MOJI);
			auto buttonInfo=(Button*)this->getChildByTag(MY_INFO_BUTON);
			auto buttonXbox=(Button*)this->getChildByTag(MY_BAG_BUTTON);
			buttonXbox->setTouchEnabled(false);
			auto xBox=MyXbox::create();
			imageOfEffectInfo->setVisible(false);
			imageOfEffectXbox->setVisible(true);
			buttonInfo->setTouchEnabled(true);
			buttonInfo->setHighlighted(false);
			buttonXbox->setHighlighted(true);
			this->removeChildByTag(CUR_HAVED_LAYER,true);
			this->addChild(xBox,3,CUR_HAVED_LAYER);
		}
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}