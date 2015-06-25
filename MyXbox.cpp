#include "MyXbox.h"
#include "Game/GameType.h"
MyXbox::MyXbox(void)
{
	myBackpack=EnvVariable::getInstance()->get_myBackpack();
}

MyXbox::~MyXbox(void)
{
}
//const char * BoxItems[6][5] = {
//	{"Cheats-dgbf.png","Cheats-da.png","Cheats-gou.png","Cheats-bang.png","Cheats-fa.png"},//打狗棒法
//	{"Cheats-dzxy.png","Cheats-dou.png","Cheats-zhuan.png","Cheats-xing2.png","Cheats-yi.png"},//斗转星移
//	{"Cheats-jyzj.png","Cheats-jiu.png","Cheats-yin.png","Cheats-zhen.png","Cheats-jing.png"},//九阴真经
//	{"Cheats-khbd.png","Cheats-kui.png","Cheats-hua.png","Cheats-bao.png","Cheats-dian.png"},//葵花宝典
//	{"Cheats-sxbb.png","Cheats-shen.png","Cheats-xing.png","Cheats-bai.png","Cheats-bian.png"},//神行百变
//	{"Cheats-xwxg.png","Cheats-xiao.png","Cheats-wu.png","Cheats-xiang.png","Cheats-gong.png"},//小无相功
//};
//const int BoxItemNum[6][5]={
//	{1,1,2,3,4},
//	{2,2,2,3,4},
//	{3,3,3,3,4},
//	{4,4,4,4,4},
//	{5,5,6,7,8},
//	{6,6,7,8,9},
//};
void MyXbox::itemsInBox_callBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto curButton=(Button*)pSender;
			auto curEffect=curButton->getChildByTag(ITEMS_CHOOSED_EFC);
			auto lastEffect=this->getChildByTag(SC_FOR_MYBOX)->getChildByTag(curEffectNum)->getChildByTag(ITEMS_CHOOSED_EFC);
			lastEffect->setVisible(false);
			curEffect->setVisible(true);
			curEffectNum=curButton->getTag();
			if(curEffectNum<myBackpack.size())
			{
				if(myBackpack[curEffectNum].ItemFragment[0]!="")
					composeCreate(myBackpack[curEffectNum]);
				else
					inforPanelCreate(myBackpack[curEffectNum]);
			}
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

bool MyXbox::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Cheats.plist");

	auto sc=ui::ScrollView::create();
	sc->setSize(Size(633,537));
	sc->setBounceEnabled(true);
	sc->setAnchorPoint(Vec2(0,0));
	sc->setInnerContainerSize(Size(visibleSize.width*0.5197,visibleSize.height*2.5));
	sc->setPosition(Vec2(origin.x+visibleSize.width*0.03,origin.y+visibleSize.height*0.09));
	this->addChild(sc,1,SC_FOR_MYBOX);

	for(int i=0;i<10;i++)
	{
		auto buttonXbox1=Button::create("mijikuang.png","mijikuang.png","mijikuang.png",TextureResType::PLIST);
		buttonXbox1->addTouchEventListener(CC_CALLBACK_2(MyXbox::itemsInBox_callBack,this));
		buttonXbox1->setAnchorPoint(Vec2(0,1));
		buttonXbox1->setPosition(Vec2(origin.x+visibleSize.width*0.01,origin.y+visibleSize.height*(2.465-0.25*i)));
		sc->addChild(buttonXbox1,1,MY_BOX_MIJIK_TAG+i*3);
		auto firstSize=buttonXbox1->getContentSize();

		auto imageOfSelectEffect1=Sprite::createWithSpriteFrameName("dianjixiaoguo.png");
		imageOfSelectEffect1->setAnchorPoint(Vec2(0.5,0.5));
		imageOfSelectEffect1->setPosition(Vec2(firstSize.width/2,firstSize.height/2));
		buttonXbox1->addChild(imageOfSelectEffect1,1,ITEMS_CHOOSED_EFC);
		imageOfSelectEffect1->setScale(0.98);
		imageOfSelectEffect1->setVisible(false);

		auto imageOfnumber1=Sprite::createWithSpriteFrameName("mijishuliang.png");
		imageOfnumber1->setAnchorPoint(Vec2(0.5,0.5));
		imageOfnumber1->setPosition(Vec2(firstSize.width,firstSize.height));
		buttonXbox1->addChild(imageOfnumber1,3,NUM_OF_MIJI);
		imageOfnumber1->setVisible(false);

		auto numCount1=LabelTTF::create("","Arial",20);
		numCount1->setAnchorPoint(Vec2(0.5,0.5));
		numCount1->setPosition(Vec2(firstSize.width,firstSize.height));
		buttonXbox1->addChild(numCount1,4,NUM_COUNT_MIJI);
		numCount1->setVisible(false);

		auto buttonXbox2=Button::create("mijikuang.png","mijikuang.png","mijikuang.png",TextureResType::PLIST);
		buttonXbox2->addTouchEventListener(CC_CALLBACK_2(MyXbox::itemsInBox_callBack,this));
		buttonXbox2->setAnchorPoint(Vec2(0,1));
		buttonXbox2->setPosition(Vec2(origin.x+visibleSize.width*0.1836,origin.y+visibleSize.height*(2.465-0.25*i)));
		sc->addChild(buttonXbox2,1,MY_BOX_MIJIK_TAG+i*3+1);
		auto scendSize=buttonXbox2->getContentSize();

		auto imageOfSelectEffect2=Sprite::createWithSpriteFrameName("dianjixiaoguo.png");
		imageOfSelectEffect2->setAnchorPoint(Vec2(0.5,0.5));
		imageOfSelectEffect2->setPosition(Vec2(scendSize.width/2,scendSize.height/2));
		buttonXbox2->addChild(imageOfSelectEffect2,1,ITEMS_CHOOSED_EFC);
		imageOfSelectEffect2->setScale(0.98);
		imageOfSelectEffect2->setVisible(false);

		auto imageOfnumber2=Sprite::createWithSpriteFrameName("mijishuliang.png");
		imageOfnumber2->setAnchorPoint(Vec2(0.5,0.5));
		imageOfnumber2->setPosition(Vec2(scendSize.width,scendSize.height));
		buttonXbox2->addChild(imageOfnumber2,3,NUM_OF_MIJI);
		imageOfnumber2->setVisible(false);

		auto numCount2=LabelTTF::create("","Arial",20);
		numCount2->setAnchorPoint(Vec2(0.5,0.5));
		numCount2->setPosition(Vec2(scendSize.width,scendSize.height));
		buttonXbox2->addChild(numCount2,4,NUM_COUNT_MIJI);
		numCount2->setVisible(false);

		auto buttonXbox3=Button::create("mijikuang.png","mijikuang.png","mijikuang.png",TextureResType::PLIST);
		buttonXbox3->addTouchEventListener(CC_CALLBACK_2(MyXbox::itemsInBox_callBack,this));
		buttonXbox3->setAnchorPoint(Vec2(0,1));
		buttonXbox3->setPosition(Vec2(origin.x+visibleSize.width*0.3572,origin.y+visibleSize.height*(2.465-0.25*i)));
		sc->addChild(buttonXbox3,1,MY_BOX_MIJIK_TAG+i*3+2);
		auto thirdSize=buttonXbox3->getContentSize();

		auto imageOfSelectEffect3=Sprite::createWithSpriteFrameName("dianjixiaoguo.png");
		imageOfSelectEffect3->setAnchorPoint(Vec2(0.5,0.5));
		imageOfSelectEffect3->setPosition(Vec2(thirdSize.width/2,thirdSize.height/2));
		buttonXbox3->addChild(imageOfSelectEffect3,1,ITEMS_CHOOSED_EFC);
		imageOfSelectEffect3->setScale(0.98);
		imageOfSelectEffect3->setVisible(false);

		auto imageOfnumber3=Sprite::createWithSpriteFrameName("mijishuliang.png");
		imageOfnumber3->setAnchorPoint(Vec2(0.5,0.5));
		imageOfnumber3->setPosition(Vec2(thirdSize.width,thirdSize.height));
		buttonXbox3->addChild(imageOfnumber3,3,NUM_OF_MIJI);
		imageOfnumber3->setVisible(false);

		auto numCount3=LabelTTF::create("","Arial",20);
		numCount3->setAnchorPoint(Vec2(0.5,0.5));
		numCount3->setPosition(Vec2(thirdSize.width,thirdSize.height));
		buttonXbox3->addChild(numCount3,4,NUM_COUNT_MIJI);
		numCount3->setVisible(false);
	}

	char tempChar[10];
	for(int i=0;i<myBackpack.size();i++)
	{
		auto curBoxButton=(Button*)sc->getChildByTag(MY_BOX_MIJIK_TAG+i);
		auto curBoxSize=curBoxButton->getContentSize();

		auto miji_png_box=Sprite::createWithSpriteFrameName(myBackpack[i].IconPhoto);
		miji_png_box->setAnchorPoint(Vec2(0.5,0.5));
		miji_png_box->setPosition(Vec2(curBoxSize.width/2,curBoxSize.height/2));
		curBoxButton->addChild(miji_png_box,2,ITEMS_MIJI_BOOK);

		if(myBackpack[i].Amount>0)
		{
			auto curMijiCount=(LabelTTF*)curBoxButton->getChildByTag(NUM_COUNT_MIJI);
			auto curMijiNumYuan=curBoxButton->getChildByTag(NUM_OF_MIJI);
			sprintf(tempChar,"%d",myBackpack[i].Amount);
			curMijiCount->setString(tempChar);
			curMijiNumYuan->setVisible(true);
			curMijiCount->setVisible(true);
		}
	}

	auto curEffect=sc->getChildByTag(MY_BOX_MIJIK_TAG)->getChildByTag(ITEMS_CHOOSED_EFC);
	curEffect->setVisible(true);
	curEffectNum=1;

	if(myBackpack[0].ItemFragment[0]!="")
		composeCreate(myBackpack[0]);
	else
		inforPanelCreate(myBackpack[0]);

	return true;
}
void MyXbox::composeCreate(Backpack_Item curItem)
{
	if(this->getChildByTag(ITEM_INFORMATION_LAY))
		this->removeChildByTag(ITEM_INFORMATION_LAY,true);

	char tempChar[20];

	auto detailInfo=Sprite::createWithSpriteFrameName("beibaoxiaobeijing.png");
	detailInfo->setAnchorPoint(Vec2(0.5,0.5));
	auto layr_bkg=Layout::create();
	layr_bkg->setAnchorPoint(Vec2::ZERO);
	layr_bkg->setSize(detailInfo->getContentSize());
	layr_bkg->setPosition(Vec2(origin.x+visibleSize.width*0.5878-30,origin.y+visibleSize.height*0.088));
	detailInfo->setPosition(Vec2(layr_bkg->getSize().width/2,layr_bkg->getSize().height/2));
	layr_bkg->addChild(detailInfo,1);
	this->addChild(layr_bkg,1,ITEM_INFORMATION_LAY);
	auto bkg_size=layr_bkg->getSize();

	auto labelOfDetailInfo=LabelTTF::create("详细信息","Arial",30);
	labelOfDetailInfo->setAnchorPoint(Vec2(0,0));
	labelOfDetailInfo->setPosition(Vec2(origin.x+visibleSize.width*0.1522,origin.y+visibleSize.height*0.687));
	layr_bkg->addChild(labelOfDetailInfo,2);

	auto imageOfSelectedXbox=Sprite::createWithSpriteFrameName("mijikuang.png");
	imageOfSelectedXbox->setAnchorPoint(Vec2(0,0));
	imageOfSelectedXbox->setPosition(Vec2(origin.x+visibleSize.width*0.0122,origin.y+visibleSize.height*0.53));
	layr_bkg->addChild(imageOfSelectedXbox,2);
	auto SelectedBoxSize=imageOfSelectedXbox->getContentSize();

	auto imageMiji=Sprite::createWithSpriteFrameName(curItem.IconPhoto);
	imageMiji->setAnchorPoint(Vec2(0.5,0.5));
	imageMiji->setPosition(Vec2(SelectedBoxSize.width/2,SelectedBoxSize.height/2));
	imageOfSelectedXbox->addChild(imageMiji);

	auto curName=LabelTTF::create(curItem.IconName,"Arial",20);
	curName->setAnchorPoint(Vec2(0,0.5));
	curName->setPosition(Vec2(imageOfSelectedXbox->getPosition().x+imageOfSelectedXbox->getContentSize().width+20,imageOfSelectedXbox->getPosition().y+imageOfSelectedXbox->getContentSize().height/2));
	layr_bkg->addChild(curName,2);

	auto curIntroduce=LabelTTF::create(curItem.DetailedInformation,"Arial",20);
	curIntroduce->setAnchorPoint(Vec2(0.5,1));
	curIntroduce->setPosition(Vec2(bkg_size.width/2,imageOfSelectedXbox->getPosition().y-20));
	layr_bkg->addChild(curIntroduce,2);

	for(int a=0;a<4;a++)
	{
		auto detailXbox1=Sprite::createWithSpriteFrameName("zikuang.png");
		detailXbox1->setAnchorPoint(Vec2(0,0));
		detailXbox1->setPosition(Vec2(origin.x+visibleSize.width*0.0368605+visibleSize.width*0.102*a-30,origin.y+visibleSize.height*0.24));
		layr_bkg->addChild(detailXbox1,2,ZI_KUANG_PNG);
		auto detailSize1=detailXbox1->getContentSize();

		auto firstZi=Sprite::createWithSpriteFrameName(curItem.ItemFragment[a]);
		firstZi->setAnchorPoint(Vec2(0.5,0.5));
		firstZi->setPosition(Vec2(detailSize1.width/2,detailSize1.height/2));
		detailXbox1->addChild(firstZi,1);

		auto numberOfDetailXbox1=Sprite::createWithSpriteFrameName("zishuliang.png");
		numberOfDetailXbox1->setAnchorPoint(Vec2(0.5,0.5));
		numberOfDetailXbox1->setPosition(Vec2(detailSize1.width,detailSize1.height));
		detailXbox1->addChild(numberOfDetailXbox1,2);
		numberOfDetailXbox1->setVisible(false);

		sprintf(tempChar,"%d",curItem.ItemFragmentNum[a]);
		auto numForCount1=LabelTTF::create(tempChar,"Arial",20);
		numForCount1->setAnchorPoint(Vec2(0.5,0.5));
		numForCount1->setPosition(Vec2(detailSize1.width,detailSize1.height));
		detailXbox1->addChild(numForCount1,2,ZI_COUNT_NUM);
		numberOfDetailXbox1->setVisible(false);

		if(curItem.ItemFragmentNum[a]>0)
		{
			numberOfDetailXbox1->setVisible(true);
			detailXbox1->setVisible(true);
		}
	}

	auto convertButton=Button::create("baoming anniu2.png","baoming anniu.png","baoming anniu.png",UI_TEX_TYPE_PLIST);
	convertButton->addTouchEventListener(CC_CALLBACK_2(MyXbox::convertCallback,this));
	auto sizeOfChange=convertButton->getContentSize();
	convertButton->setScale(0.8);
	convertButton->setPosition(Vec2(origin.x+visibleSize.width*0.12,origin.y+visibleSize.height*0.0816927));
	layr_bkg->addChild(convertButton,2);

	auto imageOfLabelChange=Sprite::createWithSpriteFrameName("duihuan.png");
	auto sizeOfLabelChange=imageOfLabelChange->getContentSize();
	imageOfLabelChange->setRotation(-90);
	imageOfLabelChange->setPosition(Vec2(origin.x+visibleSize.width*0.12,origin.y+visibleSize.height*0.0816927));
	layr_bkg->addChild(imageOfLabelChange,2);
}
void MyXbox::inforPanelCreate(Backpack_Item curItem)
{
	if(this->getChildByTag(ITEM_INFORMATION_LAY))
		this->removeChildByTag(ITEM_INFORMATION_LAY,true);

	char tempChar[20];

	auto detailInfo=Sprite::createWithSpriteFrameName("beibaoxiaobeijing.png");
	detailInfo->setAnchorPoint(Vec2(0.5,0.5));
	auto layr_bkg=Layout::create();
	layr_bkg->setAnchorPoint(Vec2::ZERO);
	layr_bkg->setSize(detailInfo->getContentSize());
	layr_bkg->setPosition(Vec2(origin.x+visibleSize.width*0.5878-30,origin.y+visibleSize.height*0.088));
	detailInfo->setPosition(Vec2(layr_bkg->getSize().width/2,layr_bkg->getSize().height/2));
	layr_bkg->addChild(detailInfo,1);
	this->addChild(detailInfo,1,ITEM_INFORMATION_LAY);
	auto bkg_size=layr_bkg->getSize();

	auto labelOfDetailInfo=LabelTTF::create("详细信息","Arial",30);
	labelOfDetailInfo->setAnchorPoint(Vec2(0,0));
	labelOfDetailInfo->setPosition(Vec2(origin.x+visibleSize.width*0.1522,origin.y+visibleSize.height*0.687));
	layr_bkg->addChild(labelOfDetailInfo,2);

	auto imageOfSelectedXbox=Sprite::createWithSpriteFrameName("mijikuang.png");
	imageOfSelectedXbox->setAnchorPoint(Vec2(0,0));
	imageOfSelectedXbox->setPosition(Vec2(origin.x+visibleSize.width*0.0122,origin.y+visibleSize.height*0.53));
	layr_bkg->addChild(imageOfSelectedXbox,2);
	auto SelectedBoxSize=imageOfSelectedXbox->getContentSize();

	auto imageMiji=Sprite::createWithSpriteFrameName(curItem.IconPhoto);
	imageMiji->setAnchorPoint(Vec2(0.5,0.5));
	imageMiji->setPosition(Vec2(SelectedBoxSize.width/2,SelectedBoxSize.height/2));
	imageOfSelectedXbox->addChild(imageMiji);

	auto curName=LabelTTF::create(curItem.IconName,"Arial",20);
	curName->setAnchorPoint(Vec2(0,0.5));
	curName->setPosition(Vec2(imageOfSelectedXbox->getPosition().x+imageOfSelectedXbox->getContentSize().width+20,imageOfSelectedXbox->getPosition().y+imageOfSelectedXbox->getContentSize().height/2));
	layr_bkg->addChild(curName,2);

	auto curIntroduce=LabelTTF::create(curItem.DetailedInformation,"Arial",20);
	curIntroduce->setAnchorPoint(Vec2(0.5,1));
	curIntroduce->setPosition(Vec2(bkg_size.width/2,imageOfSelectedXbox->getPosition().y-20));
	layr_bkg->addChild(curIntroduce,2);
}
void MyXbox::updateGoodCount()
{
	char tempChar[10];
	myBackpack=EnvVariable::getInstance()->get_myBackpack();
	auto curBookItem=this->getChildByTag(SC_FOR_MYBOX)->getChildByTag(curEffectNum);
	auto curBookNumPng=(Sprite*)curBookItem->getChildByTag(NUM_OF_MIJI);
	auto curBookNum=(LabelTTF*)curBookItem->getChildByTag(NUM_COUNT_MIJI);

	sprintf(tempChar,"%d",myBackpack[curEffectNum].Amount);
	curBookNum->setString(tempChar);

	if(myBackpack[curEffectNum].Amount>0)
	{
		if(!curBookNum->isVisible())
			curBookNum->setVisible(true);
		if(!curBookNumPng->isVisible())
			curBookNumPng->setVisible(true);
	}
}
void MyXbox::NoticeEnsure(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			if(this->getChildByTag(SYSTEM_NOTICE))
				this->runAction(CallFunc::create([=](){this->removeChildByTag(SYSTEM_NOTICE,true);}));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void MyXbox::NoticeCreate(std::string photoName)
{
	auto bkg=Sprite::create("system_Notice.png");
	bkg->setAnchorPoint(Vec2(0.5,0.5));
	bkg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(bkg,5,SYSTEM_NOTICE);
	auto bkg_size=bkg->getContentSize();

	auto bkgSwallow=LayerColor::create();
	bkgSwallow->setContentSize(Size(visibleSize.width,visibleSize.height));
	bkgSwallow->setColor(ccWHITE);
	bkgSwallow->setAnchorPoint(Vec2(0.5,0.5));
	bkgSwallow->setPosition(Vec2(bkg_size.width/2,bkg_size.height/2));
	bkg->addChild(bkgSwallow);
	EventListenerTouchOneByOne* bkgSwallowListener=EventListenerTouchOneByOne::create();
	bkgSwallowListener->setSwallowTouches(true);
	bkgSwallowListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	bkgSwallowListener->onTouchMoved=[=](Touch* touch, Event* event){};
	bkgSwallowListener->onTouchEnded=[=](Touch* touch, Event* event){};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(bkgSwallowListener,bkgSwallow);

	auto convertGood=Sprite::createWithSpriteFrameName(photoName);
	convertGood->setAnchorPoint(Vec2(0.5,0.5));
	convertGood->setPosition(Vec2(bkg_size.width/2,bkg_size.height*0.65));
	bkg->addChild(convertGood);

	auto convertEnsure=Button::create("systemprompt-affirm.png","systemprompt-affirm2.png","systemprompt-affirm2.png",UI_TEX_TYPE_PLIST);
	convertEnsure->setAnchorPoint(Vec2(0.5,0));
	convertEnsure->setPosition(Vec2(bkg_size.width/2,bkg_size.height*0.15));
	convertEnsure->addTouchEventListener(CC_CALLBACK_2(MyXbox::NoticeEnsure,this));
	bkg->addChild(convertEnsure);
}
void MyXbox::convertCallback(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			updateGoodCount();
			NoticeCreate(myBackpack[curEffectNum].IconPhoto);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}