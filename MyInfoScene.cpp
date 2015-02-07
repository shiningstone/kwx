#include "MyInfoScene.h"
#include "EnterRoomScene.h"
#include "MyInfo.h"
#include "SystemNotice.h"


MyInfoScene::MyInfoScene(void)
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
	auto sizeOfEffect=selectEffectOfInfo->getContentSize();
	selectEffectOfInfo->setPosition(origin.x+((sizeOfEffect.width/2+300)/1218)*visiableSize.width,origin.y+((620+sizeOfEffect.height/2)/716)*visiableSize.height);
	this->addChild(selectEffectOfInfo,1,1);
	selectEffectOfInfo->setVisible(true);

	auto selectEffectOfCheatCode=Sprite::createWithSpriteFrameName("moji12.png");
	selectEffectOfCheatCode->setPosition(origin.x+((sizeOfEffect.width/2+710)/1218)*visiableSize.width,origin.y+((620+sizeOfEffect.height/2)/716)*visiableSize.height);
	this->addChild(selectEffectOfCheatCode,1,2);
	selectEffectOfCheatCode->setVisible(false);

	auto imageOfBaseInfo1 = Sprite::createWithSpriteFrameName("wodexinxi.png"); 
	auto imageOfBaseInfo2 = Sprite::createWithSpriteFrameName("wodexinxi2.png"); 
	auto itemOfMenuBaseInfo= MenuItemSprite::create(imageOfBaseInfo1,imageOfBaseInfo2,CC_CALLBACK_1(MyInfoScene::buttonMyInfo, this));
	auto sizeOfBaseInfo=itemOfMenuBaseInfo->getContentSize();
	auto ButtonBaseInfo = Menu::create(itemOfMenuBaseInfo,NULL);
	ButtonBaseInfo->setPosition(origin.x+((sizeOfBaseInfo.width/2+297)/1218)*visiableSize.width,origin.y+((716-20-sizeOfBaseInfo.height/2)/716)*visiableSize.height);
	this->addChild(ButtonBaseInfo, 3);

	auto imageSystemNotice1 = CCSprite::createWithSpriteFrameName("xitonggonggao.png"); 
	auto imageSystemNotice2 = CCSprite::createWithSpriteFrameName("xitonggonggao2.png"); 
	auto itemOfSystemNotice= MenuItemSprite::create(imageSystemNotice1,imageSystemNotice2,CC_CALLBACK_1(MyInfoScene::buttonSystemNotice, this));
	auto sizeOfSystemNotice=itemOfSystemNotice->getContentSize();
	auto buttonOfSystemNotice = Menu::create(itemOfSystemNotice,NULL);
	buttonOfSystemNotice->setPosition(origin.x+((708+sizeOfSystemNotice.width/2)/1218)*visiableSize.width,origin.y+((716-20-sizeOfBaseInfo.height/2)/716)*visiableSize.height);
	this->addChild(buttonOfSystemNotice, 3);

	auto buttonItemBack1 = Sprite::create("12.png"); 
	auto buttonItemBack2 = Sprite::create("12.png"); 
	auto itemOfMenuBack = MenuItemSprite::create(buttonItemBack1,buttonItemBack2,CC_CALLBACK_1(MyInfoScene::buttonBack, this));
	auto sizeOfBack=itemOfMenuBack->getContentSize();
	auto menu7 = Menu::create(itemOfMenuBack,NULL);
	menu7->setPosition(origin.x+((sizeOfBack.width/2+1120)/1218)*visiableSize.width,origin.y+((716-12-sizeOfBack.height/2)/716)*visiableSize.height);
	this->addChild(menu7, 3);

	auto flower=Sprite::createWithSpriteFrameName("meihua.png");
	auto sizeOfFlower=flower->getContentSize();
	flower->setAnchorPoint(Vec2(1,0));
	flower->setPosition(origin.x+visiableSize.width*1.0095,origin.y-visiableSize.height*0.028);
	this->addChild(flower,0);

	auto photoFrame=Sprite::create("huodongzhonxinkuang.png");
	photoFrame->setAnchorPoint(Vec2(0,0));
	photoFrame->setPosition(Vec2(origin.x+visiableSize.width*0.0082,origin.y+visiableSize.height*0.0698));
	this->addChild(photoFrame,2);

	auto myInfo=MyInfo::create();
	this->addChild(myInfo,4,3);

	return true;
}

void MyInfoScene::buttonBack(Ref* pSender)
{
    Director::getInstance()->popScene();
}

void MyInfoScene::buttonMyInfo(Ref* pSender)
{
	auto imageOfEffectInfo=this->getChildByTag(1);
	imageOfEffectInfo->setVisible(true);

	auto imageOfEffectXbox=this->getChildByTag(2);
	imageOfEffectXbox->setVisible(false);

	this->removeChildByTag(3);
	auto myInfo=MyInfo::create();
	this->addChild(myInfo,4,3);
	
}

void MyInfoScene::buttonSystemNotice(Ref* pSender)
{
	auto imageOfEffectInfo=this->getChildByTag(1);
	imageOfEffectInfo->setVisible(false);

	auto imageOfEffectXbox=this->getChildByTag(2);
	imageOfEffectXbox->setVisible(true);

	this->removeChildByTag(3);
	auto systemNotice=SystemNotice::create();
	this->addChild(systemNotice,4,3);
}