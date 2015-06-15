#include "MyBaseInformation.h"
#include "ShopScene.h"

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
	visibleSize = Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();
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

	auto LabelForID=LabelTTF::create("12345678","Arial",30);
	LabelForID->setAnchorPoint(Vec2(0,0));
	LabelForID->setPosition(Vec2(origin.x+visibleSize.width*0.3678+labelOfID->getContentSize().width+10,origin.y+visibleSize.height*0.7304));
	this->addChild(LabelForID,1,MY_BASEINFO_ID);
	
	auto labelOfAccount=LabelTTF::create("账户:","Arial",30);
	labelOfAccount->setAnchorPoint(Vec2(0,0));
	labelOfAccount->setPosition(Vec2(origin.x+visibleSize.width*0.596,origin.y+visibleSize.height*0.73));
	this->addChild(labelOfAccount,1);

	auto LabelForAccount=LabelTTF::create("934830895@qq.com","Arial",30);
	LabelForAccount->setAnchorPoint(Vec2(0,0));
	LabelForAccount->setPosition(Vec2(origin.x+visibleSize.width*0.596+labelOfAccount->getContentSize().width+10,origin.y+visibleSize.height*0.73));
	this->addChild(LabelForAccount,1,MY_ACCOUNT_LABEL);

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

	auto WinRate=LabelTTF::create("100%","Arial",30);
	WinRate->setAnchorPoint(Vec2(0,0));
	WinRate->setPosition(Vec2(labelOfWinRate->getPosition().x+labelOfWinRate->getContentSize().width+20,origin.y+visibleSize.height*0.3835));
	this->addChild(WinRate,1,WIN_RATE);

	auto labelOfHistory=LabelTTF::create("历史最大番次数:","Arial",30);
	labelOfHistory->setAnchorPoint(Vec2(0,0));
	labelOfHistory->setPosition(Vec2(origin.x+visibleSize.width*0.3678,origin.y+visibleSize.height*0.2821));
	this->addChild(labelOfHistory,1);

	auto MaxHistory=LabelTTF::create("128","Arial",30);
	MaxHistory->setAnchorPoint(Vec2(0,0));
	MaxHistory->setPosition(Vec2(labelOfHistory->getPosition().x+labelOfHistory->getContentSize().width+20,origin.y+visibleSize.height*0.2821));
	this->addChild(MaxHistory,1,MAX_HISTORY);

	auto buttonChangeAccount=Button::create("qiehuanzhanghao2.png","qiehuanzhanghao1.png","qiehuanzhanghao1.png",TextureResType::PLIST);
	buttonChangeAccount->setAnchorPoint(Vec2(0,0));
	buttonChangeAccount->setPosition(Vec2(origin.x+visibleSize.width*0.357,origin.y+visibleSize.height*0.13));
	buttonChangeAccount->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::ChangeAccountCallBack,this));
	this->addChild(buttonChangeAccount,1);

	auto buttonChangeAccountLevel=Button::create("zhanghaoshengji2.png","zhanghaoshengji1.png","zhanghaoshengji1.png",TextureResType::PLIST);
	buttonChangeAccountLevel->setAnchorPoint(Vec2(0,0));
	buttonChangeAccountLevel->setPosition(Vec2(origin.x+visibleSize.width*0.5599,origin.y+visibleSize.height*0.13));
	buttonChangeAccountLevel->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::UpgradeAccount,this));
	this->addChild(buttonChangeAccountLevel,1);

	auto buttonManageCode=Button::create("mimaguanli2.png","mimaguanli1.png","mimaguanli1.png",TextureResType::PLIST);
	buttonManageCode->setAnchorPoint(Vec2(0,0));
	buttonManageCode->setPosition(Vec2(origin.x+visibleSize.width*0.7635,origin.y+visibleSize.height*0.13));
	buttonManageCode->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::ManagePassWord,this));
	this->addChild(buttonManageCode,1);

	for(int i=0;i<4;i++)
	{
		auto line=Sprite::createWithSpriteFrameName("fengexian1.png");
		line->setAnchorPoint(Vec2(0,0));
		line->setPosition(Vec2(origin.x+visibleSize.width*0.3653,origin.y+visibleSize.height*(0.3548+0.113*i)));
		this->addChild(line,1);
	}

	auto ItemOfPetName=Sprite::createWithSpriteFrameName("nichengkuang.png");
	ItemOfPetName->setAnchorPoint(Vec2(0,0));
	ItemOfPetName->setPosition(Vec2(origin.x+visibleSize.width*0.439,origin.y+visibleSize.height*0.593+5));
	this->addChild(ItemOfPetName,1);

	//auto LabelForName=EditBox::create(Size(visibleSize.width*0.3,30),Scale9Sprite::createWithSpriteFrameName("bi2.png"));
	//LabelForName->setAnchorPoint(Vec2(0,0));
	//LabelForName->setPosition(Vec2(origin.x+visibleSize.width*0.3678+10,origin.y+visibleSize.height*0.6145));
	//LabelForName->setFontColor(ccWHITE);
	//LabelForName->setMaxLength(16);
	//LabelForName->setInputMode(cocos2d::extension::EditBox::InputMode::ANY);
	//this->addChild(LabelForName,1,EDITBOX_FOR_NAME);

	auto buttonPen=Button::create("bi2.png","bi1.png","bi2.png",TextureResType::PLIST);
	buttonPen->setAnchorPoint(Vec2(0,0));
	buttonPen->setPosition(Vec2(origin.x+visibleSize.width*0.773,origin.y+visibleSize.height*0.593+5));
	buttonPen->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::ChangeNameForPenCallBack,this));
	this->addChild(buttonPen,1);

	auto LabelForName=LabelTTF::create("赵钱孙李","Arial",30);
	LabelForName->setAnchorPoint(Vec2(0,0));
	LabelForName->setPosition(Vec2(origin.x+visibleSize.width*0.3678+100,origin.y+visibleSize.height*0.6145));
	LabelForName->setColor(ccBLACK);
	this->addChild(LabelForName,2,LABEL_FOR_NAME);

	my_sex=BOY;

	auto manLogo=Sprite::createWithSpriteFrameName("nan.png");
	manLogo->setAnchorPoint(Vec2(0,0));
	manLogo->setPosition(Vec2(origin.x+visibleSize.width*0.539,origin.y+visibleSize.height*0.4986));
	this->addChild(manLogo,1);

	auto buttonBoy=Sprite::createWithSpriteFrameName("xingbie1.png");
	buttonBoy->setAnchorPoint(Vec2(0,0));
	buttonBoy->setPosition(Vec2(origin.x+visibleSize.width*0.457,origin.y+visibleSize.height*0.4888));
	this->addChild(buttonBoy,1);

	auto chooseedFlag1=Sprite::createWithSpriteFrameName("xingbie.png");
	chooseedFlag1->setAnchorPoint(Vec2(0,0));
	chooseedFlag1->setPosition(Vec2(origin.x+visibleSize.width*0.4655,origin.y+visibleSize.height*0.501));
	this->addChild(chooseedFlag1,2,SEX_BOY_SIGN);

	auto womanLogo=Sprite::createWithSpriteFrameName("nv.png");
	womanLogo->setAnchorPoint(Vec2(0,0));
	womanLogo->setPosition(Vec2(origin.x+visibleSize.width*0.7758,origin.y+visibleSize.height*0.4986));
	this->addChild(womanLogo,1);

	auto buttonGirl=Sprite::createWithSpriteFrameName("xingbie1.png");
	buttonGirl->setAnchorPoint(Vec2(0,0));
	buttonGirl->setPosition(Vec2(origin.x+visibleSize.width*0.6937,origin.y+visibleSize.height*0.4888));
	this->addChild(buttonGirl,1);
	
	auto chooseedFlag2=Sprite::createWithSpriteFrameName("xingbie.png");
	chooseedFlag2->setAnchorPoint(Vec2(0,0));
	chooseedFlag2->setPosition(Vec2(origin.x+visibleSize.width*0.702,origin.y+visibleSize.height*0.501));
	this->addChild(chooseedFlag2,2,SEX_GIRL_SIGN);

	switch (my_sex)
	{
	case MyBaseInformation::BOY:
		chooseedFlag2->setVisible(false);
		break;
	case MyBaseInformation::GIRL:
		chooseedFlag1->setVisible(false);
		break;
	default:
		break;
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
	buttonPlus->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::CallBack_ForJiaHao,this));
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

void MyBaseInformation::ChangeAccountCallBack(Ref* pSender,Widget::TouchEventType type)
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
void MyBaseInformation::UpgradeAccount(Ref* pSender,Widget::TouchEventType type)
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
void MyBaseInformation::ManagePassWord(Ref* pSender,Widget::TouchEventType type)
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
void MyBaseInformation::CallBack_ForJiaHao(Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto shopScene=ShopScene::create();
			Director::getInstance()->pushScene(shopScene);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void MyBaseInformation::ChangeNameForPenCallBack(Ref* pSender,Widget::TouchEventType type)
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