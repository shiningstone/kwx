#include "MyXbox.h"
MyXbox::MyXbox(void)
{
}


MyXbox::~MyXbox(void)
{
}
const char * BoxItems[6][5] = {
	{"Cheats-dgbf.png","Cheats-da.png","Cheats-gou.png","Cheats-bang.png","Cheats-fa.png"},//打狗棒法
	{"Cheats-dzxy.png","Cheats-dou.png","Cheats-zhuan.png","Cheats-xing2.png","Cheats-yi.png"},//斗转星移
	{"Cheats-jyzj.png","Cheats-jiu.png","Cheats-yin.png","Cheats-zhen.png","Cheats-jing.png"},//九阴真经
	{"Cheats-khbd.png","Cheats-kui.png","Cheats-hua.png","Cheats-bao.png","Cheats-dian.png"},//葵花宝典
	{"Cheats-sxbb.png","Cheats-shen.png","Cheats-xing.png","Cheats-bai.png","Cheats-bian.png"},//神行百变
	{"Cheats-xwxg.png","Cheats-xiao.png","Cheats-wu.png","Cheats-xiang.png","Cheats-gong.png"},//小无相功
};
const int BoxItemNum[6][5]={
	{1,1,2,3,4},
	{2,2,2,3,4},
	{3,3,3,3,4},
	{4,4,4,4,4},
	{5,5,6,7,8},
	{6,6,7,8,9},
};
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

	char tempNumForZi[10];
	for(int i=0;i<6;i++)
	{
		auto curBoxButton=(Button*)sc->getChildByTag(MY_BOX_MIJIK_TAG+i);
		auto curBoxSize=curBoxButton->getContentSize();

		auto miji_png_box=Sprite::createWithSpriteFrameName(BoxItems[i][0]);
		miji_png_box->setAnchorPoint(Vec2(0.5,0.5));
		miji_png_box->setPosition(Vec2(curBoxSize.width/2,curBoxSize.height/2));
		curBoxButton->addChild(miji_png_box,2,ITEMS_MIJI_BOOK);

		switch (i)
		{
		case 0:
			{
				auto curEffect=curBoxButton->getChildByTag(ITEMS_CHOOSED_EFC);
				curEffect->setVisible(true);
				curEffectNum=1;
			}
			break;
		default:
			break;
		}
		if(BoxItemNum[i][0]>0)
		{
			auto curMijiCount=(LabelTTF*)curBoxButton->getChildByTag(NUM_COUNT_MIJI);
			auto curMijiNumYuan=curBoxButton->getChildByTag(NUM_OF_MIJI);
			sprintf(tempNumForZi,"%d",BoxItemNum[i][0]);
			curMijiCount->setString(tempNumForZi);
			curMijiNumYuan->setVisible(true);
			curMijiCount->setVisible(true);
		}
	}

	auto detailInfo=Sprite::createWithSpriteFrameName("beibaoxiaobeijing.png");
	auto sizeOfDetailInfo=detailInfo->getContentSize();
	detailInfo->setAnchorPoint(Vec2(0,0));
	detailInfo->setPosition(Vec2(origin.x+visibleSize.width*0.5878-30,origin.y+visibleSize.height*0.088));
	this->addChild(detailInfo,1);

	auto labelOfDetailInfo=LabelTTF::create("详细信息","Arial",30);
	labelOfDetailInfo->setAnchorPoint(Vec2(0,0));
	auto labelOfInfoID=labelOfDetailInfo->getContentSize();
	labelOfDetailInfo->setPosition(Vec2(origin.x+visibleSize.width*0.74-30,origin.y+visibleSize.height*0.775));
	this->addChild(labelOfDetailInfo,2);

	auto imageOfSelectedXbox=Sprite::createWithSpriteFrameName("mijikuang.png");
	imageOfSelectedXbox->setAnchorPoint(Vec2(0,0));
	imageOfSelectedXbox->setPosition(Vec2(origin.x+visibleSize.width*0.6-30,origin.y+visibleSize.height*0.53));
	this->addChild(imageOfSelectedXbox,2,DETAILED_OF_MIJI);
	auto SelectedBoxSize=imageOfSelectedXbox->getContentSize();

	auto imageMiji=Sprite::createWithSpriteFrameName("Cheats-dgbf.png");
	imageMiji->setAnchorPoint(Vec2(0.5,0.5));
	imageMiji->setPosition(Vec2(SelectedBoxSize.width/2,SelectedBoxSize.height/2));
	imageOfSelectedXbox->addChild(imageMiji);

	auto detailXbox1=Sprite::createWithSpriteFrameName("zikuang.png");
	detailXbox1->setAnchorPoint(Vec2(0,0));
	detailXbox1->setPosition(Vec2(origin.x+visibleSize.width*0.6-30,origin.y+visibleSize.height*0.24));
	this->addChild(detailXbox1,2,CURMJ_ITEM_ONE);
	auto detailSize1=detailXbox1->getContentSize();

	auto firstZi=Sprite::createWithSpriteFrameName("Cheats-da.png");
	firstZi->setAnchorPoint(Vec2(0.5,0.5));
	firstZi->setPosition(Vec2(detailSize1.width/2,detailSize1.height/2));
	detailXbox1->addChild(firstZi,1,ZI_PNG);

	auto numberOfDetailXbox1=Sprite::createWithSpriteFrameName("zishuliang.png");
	numberOfDetailXbox1->setAnchorPoint(Vec2(0.5,0.5));
	numberOfDetailXbox1->setPosition(Vec2(detailSize1.width,detailSize1.height));
	detailXbox1->addChild(numberOfDetailXbox1,2,ZI_MUN_PNG);
	numberOfDetailXbox1->setVisible(false);

	auto numForCount1=LabelTTF::create("","Arial",20);
	numForCount1->setAnchorPoint(Vec2(0.5,0.5));
	numForCount1->setPosition(Vec2(detailSize1.width,detailSize1.height));
	detailXbox1->addChild(numForCount1,2,ZI_NUM);
	numForCount1->setVisible(false);

	if(BoxItemNum[0][1]>0)
	{
		numberOfDetailXbox1->setVisible(true);
		sprintf(tempNumForZi,"%d",BoxItemNum[0][1]);
		numForCount1->setString(tempNumForZi);
		numForCount1->setVisible(true);
	}

	auto detailXbox2=Sprite::createWithSpriteFrameName("zikuang.png");
	detailXbox2->setAnchorPoint(Vec2(0,0));
	detailXbox2->setPosition(Vec2(origin.x+visibleSize.width*0.702-30,origin.y+visibleSize.height*0.24));
	this->addChild(detailXbox2,2,CURMJ_ITEM_TWO);
	auto detailSize2=detailXbox2->getContentSize();

	auto scendZi=Sprite::createWithSpriteFrameName("Cheats-gou.png");
	scendZi->setAnchorPoint(Vec2(0.5,0.5));
	scendZi->setPosition(Vec2(detailSize2.width/2,detailSize2.height/2));
	detailXbox2->addChild(scendZi,1,ZI_PNG);

	auto numberOfDetailXbox2=Sprite::createWithSpriteFrameName("zishuliang.png");
	numberOfDetailXbox2->setAnchorPoint(Vec2(0.5,0.5));
	numberOfDetailXbox2->setPosition(Vec2(detailSize2.width,detailSize2.height));
	detailXbox2->addChild(numberOfDetailXbox2,2,ZI_MUN_PNG);
	numberOfDetailXbox2->setVisible(false);

	auto numForCount2=LabelTTF::create("","Arial",20);
	numForCount2->setAnchorPoint(Vec2(0.5,0.5));
	numForCount2->setPosition(Vec2(detailSize2.width,detailSize2.height));
	detailXbox2->addChild(numForCount2,2,ZI_NUM);
	numForCount2->setVisible(false);

	if(BoxItemNum[0][2]>0)
	{
		numberOfDetailXbox2->setVisible(true);
		sprintf(tempNumForZi,"%d",BoxItemNum[0][2]);
		numForCount2->setString(tempNumForZi);
		numForCount2->setVisible(true);
	}

	auto detailXbox3=Sprite::createWithSpriteFrameName("zikuang.png");
	detailXbox3->setAnchorPoint(Vec2(0,0));
	detailXbox3->setPosition(Vec2(origin.x+visibleSize.width*0.804-30,origin.y+visibleSize.height*0.24));
	this->addChild(detailXbox3,2,CURMJ_ITEM_THREE);
	auto detailSize3=detailXbox3->getContentSize();

	auto thirdZi=Sprite::createWithSpriteFrameName("Cheats-bang.png");
	thirdZi->setAnchorPoint(Vec2(0.5,0.5));
	thirdZi->setPosition(Vec2(detailSize3.width/2,detailSize3.height/2));
	detailXbox3->addChild(thirdZi,1,ZI_PNG);

	auto numberOfDetailXbox3=Sprite::createWithSpriteFrameName("zishuliang.png");
	numberOfDetailXbox3->setAnchorPoint(Vec2(0.5,0.5));
	numberOfDetailXbox3->setPosition(Vec2(detailSize3.width,detailSize3.height));
	detailXbox3->addChild(numberOfDetailXbox3,2,NUM_ITEM_THREE);
	numberOfDetailXbox3->setVisible(false);

	auto numForCount3=LabelTTF::create("","Arial",20);
	numForCount3->setAnchorPoint(Vec2(0.5,0.5));
	numForCount3->setPosition(Vec2(detailSize3.width,detailSize3.height));
	detailXbox3->addChild(numForCount3,2,ZI_NUM);
	numForCount3->setVisible(false);

	if(BoxItemNum[0][3]>0)
	{
		numberOfDetailXbox3->setVisible(true);
		sprintf(tempNumForZi,"%d",BoxItemNum[0][3]);
		numForCount3->setString(tempNumForZi);
		numForCount3->setVisible(true);
	}

	auto detailXbox4=Sprite::createWithSpriteFrameName("zikuang.png");
	detailXbox4->setAnchorPoint(Vec2(0,0));
	detailXbox4->setPosition(Vec2(origin.x+visibleSize.width*0.906-30,origin.y+visibleSize.height*0.24));
	this->addChild(detailXbox4,2,CURMJ_ITEM_FOUR);
	auto fourthsSize=detailXbox4->getContentSize();

	auto fourthZi=Sprite::createWithSpriteFrameName("Cheats-fa.png");
	fourthZi->setAnchorPoint(Vec2(0.5,0.5));
	fourthZi->setPosition(Vec2(fourthsSize.width/2,fourthsSize.height/2));
	detailXbox4->addChild(fourthZi,1,ZI_PNG);

	auto numberOfDetailXbox4=Sprite::createWithSpriteFrameName("zishuliang.png");
	numberOfDetailXbox4->setAnchorPoint(Vec2(0.5,0.5));
	numberOfDetailXbox4->setPosition(Vec2(fourthsSize.width,fourthsSize.height));
	detailXbox4->addChild(numberOfDetailXbox4,2,NUM_ITEM_FOUR);
	numberOfDetailXbox4->setVisible(false);

	auto numForCount4=LabelTTF::create("","Arial",20);
	numForCount4->setAnchorPoint(Vec2(0.5,0.5));
	numForCount4->setPosition(Vec2(fourthsSize.width,fourthsSize.height));
	detailXbox4->addChild(numForCount4,2,ZI_NUM);
	numForCount4->setVisible(false);

	if(BoxItemNum[0][4]>0)
	{
		numberOfDetailXbox4->setVisible(true);
		sprintf(tempNumForZi,"%d",BoxItemNum[0][4]);
		numForCount4->setString(tempNumForZi);
		numForCount4->setVisible(true);
	}

	auto convertButton=Button::create("baoming anniu2.png","baoming anniu.png","baoming anniu.png",UI_TEX_TYPE_PLIST);
	convertButton->addTouchEventListener(CC_CALLBACK_2(MyXbox::convertCallback,this));
	auto sizeOfChange=convertButton->getContentSize();
	convertButton->setScale(0.8);
	convertButton->setPosition(Vec2((855+sizeOfChange.width/2)/1218*visibleSize.width+origin.x-30,(80+sizeOfChange.height/2)/716*visibleSize.height+origin.y));
	this->addChild(convertButton,2);

	auto imageOfLabelChange=Sprite::createWithSpriteFrameName("duihuan.png");
	auto sizeOfLabelChange=imageOfLabelChange->getContentSize();
	imageOfLabelChange->setRotation(-90);
	imageOfLabelChange->setPosition(((855+sizeOfChange.width/2)/1218)*visibleSize.width+origin.x-30,((80+sizeOfChange.height/2)/716)*visibleSize.height+origin.y);
	this->addChild(imageOfLabelChange,2);
	
	return true;
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
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}