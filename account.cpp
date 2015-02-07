#include "account.h"
//#include "RobotScene.h"

account::account()
{
}

account::~account()
{
	
}

bool account::init()
{
	if(!Layer::init())
	{
		return false ;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();  
    cache->addSpriteFramesWithFile("huojiangjiesuan.plist", "huojiangjiesuan.png");
	cache->addSpriteFramesWithFile("race.plist","race.png");

	auto BackGround=Sprite::create("racetable.png");//结算背景
	BackGround->setScale(1.189);
	BackGround->setAnchorPoint(Vec2(0.5,0.5));
	BackGround->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2 ));
	this->addChild(BackGround,0);

	auto Striae1=Sprite::create("tiaotu4.png");//下条纹
	Striae1->setAnchorPoint(Vec2(0.5,0));
	Striae1->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+0));
	this->addChild(Striae1,1);

	auto Striae2=Sprite::create("tiaotu3.png");//上条纹
	Striae2->setAnchorPoint(Vec2(0.5,1));
	int i=Striae2->getTextureRect().size.height;
	Striae2->setScaleY(201.6/Striae2->getTextureRect().size.height);
	Striae2->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.718435));
	this->addChild(Striae2,1);

	auto zhuangPic=Sprite::create("tileImage/zhuang.png");
	zhuangPic->setAnchorPoint(Vec2(0.5,0.5));
	zhuangPic->setScale(0.8);
	zhuangPic->setPosition(Vec2(origin.x+visibleSize.width*156/1218,origin.y+visibleSize.height*575/716));
	this->addChild(zhuangPic,3);
	//auto playerBar=LayerColor::create();
	//playerBar->setContentSize(Size(visibleSize.width,201.6));
	//this->addChild(playerBar,2,4);
	//auto RobotBar1=LayerColor::create();
	//RobotBar1->setContentSize(Size(visibleSize.width,201.6));
	//this->addChild(RobotBar1,2,5);
	//auto RobotBar2=LayerColor::create();
	//RobotBar2->setContentSize(Size(visibleSize.width,201.6));
	//this->addChild(RobotBar2,2,6);
	auto PlaceOfPlayer0=LayerColor::create();
	PlaceOfPlayer0->setContentSize(Size(visibleSize.width,visibleSize.height*196.5/716));
	this->addChild(PlaceOfPlayer0,2,0);

	auto PlaceOfPlayer1=LayerColor::create();
	PlaceOfPlayer1->setContentSize(Size(visibleSize.width,visibleSize.height*196.5/716));
	this->addChild(PlaceOfPlayer1,2,1);

	auto PlaceOfPlayer2=LayerColor::create();
	PlaceOfPlayer2->setContentSize(Size(visibleSize.width,visibleSize.height*196.5/716));
	this->addChild(PlaceOfPlayer2,2,2);	
	//for(int i=0;i<3;i++)
	//{
	//	auto gold1=Sprite::createWithSpriteFrameName("jinbi-game.png");
	//	gold1->setAnchorPoint(Vec2(0.5,0.5));
	//	gold1->setScale(0.7);
	//	gold1->setPosition(Vec2(origin.x+visibleSize.width*0.02380952,origin.y+20));
	//	this->getChildByTag(4+i)->addChild(gold1,2);

	//	auto propertyOfIncrease=LabelAtlas::create("0","fonts/moneyMessage.png",28, 39, '0');
	//	propertyOfIncrease->setAnchorPoint(Vec2(0,0.5));
	//	propertyOfIncrease->setPosition(Vec2(origin.x+visibleSize.width*0.8+50,origin.y+30));
	//	this->getChildByTag(4+i)->addChild(propertyOfIncrease,2,2);
	//	propertyOfIncrease->setVisible(false);
	//}
	return true;
}