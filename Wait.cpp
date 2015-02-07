#include "Wait.h"
#include "ShopScene.h"


Wait::Wait(void)
{
}


Wait::~Wait(void)
{
}

Scene* Wait::createScene()
{
	auto scene=Scene::create();
	auto waitLayer=Wait::create();
	scene->addChild(waitLayer);
	return scene;
}

bool Wait::init()
{
	if(!Layer::init())
	{
		return false;
	}

	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();
	float s_scale=1.189f;

	auto bg=Sprite::create("602.png");
	bg->setAnchorPoint(Vec2(0.5,0.5));
	bg->setScaleX(s_scale);
	bg->setScaleY(s_scale);
	bg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(bg,0);

	auto flower=Sprite::createWithSpriteFrameName("meihua.png");
	auto sizeOfFlower=flower->getContentSize();
	flower->setAnchorPoint(Vec2(1,0));
	flower->setPosition(Vec2(origin.x+visibleSize.width*1.0095,origin.y-visibleSize.height*0.028));
	this->addChild(flower,0);

	auto paddle=LayerColor::create(Color4B(47,86,110,100));
	paddle->setContentSize(Size(visibleSize.width,visibleSize.height*0.8533));
	paddle->setPosition(Vec2(origin.x,origin.y));
	this->addChild(paddle,1);

	auto marketButton=Button::create("10.png","10.png","10.png",TextureResType::LOCAL);
	marketButton->setAnchorPoint(Vec2(0,1));
	marketButton->setPosition(Vec2(origin.x+visibleSize.width*0.03,origin.y+visibleSize.height*0.978));
	marketButton->addTouchEventListener(CC_CALLBACK_2(Wait::onButtonMarket,this));
	this->addChild(marketButton,1);

	auto competeBar=Sprite::createWithSpriteFrameName("bisaijifentiao.png");
	competeBar->setAnchorPoint(Vec2(0,0));
	competeBar->setPosition(Vec2(origin.x+visibleSize.width*0.7413,origin.y+visibleSize.height*0.88));
	this->addChild(competeBar,1);

	auto imageOfCompetePoint=Sprite::createWithSpriteFrameName("bisaijifen.png");
	imageOfCompetePoint->setAnchorPoint(Vec2(0,0));
	imageOfCompetePoint->setPosition(Vec2(origin.x+visibleSize.width*0.75,origin.y+visibleSize.height*0.895));
	this->addChild(imageOfCompetePoint,2);

	auto competePoint=LabelAtlas::create("12345", "fonts/goumai_number3.png", 21, 27, '0');
	competePoint->setAnchorPoint(Vec2(1,0));
	competePoint->setPosition(Vec2(origin.x+visibleSize.width*0.96,origin.y+visibleSize.height*0.895));
	this->addChild(competePoint,2);

	auto rankFrame1=Sprite::create("bisaipaimingkuang.png");
	rankFrame1->setAnchorPoint(Vec2(0,0));
	rankFrame1->setPosition(Vec2(origin.x+visibleSize.width*0.06,origin.y+visibleSize.height*0.04));
	this->addChild(rankFrame1,2);

	auto rankFrame2=Sprite::create("bisaipaimingkuang.png");
	rankFrame2->setAnchorPoint(Vec2(1,0));
	rankFrame2->setPosition(Vec2(origin.x+visibleSize.width*0.94,origin.y+visibleSize.height*0.04));
	this->addChild(rankFrame2,2);

	auto countDownBar=Sprite::createWithSpriteFrameName("daojishi.png");
	countDownBar->setAnchorPoint(Vec2(0,0));
	countDownBar->setPosition(Vec2(origin.x+visibleSize.width*0.035,origin.y+visibleSize.height*0.08));
	this->addChild(countDownBar,3);

	auto imageOfCountDownBar=Sprite::createWithSpriteFrameName("yujidengdai.png");
	imageOfCountDownBar->setAnchorPoint(Vec2(0,1));
	imageOfCountDownBar->setRotation(-90);
	imageOfCountDownBar->setPosition(Vec2(origin.x+visibleSize.width*0.045,origin.y+visibleSize.height*0.105));
	this->addChild(imageOfCountDownBar,4);

	auto matchBar=Sprite::createWithSpriteFrameName("pipeihengtiao.png");
	matchBar->setAnchorPoint(Vec2(0,0));
	matchBar->setPosition(Vec2(origin.x+visibleSize.width*0.04,origin.y+visibleSize.height*0.62));
	this->addChild(matchBar,3);

	auto imageOfWaitForMatch=Sprite::createWithSpriteFrameName("dengdaipipei.png");
	imageOfWaitForMatch->setAnchorPoint(Vec2(0,0));
	imageOfWaitForMatch->setPosition(Vec2(origin.x+visibleSize.width*0.1,origin.y+visibleSize.height*0.65));
	this->addChild(imageOfWaitForMatch,4);

	auto rankBar=Sprite::createWithSpriteFrameName("pipeihengtiao.png");
	rankBar->setAnchorPoint(Vec2(0,0));
	rankBar->setPosition(Vec2(origin.x+visibleSize.width*0.534,origin.y+visibleSize.height*0.62));
	this->addChild(rankBar,3);

	auto imageOfRank=Sprite::createWithSpriteFrameName("bisaipaiming.png");
	imageOfRank->setAnchorPoint(Vec2(0,0));
	imageOfRank->setPosition(Vec2(origin.x+visibleSize.width*0.598,origin.y+visibleSize.height*0.65));
	this->addChild(imageOfRank,4);

	auto imageOfRanking=Sprite::createWithSpriteFrameName("mingci.png");
	imageOfRanking->setAnchorPoint(Vec2(0,0));
	imageOfRanking->setPosition(Vec2(origin.x+visibleSize.width*0.09,origin.y+visibleSize.height*0.57));
	this->addChild(imageOfRanking,3);

	auto imageOfRankOfCurrntTable=Sprite::createWithSpriteFrameName("benzhuopaiming.png");
	imageOfRankOfCurrntTable->setAnchorPoint(Vec2(0,0));
	imageOfRankOfCurrntTable->setPosition(Vec2(origin.x+visibleSize.width*0.09,origin.y+visibleSize.height*0.46));
	this->addChild(imageOfRankOfCurrntTable,3);

	auto imageOfRiseNumber=Sprite::createWithSpriteFrameName("jinjirenshu.png");
	imageOfRiseNumber->setAnchorPoint(Vec2(0,0));
	imageOfRiseNumber->setPosition(Vec2(origin.x+visibleSize.width*0.09,origin.y+visibleSize.height*0.35));
	this->addChild(imageOfRiseNumber,3);

	auto imageOfRemainNumber=Sprite::createWithSpriteFrameName("shengyuzhuoshu.png");
	imageOfRemainNumber->setAnchorPoint(Vec2(0,0));
	imageOfRemainNumber->setPosition(Vec2(origin.x+visibleSize.width*0.09,origin.y+visibleSize.height*0.24));
	this->addChild(imageOfRemainNumber,3);

	auto containerLayer=Layer::create();
	for(int i=0;i<100;i++)
	{
		char ranking[5]={0};
		sprintf(ranking,"%d",i+1);
		auto labelOfRanking=LabelTTF::create(ranking,"Arial",25);
		labelOfRanking->setAnchorPoint(Vec2(0,0));
		labelOfRanking->setPosition(Vec2(10,visibleSize.height*(4.95-i*0.05)));
		containerLayer->addChild(labelOfRanking);
	}
	auto rankSrollView=ui::ScrollView::create();
	rankSrollView->setSize(Size(400,400));
	rankSrollView->setBounceEnabled(true);
	rankSrollView->setDirection(ui::ScrollView::Direction::VERTICAL); 
	rankSrollView->setAnchorPoint(Vec2(1,0));
	rankSrollView->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.08));
	rankSrollView->setInnerContainerSize(Size(visibleSize.width,visibleSize.height*5));
	rankSrollView->addChild(containerLayer);
	this->addChild(rankSrollView,4);

	return true;
}

void Wait::onButtonMarket(Ref* pSender,Widget::TouchEventType type)
{
	auto shopScene=ShopScene::create();
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Director::getInstance()->pushScene(shopScene);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}