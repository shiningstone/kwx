#include "ChangeScene.h"
#include "BaseScene.h"
#include "MyInfoScene.h"
#include "ActivityCenter.h"
#include "TaskReward.h"
#include "SetScene.h"
#include "ShopScene.h"
#include "CommonField.h"
#include "CompetitionField.h"
#include "Wait.h"

ChangeScene::ChangeScene(void)
{
}


ChangeScene::~ChangeScene(void)
{
}

bool ChangeScene::init()
{
	if(!Scene::init())
	{
		return false;
	}
	auto visiableSize=Director::getInstance()->getVisibleSize();
	auto button1=MenuItemFont::create("ButtonToBaseInfo",CC_CALLBACK_1(ChangeScene::buttonToBaseInfo, this));
	auto button2=MenuItemFont::create("ButtonToMyInfo",CC_CALLBACK_1(ChangeScene::buttonToMyInfo, this));
	auto button3=MenuItemFont::create("ButtonToActivity",CC_CALLBACK_1(ChangeScene::buttonToActivity, this));
	auto button4=MenuItemFont::create("ButtonToTaskReward",CC_CALLBACK_1(ChangeScene::buttonToTaskReward, this));
	auto button5=MenuItemFont::create("ButtonToSet",CC_CALLBACK_1(ChangeScene::buttonToSet, this));
	auto button6=MenuItemFont::create("ButtonToShop",CC_CALLBACK_1(ChangeScene::buttonToShop, this));
	auto button7=MenuItemFont::create("ButtonToCommonField",CC_CALLBACK_1(ChangeScene::buttonToCommonField, this));
	auto button8=MenuItemFont::create("ButtonToCompetitionField",CC_CALLBACK_1(ChangeScene::buttonToCompetition,this));
	auto button9=MenuItemFont::create("BttonToWait",CC_CALLBACK_1(ChangeScene::buttonToWait,this));
	auto button=Menu::create(button1,button2,button3,button4,button5,button6,button7,button8,button9,NULL);
	button->setPosition(visiableSize.width/2,visiableSize.height/2);
	button->alignItemsVertically();  
	this->addChild(button);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("jibenxinximian.plist"); 
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("chongfuziyuan.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("chongfuziyuan2.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("shezhijiemian.plist");
	CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile("changguichangbupiao.plist");
	CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile("changguichangdaipiao.plist");
	CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile("bisaichang.plist");
	CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile("bisaidengdai.plist");
	CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile("bisaibaoming.plist");

	return true;
}

void ChangeScene::buttonToMyInfo(Ref* pSender)
{
	auto MyInfo=MyInfoScene::create();
	Director::getInstance()->replaceScene(MyInfo);
}

void ChangeScene::buttonToBaseInfo(Ref* pSender)
{
	auto BaseInfo=BaseScene::create();
	Director::getInstance()->replaceScene(BaseInfo);
}

void ChangeScene::buttonToActivity(Ref* pSender)
{
	auto activityScene=ActivityCenter::create();
	Director::getInstance()->replaceScene(activityScene);
}

void ChangeScene::buttonToTaskReward(Ref* pSender)
{
	auto taskReward=TaskReward::create();
	Director::getInstance()->replaceScene(taskReward);
}

void ChangeScene::buttonToSet(Ref* pSender)
{
	auto setScene=SetScene::create();
	Director::getInstance()->replaceScene(setScene);
}

void ChangeScene::buttonToShop(Ref* pSender)
{
	auto shopScene=ShopScene::create();
	Director::getInstance()->replaceScene(shopScene);
}

void ChangeScene::buttonToCommonField(Ref* pSender)
{
	auto commonField=CommonField::create();
	Director::getInstance()->replaceScene(commonField);
}

void ChangeScene::buttonToCompetition(Ref* pSender)
{
	auto competitionField=CompetitionField::create();
	Director::getInstance()->replaceScene(competitionField);
}

void ChangeScene::buttonToWait(Ref* pSender)
{
	auto waitScene=Wait::createScene();
	Director::getInstance()->replaceScene(waitScene);
}