#include "VersionUpgrade.h"
#include "HelloWorldScene.h"

VersionUpgrade::VersionUpgrade(std::string curEdition,std::string newestEdition)
{
	curGameEdition=curEdition;
	targetEditon=newestEdition;
	smallUpgrade=true;

	std::string tempStrCur[4]={};
	std::string tempStrNew[4]={};
	for(int a=0;a<4;a++)
	{
		tempStrCur[a]=curEdition.substr(0+a*2,1);
		tempStrNew[a]=newestEdition.substr(0+a*2,1);
	}
	if(tempStrNew[0]>tempStrCur[0]||tempStrNew[1]>tempStrCur[1]||tempStrNew[2]>tempStrCur[2])
		smallUpgrade=false;
	else
		smallUpgrade=true;
	init();
}
VersionUpgrade::~VersionUpgrade(void)
{
}
void VersionUpgrade::upCancelCallBack(cocos2d::Ref* pSender,ui::Widget::TouchEventType type)
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
			auto startLayer=HelloWorld::create();
			scene->addChild(startLayer,1);
			Director::getInstance()->replaceScene(scene);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void VersionUpgrade::downDataUpdate(float delta)
{
	srand(time(0));
	auto progressBar=(ProgressTimer*)this->getChildByTag(DOWN_BAR);//progress bar

	auto downSize=(LabelTTF*)this->getChildByTag(HAVE_DOWN_SIZE);//have downed file size 
	auto downUnit=(LabelTTF*)this->getChildByTag(HAVE_DOWN_UNIT);//right to downSize

	auto speedLabel=(LabelTTF*)this->getChildByTag(DOWN_SPEED_KB);//download speed
	auto downdPersent=(LabelTTF*)this->getChildByTag(DOWN_PERSETTAGE);//persent for progress bar

	downSpeed=rand()%2048;
	haveDownSize+=((float)downSpeed/1024);
	float persentCount=haveDownSize/targetFileSize*100;

	if(haveDownSize<targetFileSize)
	{
		char tempNumber[20];

		sprintf(tempNumber,"下载速度: %d ",downSpeed);
		speedLabel->setString(tempNumber);

		sprintf(tempNumber,"文件大小: %.1f ",haveDownSize);
		downSize->setString(tempNumber);
		downUnit->setPosition(Vec2(downSize->getPosition().x+downSize->getContentSize().width,downSize->getPosition().y));

		sprintf(tempNumber,"%.1f",persentCount);
		downdPersent->setString(tempNumber);

		auto persentTo=ProgressTo::create(1,persentCount);
		progressBar->runAction(persentTo);
	}
	else
	{
		haveDownSize=targetFileSize;
		persentCount=100;
		this->unschedule(schedule_selector(VersionUpgrade::downDataUpdate));

		auto delayFunc=CallFunc::create([=](){
			char tempNumber[20];

			speedLabel->setString("下载速度: 0 ");
			sprintf(tempNumber,"文件大小: %.1f ",targetFileSize);
			downSize->setString(tempNumber);
			downUnit->setPosition(Vec2(downSize->getPosition().x+downSize->getContentSize().width,downSize->getPosition().y));

			downdPersent->setString("100");
		});

		auto progressAction=CallFunc::create([=](){
			auto persentTo=ProgressTo::create(1,persentCount);
			progressBar->runAction(persentTo);
		});

		auto curFunc=CallFunc::create([=](){
			auto scene=Scene::create();
			auto startLayer=HelloWorld::create();
			scene->addChild(startLayer,1);
			Director::getInstance()->replaceScene(scene);
		});

		this->runAction(Sequence::create(progressAction,delayFunc,curFunc,NULL));
	}
}
void VersionUpgrade::upEnsureCallBack(cocos2d::Ref* pSender,ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			for(int a=0;a<4;a++)
			{
				if(this->getChildByTag(SMALL_MESSAGE+a))
					this->getChildByTag(SMALL_MESSAGE+a)->setVisible(false);
			}

			downSpeed=0;
			targetFileSize=21.5;
			haveDownSize=0;
			char tempCount[10];

			auto progressBKG=Sprite::createWithSpriteFrameName("systemprompt-schedule.png");
			progressBKG->setAnchorPoint(Vec2(0.5,0.5));
			progressBKG->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-50));
			this->addChild(progressBKG,2);
			auto BKG_POS=progressBKG->getPosition();
			auto BKG_SIZE=progressBKG->getTextureRect().size;

			auto progress=Sprite::createWithSpriteFrameName("systemprompt-schedule2.png");
			auto progrssBar=ProgressTimer::create(progress);
			progrssBar->setType(ProgressTimer::Type::BAR);
			progrssBar->setBarChangeRate(Point(1,0));
			progrssBar->setMidpoint(Point(0,0));
			progrssBar->setPercentage(0.0f);
			progrssBar->setAnchorPoint(Vec2(0.5,0.5));
			progrssBar->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-50));
			this->addChild(progrssBar,3,DOWN_BAR); 

			auto fileSizeFont=LabelTTF::create("文件大小: 0 ","Arial",20);
			fileSizeFont->setAnchorPoint(Vec2(0,0));
			fileSizeFont->setPosition(Vec2(BKG_POS.x-BKG_SIZE.width/2,BKG_POS.y+BKG_SIZE.height/2+10));
			this->addChild(fileSizeFont,2,HAVE_DOWN_SIZE);

			sprintf(tempCount,"M/%.1fM",targetFileSize);
			auto fileSize=LabelTTF::create(tempCount,"Arial",20);
			fileSize->setAnchorPoint(Vec2(0,0));
			fileSize->setPosition(Vec2(fileSizeFont->getPosition().x+fileSizeFont->getContentSize().width+3,BKG_POS.y+BKG_SIZE.height/2+10));
			this->addChild(fileSize,2,HAVE_DOWN_UNIT);

			auto downUnit=LabelTTF::create("Kb/s","Arial",20);
			downUnit->setAnchorPoint(Vec2(1,0));
			downUnit->setPosition(Vec2(BKG_POS.x+BKG_SIZE.width/2,BKG_POS.y+BKG_SIZE.height/2));
			this->addChild(downUnit,2);

			auto downSpeedFont=LabelTTF::create("下载速度: 0 ","Arial",20);
			downSpeedFont->setAnchorPoint(Vec2(1,0));
			downSpeedFont->setPosition(Vec2(downUnit->getPosition().x-downUnit->getContentSize().width-3,downUnit->getPosition().y));
			this->addChild(downSpeedFont,2,DOWN_SPEED_KB);

			auto persentSign=LabelTTF::create("%","Arial",20);
			persentSign->setAnchorPoint(Vec2(1,0.5));
			persentSign->setPosition(Vec2(BKG_POS.x+BKG_SIZE.width/2,BKG_POS.y));
			this->addChild(persentSign,3);

			auto progressPersent=LabelTTF::create("0","Arial",20);
			progressPersent->setAnchorPoint(Vec2(1,0.5));
			progressPersent->setPosition(persentSign->getPosition().x-persentSign->getContentSize().width-3,BKG_POS.y);
			this->addChild(progressPersent,3,DOWN_PERSETTAGE);

			auto downloadCancel=Button::create("systemprompt-cancel.png","systemprompt-cancel2.png","systemprompt-cancel2.png",UI_TEX_TYPE_PLIST);
			downloadCancel->addTouchEventListener(CC_CALLBACK_2(VersionUpgrade::downCancelCallBack,this));
			downloadCancel->setAnchorPoint(Vec2(0.5,0.5));
			downloadCancel->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-120));
			this->addChild(downloadCancel,2);

			schedule(schedule_selector(VersionUpgrade::downDataUpdate),1.0f);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void VersionUpgrade::downCancelCallBack(cocos2d::Ref* pSender,ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			if(smallUpgrade)
			{
				auto scene=Scene::create();
				auto startLayer=HelloWorld::create();
				scene->addChild(startLayer,1);
				Director::getInstance()->replaceScene(scene);
			}
			else
				Director::getInstance()->end();//缓存清理？
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
bool VersionUpgrade::init()
{
	if(!Layer::init())
	{
		return false;
	}

	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("systemprompt.plist");

	auto BKG=Sprite::create("loadwin.png");
	BKG->setAnchorPoint(Vec2(0.5,0.5));
	BKG->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	BKG->setScale(1.189);
	this->addChild(BKG,0);

	auto noticeBkg=Sprite::createWithSpriteFrameName("systemprompt-sj.png");
	noticeBkg->setAnchorPoint(Vec2(0.5,0.5));
	noticeBkg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(noticeBkg,1);

	char promptMessage[100];
	std::string messageHead;
	std::string messageEnd;
	sprintf(promptMessage,"检测到您当前版本号为%s",curGameEdition.c_str());//sprintf 参数中如果有两个%s ERROR
	messageHead=promptMessage;
	sprintf(promptMessage,"低于最新版本%s,是否立即更新？",targetEditon.c_str());
	messageEnd=promptMessage;
	std::string allMessageStr=messageHead+messageEnd;
	auto noticeFont=LabelTTF::create(allMessageStr,"Arial",30);
	noticeFont->setAnchorPoint(Vec2(0.5,0.5));
	noticeFont->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2+40));
	this->addChild(noticeFont,2,SMALL_MESSAGE);

	if(!smallUpgrade)
	{
		auto bigNotice=LabelTTF::create("当前版本存在重要更新，请立即更新。","Arial",20);
		bigNotice->setColor(ccRED);
		bigNotice->setAnchorPoint(Vec2(0.5,0.5));
		bigNotice->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-40));
		this->addChild(bigNotice,2,BIG_MESSAGE);
	}
	else
	{
		auto upCancel=Button::create("systemprompt-no.png","systemprompt-no2.png","systemprompt-no2.png",UI_TEX_TYPE_PLIST);
		upCancel->addTouchEventListener(CC_CALLBACK_2(VersionUpgrade::upCancelCallBack,this));
		upCancel->setAnchorPoint(Vec2(0.5,0.5));
		upCancel->setPosition(Vec2(origin.x+visibleSize.width/2-130,origin.y+visibleSize.height/2-120));
		this->addChild(upCancel,2,UP_CANCEL_BUT);
	}

	auto upEnsure=Button::create("systemprompt-yes.png","systemprompt--yes2.png","systemprompt--yes2.png",UI_TEX_TYPE_PLIST);
	upEnsure->addTouchEventListener(CC_CALLBACK_2(VersionUpgrade::upEnsureCallBack,this));
	upEnsure->setAnchorPoint(Vec2(0.5,0.5));
	if(smallUpgrade)
		upEnsure->setPosition(Vec2(origin.x+visibleSize.width/2+130,origin.y+visibleSize.height/2-120));
	else
		upEnsure->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-120));
	this->addChild(upEnsure,2,UP_ENSURE_BUT);

	return true;
}