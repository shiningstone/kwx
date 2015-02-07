#include "MyXbox.h"
MyXbox::MyXbox(void)
{
}


MyXbox::~MyXbox(void)
{
}

bool MyXbox::init()
{
	if(!Layer::init())
	{
		return false;
	}
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto sc=ui::ScrollView::create();
	sc->setSize(Size(633,537));
	sc->setBounceEnabled(true);
	sc->setAnchorPoint(Vec2(0,0));
	sc->setInnerContainerSize(Size(visibleSize.width*0.5197,visibleSize.height*2.5));
	sc->setPosition(Vec2(origin.x+visibleSize.width*0.03,origin.y+visibleSize.height*0.09));
	this->addChild(sc,1);

	for(int i=0;i<10;i++)
	{
		auto buttonXbox1=Button::create("mijikuang.png","mijikuang.png","mijikuang.png",TextureResType::PLIST);
		buttonXbox1->setAnchorPoint(Vec2(0,1));
		buttonXbox1->setPosition(Vec2(origin.x+visibleSize.width*0.01,origin.y+visibleSize.height*(2.465-0.25*i)));
		sc->addChild(buttonXbox1,1);

		auto buttonXbox2=Button::create("mijikuang.png","mijikuang.png","mijikuang.png",TextureResType::PLIST);
		buttonXbox2->setAnchorPoint(Vec2(0,1));
		buttonXbox2->setPosition(Vec2(origin.x+visibleSize.width*0.1836,origin.y+visibleSize.height*(2.465-0.25*i)));
		sc->addChild(buttonXbox2,1);

		auto buttonXbox3=Button::create("mijikuang.png","mijikuang.png","mijikuang.png",TextureResType::PLIST);
		buttonXbox3->setAnchorPoint(Vec2(0,1));
		buttonXbox3->setPosition(Vec2(origin.x+visibleSize.width*0.3572,origin.y+visibleSize.height*(2.465-0.25*i)));
		sc->addChild(buttonXbox3,1);
		
		auto imageOfSelectEffect1=Sprite::createWithSpriteFrameName("dianjixiaoguo.png");
		imageOfSelectEffect1->setAnchorPoint(Vec2(0,1));
		imageOfSelectEffect1->setPosition(Vec2(origin.x+visibleSize.width*0.002,origin.y+visibleSize.height*(2.481-0.25*i)));
		sc->addChild(imageOfSelectEffect1,2);
		imageOfSelectEffect1->setScale(0.98);

		auto imageOfSelectEffect2=Sprite::createWithSpriteFrameName("dianjixiaoguo.png");
		imageOfSelectEffect2->setAnchorPoint(Vec2(0,1));
		imageOfSelectEffect2->setPosition(Vec2(origin.x+visibleSize.width*0.175,origin.y+visibleSize.height*(2.481-0.25*i)));
		sc->addChild(imageOfSelectEffect2,2);
		imageOfSelectEffect2->setScale(0.98);

		auto imageOfSelectEffect3=Sprite::createWithSpriteFrameName("dianjixiaoguo.png");
		imageOfSelectEffect3->setAnchorPoint(Vec2(0,1));
		imageOfSelectEffect3->setPosition(Vec2(origin.x+visibleSize.width*0.348,origin.y+visibleSize.height*(2.481-0.25*i)));
		sc->addChild(imageOfSelectEffect3,2);
		imageOfSelectEffect3->setScale(0.98);

		if(i==1)
		{
			imageOfSelectEffect1->setVisible(false);
			imageOfSelectEffect2->setVisible(true);
			imageOfSelectEffect3->setVisible(false);
		}
		else
		{
			imageOfSelectEffect1->setVisible(false);
			imageOfSelectEffect2->setVisible(false);
			imageOfSelectEffect3->setVisible(false);
		}

		auto imageOfnumber1=Sprite::createWithSpriteFrameName("mijishuliang.png");
		imageOfnumber1->setAnchorPoint(Vec2(0,0.5));
		imageOfnumber1->setPosition(Vec2(origin.x+visibleSize.width*0.110,origin.y+visibleSize.height*(2.45-0.25*i)));
		sc->addChild(imageOfnumber1,2);

		auto imageOfnumber2=Sprite::createWithSpriteFrameName("mijishuliang.png");
		imageOfnumber2->setAnchorPoint(Vec2(0,0.5));
		imageOfnumber2->setPosition(Vec2(origin.x+visibleSize.width*0.283,origin.y+visibleSize.height*(2.45-0.25*i)));
		sc->addChild(imageOfnumber2,2);

		auto imageOfnumber3=Sprite::createWithSpriteFrameName("mijishuliang.png");
		imageOfnumber3->setAnchorPoint(Vec2(0,0.5));
		imageOfnumber3->setPosition(Vec2(origin.x+visibleSize.width*0.456,origin.y+visibleSize.height*(2.45-0.25*i)));
		sc->addChild(imageOfnumber3,2);
	}
	
	auto detailInfo=Sprite::createWithSpriteFrameName("beibaoxiaobeijing.png");
	auto sizeOfDetailInfo=detailInfo->getContentSize();
	detailInfo->setAnchorPoint(Vec2(0,0));
	detailInfo->setPosition(Vec2(origin.x+visibleSize.width*0.5878,origin.y+visibleSize.height*0.088));
	this->addChild(detailInfo,1);

	auto labelOfDetailInfo=LabelTTF::create("详细信息","Arial",30);
	labelOfDetailInfo->setAnchorPoint(Vec2(0,0));
	auto labelOfInfoID=labelOfDetailInfo->getContentSize();
	labelOfDetailInfo->setPosition(Vec2(origin.x+visibleSize.width*0.74,origin.y+visibleSize.height*0.775));
	this->addChild(labelOfDetailInfo,2);

	auto imageOfSelectedXbox=Sprite::createWithSpriteFrameName("mijikuang.png");
	imageOfSelectedXbox->setAnchorPoint(Vec2(0,0));
	imageOfSelectedXbox->setPosition(origin.x+visibleSize.width*0.6,origin.y+visibleSize.height*0.53);
	this->addChild(imageOfSelectedXbox,2);


	auto detailXbox1=Sprite::createWithSpriteFrameName("zikuang.png");
	detailXbox1->setAnchorPoint(Vec2(0,0));
	detailXbox1->setPosition(Vec2(origin.x+visibleSize.width*0.6,origin.y+visibleSize.height*0.24));
	this->addChild(detailXbox1,2);

	auto detailXbox2=Sprite::createWithSpriteFrameName("zikuang.png");
	detailXbox2->setAnchorPoint(Vec2(0,0));
	detailXbox2->setPosition(Vec2(origin.x+visibleSize.width*0.702,origin.y+visibleSize.height*0.24));
	this->addChild(detailXbox2,2);

	auto detailXbox3=Sprite::createWithSpriteFrameName("zikuang.png");
	detailXbox3->setAnchorPoint(Vec2(0,0));
	detailXbox3->setPosition(Vec2(origin.x+visibleSize.width*0.804,origin.y+visibleSize.height*0.24));
	this->addChild(detailXbox3,2);

	auto detailXbox4=Sprite::createWithSpriteFrameName("zikuang.png");
	detailXbox4->setAnchorPoint(Vec2(0,0));
	detailXbox4->setPosition(Vec2(origin.x+visibleSize.width*0.906,origin.y+visibleSize.height*0.24));
	this->addChild(detailXbox4,2);

	//*************************************
	auto numberOfDetailXbox1=Sprite::createWithSpriteFrameName("zishuliang.png");
	auto sizeOfNumberOfDetail=numberOfDetailXbox1->getContentSize();
	numberOfDetailXbox1->setPosition(((806+sizeOfNumberOfDetail.width/2)/1218)*visibleSize.width+origin.x,((232+sizeOfNumberOfDetail.height/2)/716)*visibleSize.height+origin.y);
	this->addChild(numberOfDetailXbox1,2);

	auto numberOfDetailXbox2=Sprite::createWithSpriteFrameName("zishuliang.png");
	numberOfDetailXbox2->setPosition(((930+sizeOfNumberOfDetail.width/2)/1218)*visibleSize.width+origin.x,((232+sizeOfNumberOfDetail.height/2)/716)*visibleSize.height+origin.y);
	this->addChild(numberOfDetailXbox2,2);
	
	auto numberOfDetailXbox3=Sprite::createWithSpriteFrameName("zishuliang.png");
	numberOfDetailXbox3->setPosition(((1040+sizeOfNumberOfDetail.width/2)/1218)*visibleSize.width+origin.x,((232+sizeOfNumberOfDetail.height/2)/716)*visibleSize.height+origin.y);
	this->addChild(numberOfDetailXbox3,2);

	auto numberOfDetailXbox4=Sprite::createWithSpriteFrameName("zishuliang.png");
	numberOfDetailXbox4->setPosition(((1162+sizeOfNumberOfDetail.width/2)/1218)*visibleSize.width+origin.x,((232+sizeOfNumberOfDetail.height/2)/716)*visibleSize.height+origin.y);
	this->addChild(numberOfDetailXbox4,2);

	auto imageChange1=Sprite::createWithSpriteFrameName("baoming anniu2.png");
	auto imageChange2=Sprite::createWithSpriteFrameName("baoming anniu.png");
	auto itemOfChange=MenuItemSprite::create(imageChange1,imageChange2,CC_CALLBACK_1(MyXbox::menuCloseCallback, this));
	auto sizeOfChange=itemOfChange->getContentSize();
	itemOfChange->setScale(0.8);
	auto ButtonOfChange=Menu::create(itemOfChange,NULL);
	ButtonOfChange->setPosition(((855+sizeOfChange.width/2)/1218)*visibleSize.width+origin.x,((80+sizeOfChange.height/2)/716)*visibleSize.height+origin.y);
	this->addChild(ButtonOfChange,2);

	auto imageOfLabelChange=Sprite::createWithSpriteFrameName("duihuan.png");
	auto sizeOfLabelChange=imageOfLabelChange->getContentSize();
	imageOfLabelChange->setRotation(-90);
	imageOfLabelChange->setPosition(((855+sizeOfChange.width/2)/1218)*visibleSize.width+origin.x,((80+sizeOfChange.height/2)/716)*visibleSize.height+origin.y);
	this->addChild(imageOfLabelChange,2);
	
	return true;
}

void MyXbox::menuCloseCallback(Ref* pSender)
{
	
}