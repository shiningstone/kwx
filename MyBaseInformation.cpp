#include "MyBaseInformation.h"
#include "ShopScene.h"

MyBaseInformation::MyBaseInformation()
{
	playerInforMation=EnvVariable::getInstance()->get_personalDetailed();
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
	char tempChar[20];

	auto perInfoBg=Sprite::create("PerInformation.png");
	perInfoBg->setAnchorPoint(Vec2(0,0));
	perInfoBg->setPosition(Vec2(origin.x+visibleSize.width*0.3087,origin.y+visibleSize.height*0.85));
	perInfoBg->setRotation(90);
	this->addChild(perInfoBg,1);

	auto photoKuang=Sprite::createWithSpriteFrameName("touxiangkuang.png");
	photoKuang->setAnchorPoint(Vec2(0,0));
	photoKuang->setPosition(Vec2(origin.x+visibleSize.width*0.0435,origin.y+visibleSize.height*0.474));
	this->addChild(photoKuang,1);

	auto playerPhoto=Sprite::createWithSpriteFrameName(playerInforMation.PhotoName);//头像
	playerPhoto->setAnchorPoint(Vec2(0.5,0.5));
	playerPhoto->setPosition(Vec2(photoKuang->getContentSize().width/2,photoKuang->getContentSize().height/2));
	playerPhoto->setScale(227/playerPhoto->getTextureRect().size.width,227/playerPhoto->getTextureRect().size.height);
	photoKuang->addChild(playerPhoto);

	auto buttonChangePhoto=Button::create("xiugaitouxiang.png","xiugaitouxiang1.png","xiugaitouxiang1.png",TextureResType::PLIST);
	buttonChangePhoto->setAnchorPoint(Vec2(0,0));
	buttonChangePhoto->setPosition(Vec2(origin.x+visibleSize.width*0.0607,origin.y+visibleSize.height*0.359));
	buttonChangePhoto->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::onButtonChangePhoto,this));
	this->addChild(buttonChangePhoto,1);
	
	auto labelOfID=LabelTTF::create("ID:","Arial",30);
	labelOfID->setAnchorPoint(Vec2(0,0));
	labelOfID->setPosition(Vec2(origin.x+visibleSize.width*0.3678,origin.y+visibleSize.height*0.7304));
	this->addChild(labelOfID,1);

	sprintf(tempChar,"%d",playerInforMation.id);
	auto LabelForID=LabelTTF::create(tempChar,"Arial",30);//ID
	LabelForID->setAnchorPoint(Vec2(0,0));
	LabelForID->setPosition(Vec2(origin.x+visibleSize.width*0.3678+labelOfID->getContentSize().width+10,origin.y+visibleSize.height*0.7304));
	this->addChild(LabelForID,1,MY_BASEINFO_ID);
	
	auto labelOfAccount=LabelTTF::create("账户:","Arial",30);
	labelOfAccount->setAnchorPoint(Vec2(0,0));
	labelOfAccount->setPosition(Vec2(origin.x+visibleSize.width*0.596,origin.y+visibleSize.height*0.73));
	this->addChild(labelOfAccount,1);

	auto LabelForAccount=LabelTTF::create(playerInforMation.account,"Arial",30);//账户
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

	sprintf(tempChar,"%d",playerInforMation.Winrate);
	auto WinRate=LabelTTF::create(tempChar,"Arial",30);//胜率
	WinRate->setAnchorPoint(Vec2(0,0));
	WinRate->setPosition(Vec2(labelOfWinRate->getPosition().x+labelOfWinRate->getContentSize().width+20,origin.y+visibleSize.height*0.3835));
	this->addChild(WinRate,1,WIN_RATE);

	auto labelOfHistory=LabelTTF::create("历史最大番次数:","Arial",30);
	labelOfHistory->setAnchorPoint(Vec2(0,0));
	labelOfHistory->setPosition(Vec2(origin.x+visibleSize.width*0.3678,origin.y+visibleSize.height*0.2821));
	this->addChild(labelOfHistory,1);

	sprintf(tempChar,"%d",playerInforMation.maxTime);
	auto MaxHistory=LabelTTF::create(tempChar,"Arial",30);//最大番型
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

	auto buttonPen=Button::create("bi2.png","bi1.png","bi2.png",TextureResType::PLIST);
	buttonPen->setAnchorPoint(Vec2(0,0));
	buttonPen->setPosition(Vec2(origin.x+visibleSize.width*0.773,origin.y+visibleSize.height*0.593+5));
	buttonPen->addTouchEventListener(CC_CALLBACK_2(MyBaseInformation::ChangeNameForPenCallBack,this));
	this->addChild(buttonPen,1);

	auto LabelForName=LabelTTF::create(playerInforMation.Nickname,"Arial",30);//昵称
	LabelForName->setAnchorPoint(Vec2(0,0));
	LabelForName->setPosition(Vec2(origin.x+visibleSize.width*0.3678+100,origin.y+visibleSize.height*0.6145));
	LabelForName->setColor(ccBLACK);
	this->addChild(LabelForName,2,LABEL_FOR_NAME);

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

	switch (playerInforMation.PlayerSex)//性别
	{
	case BOY:
		chooseedFlag2->setVisible(false);
		break;
	case GIRL:
		chooseedFlag1->setVisible(false);
		break;
	default:
		break;
	}

	auto LevelProgressBg=Sprite::createWithSpriteFrameName("jingyantiao.png");
	LevelProgressBg->setAnchorPoint(Vec2(0.5,0.5));
	LevelProgressBg->setPosition(Vec2(origin.x+visibleSize.width*0.12,origin.y+visibleSize.height*0.3));
	this->addChild(LevelProgressBg,1);

	int playerLv=(int)playerInforMation.PlayerLv;

	auto LevelProgress=Sprite::createWithSpriteFrameName("jingyantiao1.png");//经验条
	CCProgressTimer* pt = CCProgressTimer::create(LevelProgress);  
    pt->setMidpoint(ccp(0, 0));  
	pt->setType(kCCProgressTimerTypeBar);
    pt->setBarChangeRate(ccp(1,0));  
    pt->setPercentage((playerInforMation.PlayerLv-playerLv)*100);  
	pt->ignoreAnchorPointForPosition(false);
	pt->setAnchorPoint(Vec2(0.5,0.5));
    pt->setPosition(Vec2(origin.x+visibleSize.width*0.12,origin.y+visibleSize.height*0.3));
    this->addChild(pt,2);  

	sprintf(tempChar,"Lv.%d",playerLv);//等级
	auto labelOfLevel=LabelTTF::create(tempChar,"Arial",20);
	labelOfLevel->setAnchorPoint(Vec2(0,0.5));
	labelOfLevel->setPosition(Vec2(origin.x+visibleSize.width*0.22,origin.y+visibleSize.height*0.3));
	this->addChild(labelOfLevel,2);

	LabelTTF* identity;
	switch (playerInforMation.accountType)//用户身份
	{
	case Tourist:
		identity=LabelTTF::create("当前身份: 游客身份","Arial",30);
		break;
	case KWXAccount:
		identity=LabelTTF::create("当前身份: KWX用户","Arial",30);
		break;
	case QQAccount:
		identity=LabelTTF::create("当前身份: QQ用户","Arial",30);
		break;
	default:
		break;
	}
	identity->setAnchorPoint(Vec2(0,0.5));
	identity->setPosition(Vec2(origin.x+visibleSize.width*0.05,origin.y+visibleSize.height*0.2));
	this->addChild(identity,2);

	auto GoldFont=LabelTTF::create("金币: ","Arial",30);
	GoldFont->setAnchorPoint(Vec2(0,0.5));
	GoldFont->setPosition(Vec2(origin.x+visibleSize.width*0.05,origin.y+visibleSize.height*0.13));
	this->addChild(GoldFont,2);

	sprintf(tempChar,"%d",playerInforMation.GoldNum);
	auto GoldNum=LabelTTF::create(tempChar,"Arial",30);
	GoldNum->setAnchorPoint(Vec2(0,0.5));
	GoldNum->setPosition(Vec2(GoldFont->getPosition().x+GoldFont->getContentSize().width,origin.y+visibleSize.height*0.13));
	this->addChild(GoldNum,2);

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
//void MyBaseInformation::CallBack_ForJiaHao(Ref* pSender,Widget::TouchEventType type)
//{
//	switch (type)
//	{
//	case cocos2d::ui::Widget::TouchEventType::BEGAN:
//		break;
//	case cocos2d::ui::Widget::TouchEventType::MOVED:
//		break;
//	case cocos2d::ui::Widget::TouchEventType::ENDED:
//		{
//			auto shopScene=ShopScene::create();
//			Director::getInstance()->pushScene(shopScene);
//		}
//		break;
//	case cocos2d::ui::Widget::TouchEventType::CANCELED:
//		break;
//	default:
//		break;
//	}
//}
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