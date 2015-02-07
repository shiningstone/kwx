#include "SetScene.h"
#include "ChangeScene.h"
#include "EnterRoomScene.h"
#include "SetLayer.h"

SetScene::SetScene(void)
{
}


SetScene::~SetScene(void)
{
}

bool SetScene::init()
{
	if(!Scene::init())
	{
		return false;
	}
	auto visiableSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	float s_scale=1.189f;

	auto bg=Sprite::create("602.png");
	bg->setPosition(Vec2(origin.x+visiableSize.width/2,origin.y+visiableSize.height/2));
	bg->setScaleX(s_scale);
	bg->setScaleY(s_scale);
	this->addChild(bg,0);

	auto paddle=LayerColor::create(Color4B(47,86,110,100));
	auto sizeOfPaddle=paddle->getContentSize();
	paddle->setContentSize(Size(visiableSize.width,visiableSize.height*564/716));
	paddle->setPosition(Vec2(origin.x,origin.y+visiableSize.height*0.0656));
	this->addChild(paddle,1);

	auto selectEffectOfInfo=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffectOfInfo->setAnchorPoint(Vec2(0.5,0.5));
	selectEffectOfInfo->setPosition(origin.x+0.5*visiableSize.width,origin.y+visiableSize.height*0.93);
	this->addChild(selectEffectOfInfo,1,1);
	selectEffectOfInfo->setVisible(true);

	auto buttonItemBack1 = Sprite::create("12.png"); 
	auto buttonItemBack2 = Sprite::create("12.png"); 
	auto itemOfMenuBack = MenuItemSprite::create(buttonItemBack1,buttonItemBack2,CC_CALLBACK_1(SetScene::onButtonBack, this));
	auto sizeOfBack=itemOfMenuBack->getContentSize();
	auto buttonBack = Menu::create(itemOfMenuBack,NULL);
	buttonBack->setPosition(origin.x+((sizeOfBack.width/2+1120)/1218)*visiableSize.width,origin.y+((716-12-sizeOfBack.height/2)/716)*visiableSize.height);
	this->addChild(buttonBack, 3);

	auto titleOfActivity=Sprite::createWithSpriteFrameName("shezhi.png");
	titleOfActivity->setAnchorPoint(Vec2(0.5,0.5));
	titleOfActivity->setPosition(origin.x+0.5*visiableSize.width,origin.y+visiableSize.height*0.93);
	this->addChild(titleOfActivity,3);

	auto flower=Sprite::createWithSpriteFrameName("meihua.png");
	auto sizeOfFlower=flower->getContentSize();
	flower->setAnchorPoint(Vec2(1,0));
	flower->setPosition(origin.x+visiableSize.width*1.0095,origin.y-visiableSize.height*0.028);
	this->addChild(flower,0);

	auto setLayer=SetLayer::create();
	this->addChild(setLayer,3);

	//auto tt=test::create();
	//this->addChild(tt,3);

	return true;
}

void SetScene::onButtonBack(Ref* pSender)
{
	Director::getInstance()->popScene();
}
