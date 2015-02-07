#include "MyBaseInformation.h"


MyBaseInformation::MyBaseInformation(void)
{
}


MyBaseInformation::~MyBaseInformation(void)
{
}

bool MyBaseInformation::init()
{
	if(!Layer::init())
	{
		return false;
	}
	this->sex=boy;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	this->setContentSize(Size(origin.x+visibleSize.width,origin.y+visibleSize.height*0.7877));
	
	auto perInfoBg=Sprite::create("PerInformation.png");
	perInfoBg->setAnchorPoint(Vec2(0,0));
	perInfoBg->setPosition(Vec2(origin.x+visibleSize.width*0.3087,origin.y+visibleSize.height*0.85));
	perInfoBg->setRotation(90);
	this->addChild(perInfoBg,1);

	auto playerPhoto=Sprite::createWithSpriteFrameName("touxiangkuang.png");
	playerPhoto->setAnchorPoint(Vec2(0,0));
	playerPhoto->setPosition(Vec2(origin.x+visibleSize.width*0.0435,origin.y+visibleSize.height*0.474));
	this->addChild(playerPhoto,1);

	auto buttonChangePhoto=Button::create("xiugaitouxiang.png","xiugaitouxiang1.png","xiugaitouxiang1.png",TextureResType::PLIST);
	buttonChangePhoto->setAnchorPoint(Vec2(0,0));
	buttonChangePhoto->setPosition(Vec2(origin.x+visibleSize.width*0.0607,origin.y+visibleSize.height*0.359));
	buttonChangePhoto->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::onButtonChangePhoto,this));
	this->addChild(buttonChangePhoto,1);
	
	auto labelOfID=LabelTTF::create("ID:","Arial",30);
	labelOfID->setAnchorPoint(Vec2(0,0));
	labelOfID->setPosition(Vec2(origin.x+visibleSize.width*0.3678,origin.y+visibleSize.height*0.7304));
	this->addChild(labelOfID,1);
	
	auto labelOfAccount=LabelTTF::create("账户:","Arial",30);
	labelOfAccount->setAnchorPoint(Vec2(0,0));
	labelOfAccount->setPosition(Vec2(origin.x+visibleSize.width*0.596,origin.y+visibleSize.height*0.73));
	this->addChild(labelOfAccount,1);

	auto labelOfName=LabelTTF::create("昵称:","Arial",30);
	labelOfName->setAnchorPoint(Vec2(0,0));
	labelOfName->setPosition(Vec2(origin.x+visibleSize.width*0.3678,origin.y+visibleSize.height*0.6145));
	this->addChild(labelOfName,1);
	
	auto labelOfSex=LabelTTF::create("性别:","Arial",30);
	labelOfSex->setAnchorPoint(Vec2(0,0));
	labelOfSex->setPosition(Vec2(origin.x+visibleSize.width*0.3678,origin.y+visibleSize.height*0.499));
	this->addChild(labelOfSex,1);

	auto labelOfWinRate=LabelTTF::create("胜率:","Arial",30);
	labelOfWinRate->setAnchorPoint(Vec2(0,0));
	labelOfWinRate->setPosition(Vec2(origin.x+visibleSize.width*0.3678,origin.y+visibleSize.height*0.3835));
	this->addChild(labelOfWinRate,1);

	auto labelOfHistory=LabelTTF::create("历史最大番次数:","Arial",30);
	labelOfHistory->setAnchorPoint(Vec2(0,0));
	labelOfHistory->setPosition(Vec2(origin.x+visibleSize.width*0.3678,origin.y+visibleSize.height*0.2821));
	this->addChild(labelOfHistory,1);

	auto buttonChangeAccount=Button::create("qiehuanzhanghao2.png","qiehuanzhanghao1.png","qiehuanzhanghao1.png",TextureResType::PLIST);
	buttonChangeAccount->setAnchorPoint(Vec2(0,0));
	buttonChangeAccount->setPosition(Vec2(origin.x+visibleSize.width*0.357,origin.y+visibleSize.height*0.13));
	buttonChangeAccount->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::onButtonChangePhoto,this));
	this->addChild(buttonChangeAccount,1);

	auto buttonChangeAccountLevel=Button::create("zhanghaoshengji2.png","zhanghaoshengji1.png","zhanghaoshengji1.png",TextureResType::PLIST);
	buttonChangeAccountLevel->setAnchorPoint(Vec2(0,0));
	buttonChangeAccountLevel->setPosition(Vec2(origin.x+visibleSize.width*0.5599,origin.y+visibleSize.height*0.13));
	buttonChangeAccountLevel->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::onButtonChangePhoto,this));
	this->addChild(buttonChangeAccountLevel,1);

	auto buttonManageCode=Button::create("mimaguanli2.png","mimaguanli1.png","mimaguanli1.png",TextureResType::PLIST);
	buttonManageCode->setAnchorPoint(Vec2(0,0));
	buttonManageCode->setPosition(Vec2(origin.x+visibleSize.width*0.7635,origin.y+visibleSize.height*0.13));
	buttonManageCode->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::onButtonChangePhoto,this));
	this->addChild(buttonManageCode,1);

	for(int i=0;i<4;i++)
	{
		auto line=Sprite::createWithSpriteFrameName("fengexian1.png");
		line->setAnchorPoint(Vec2(0,0));
		line->setPosition(Vec2(origin.x+visibleSize.width*0.3653,origin.y+visibleSize.height*(0.3548+0.113*i)));
		this->addChild(line,1);
	}

	auto buttonPen=Button::create("bi1.png","bi2.png","bi2.png",TextureResType::PLIST);
	buttonPen->setAnchorPoint(Vec2(0,0));
	buttonPen->setPosition(Vec2(origin.x+visibleSize.width*0.773,origin.y+visibleSize.height*0.593));
	buttonPen->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::onButtonChangePhoto,this));
	this->addChild(buttonPen,1);
	
	auto ItemOfPetName=Sprite::createWithSpriteFrameName("nichengkuang.png");
	ItemOfPetName->setAnchorPoint(Vec2(0,0));
	ItemOfPetName->setPosition(Vec2(origin.x+visibleSize.width*0.439,origin.y+visibleSize.height*0.593));
	this->addChild(ItemOfPetName,1);

	auto manLogo=Sprite::createWithSpriteFrameName("nan.png");
	manLogo->setAnchorPoint(Vec2(0,0));
	manLogo->setPosition(Vec2(origin.x+visibleSize.width*0.539,origin.y+visibleSize.height*0.4986));
	this->addChild(manLogo,1);

	auto buttonBoy=ui::Button::create("xingbie1.png","xingbie1.png","xingbie1.png",TextureResType::PLIST);
	buttonBoy->setAnchorPoint(Vec2(0,0));
	buttonBoy->setPosition(Vec2(origin.x+visibleSize.width*0.457,origin.y+visibleSize.height*0.4888));
	buttonBoy->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::onButtonBoy,this));
	this->addChild(buttonBoy,1);

	auto chooseedFlag1=Sprite::createWithSpriteFrameName("xingbie.png");
	chooseedFlag1->setAnchorPoint(Vec2(0,0));
	chooseedFlag1->setPosition(Vec2(origin.x+visibleSize.width*0.4655,origin.y+visibleSize.height*0.501));
	this->addChild(chooseedFlag1,2,1);
	if(sex==boy)
	{
		chooseedFlag1->setVisible(true);
	}
	else
	{
		chooseedFlag1->setVisible(false);
	}

	auto womanLogo=Sprite::createWithSpriteFrameName("nv.png");
	womanLogo->setAnchorPoint(Vec2(0,0));
	womanLogo->setPosition(Vec2(origin.x+visibleSize.width*0.7758,origin.y+visibleSize.height*0.4986));
	this->addChild(womanLogo,1);

	auto buttonGirl=ui::Button::create("xingbie1.png","xingbie1.png","xingbie1.png",TextureResType::PLIST);
	buttonGirl->setAnchorPoint(Vec2(0,0));
	buttonGirl->setPosition(Vec2(origin.x+visibleSize.width*0.6937,origin.y+visibleSize.height*0.4888));
	buttonGirl->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::onButtonGirl,this));
	this->addChild(buttonGirl,1);
	
	auto chooseedFlag2=Sprite::createWithSpriteFrameName("xingbie.png");
	chooseedFlag2->setAnchorPoint(Vec2(0,0));
	chooseedFlag2->setPosition(Vec2(origin.x+visibleSize.width*0.702,origin.y+visibleSize.height*0.501));
	this->addChild(chooseedFlag2,2,2);
	if(sex==girl)
	{
		chooseedFlag2->setVisible(true);
	}
	else
	{
		chooseedFlag2->setVisible(false);
	}

	auto goodNameBar=Sprite::createWithSpriteFrameName("shang.png");
	goodNameBar->setAnchorPoint(Vec2(0,0));
	goodNameBar->setPosition(Vec2(origin.x+visibleSize.width*0.04,origin.y+visibleSize.height*0.268));
	this->addChild(goodNameBar,1);
	
	auto goodName=Sprite::createWithSpriteFrameName("812.png");
	goodName->setAnchorPoint(Vec2(0,0));
	goodName->setPosition(Vec2(origin.x+visibleSize.width*0.04,origin.y+visibleSize.height*0.264));
	this->addChild(goodName,2);

	auto competeBar=Sprite::createWithSpriteFrameName("middle.png");
	competeBar->setAnchorPoint(Vec2(0,0));
	competeBar->setPosition(Vec2(origin.x+visibleSize.width*0.04,origin.y+visibleSize.height*0.215));
	this->addChild(competeBar,1);
	
	auto compete=Sprite::createWithSpriteFrameName("jinbi.png");
	compete->setAnchorPoint(Vec2(0,0));
	compete->setPosition(Vec2(origin.x+visibleSize.width*0.0398,origin.y+visibleSize.height*0.214));
	compete->setScale(0.5f);
	this->addChild(compete,2);

	auto buttonPlus=Button::create("jiahao.png","jiahao5.png","jiahao5.png",TextureResType::PLIST);
	buttonPlus->setAnchorPoint(Vec2(0,0));
	buttonPlus->setPosition(Vec2(origin.x+visibleSize.width*0.196,origin.y+visibleSize.height*0.215));
	buttonPlus->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::onButtonChangePhoto,this));
	this->addChild(buttonPlus,2);

	auto TaskBar=Sprite::createWithSpriteFrameName("xia.png");
	TaskBar->setAnchorPoint(Vec2(0,0));
	TaskBar->setPosition(Vec2(origin.x+visibleSize.width*0.04,origin.y+visibleSize.height*0.166));
	this->addChild(TaskBar,1);

	auto task=Sprite::createWithSpriteFrameName("juan.png");
	task->setAnchorPoint(Vec2(0,0));
	task->setPosition(Vec2(origin.x+visibleSize.width*0.045,origin.y+visibleSize.height*0.1676));
	this->addChild(task,2);

	auto LevelProgressBg=Sprite::createWithSpriteFrameName("jingyantiao.png");
	LevelProgressBg->setAnchorPoint(Vec2(0,0));
	LevelProgressBg->setPosition(Vec2(origin.x+visibleSize.width*0.0418,origin.y+visibleSize.height*0.09));
	this->addChild(LevelProgressBg,1);

	auto LevelProgress=Sprite::createWithSpriteFrameName("jingyantiao1.png");
	CCProgressTimer* pt = CCProgressTimer::create(LevelProgress);  
    pt->setMidpoint(ccp(0, 0));  
	pt->setType(kCCProgressTimerTypeBar);
    pt->setBarChangeRate(ccp(1,0));  
    pt->setPercentage(100.0f);  
	pt->setAnchorPoint(Vec2(0,0));
    pt->setPosition(Vec2(origin.x+visibleSize.width*0.0525,origin.y+visibleSize.height*0.111));
    this->addChild(pt,2);  

	auto labelOfLevel=LabelTTF::create("Lv.1","Arial",20);
	labelOfLevel->setAnchorPoint(Vec2(0,0));
	labelOfLevel->setPosition(Vec2(origin.x+visibleSize.width*0.2069,origin.y+visibleSize.height*0.109));
	this->addChild(labelOfLevel,4);

	return true;
}

void MyBaseInformation::menuCloseCallback(Ref* pSender)
{

}

void MyBaseInformation::onButtonChangePhoto(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}

void MyBaseInformation::onButtonBoy(Ref* pSender,Widget::TouchEventType type)
{
	auto choosedBoyFlag=this->getChildByTag(1);
	auto choosedGirlFlag=this->getChildByTag(2);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		choosedBoyFlag->setVisible(true);
		choosedGirlFlag->setVisible(false);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}

void MyBaseInformation::onButtonGirl(Ref* pSender,Widget::TouchEventType type)
{
	auto choosedBoyFlag=this->getChildByTag(1);
	auto choosedGirlFlag=this->getChildByTag(2);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		choosedBoyFlag->setVisible(false);
		choosedGirlFlag->setVisible(true);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}
