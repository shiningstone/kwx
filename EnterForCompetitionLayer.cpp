#include "EnterForCompetitionLayer.h"
#include "CompetitionField.h"


EnterForCompetitionLayer::EnterForCompetitionLayer(Node* parent)
{
	parentScene=(CompetitionField*)parent;
	init();
}


EnterForCompetitionLayer::~EnterForCompetitionLayer(void)
{
}

bool EnterForCompetitionLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto paddle=LayerColor::create(Color4B(0,0,0,200));
	paddle->setContentSize(Size(visibleSize.width,visibleSize.height));
	paddle->setAnchorPoint(Vec2(0,0));
	paddle->setPosition(Vec2(origin.x,origin.y));

	auto sc=ui::ScrollView::create();
	sc->setSize(visibleSize);
	sc->addChild(paddle);
	this->addChild(sc,0);

	auto bg=Sprite::createWithSpriteFrameName("bisaikuang.png");
	bg->setAnchorPoint(Vec2(0.5,0.5));
	bg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(bg,1);

	auto titleBg=Sprite::createWithSpriteFrameName("biaoti moji.png");
	titleBg->setAnchorPoint(Vec2(0.5,0));
	titleBg->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.77));
	this->addChild(titleBg,2);

	auto title=Sprite::createWithSpriteFrameName("baomingbisai.png");
	title->setAnchorPoint(Vec2(0.5,0));
	title->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.8));
	this->addChild(title,2);

	auto imageOfChampion=Sprite::createWithSpriteFrameName("guanjun.png");
	imageOfChampion->setAnchorPoint(Vec2(0,0));
	imageOfChampion->setScaleX(0.78);
	imageOfChampion->setScaleY(0.65);
	imageOfChampion->setPosition(Vec2(origin.x+visibleSize.width*0.235,origin.y+visibleSize.height*0.65));
	this->addChild(imageOfChampion,2);

	auto imageOfSecond=Sprite::createWithSpriteFrameName("yajun.png");
	imageOfSecond->setAnchorPoint(Vec2(0,0));
	imageOfSecond->setScaleX(0.9);
	imageOfSecond->setScaleY(0.76);
	imageOfSecond->setPosition(Vec2(origin.x+visibleSize.width*0.418,origin.y+visibleSize.height*0.65));
	this->addChild(imageOfSecond,2);

	auto imageOfThird=Sprite::createWithSpriteFrameName("jijun.png");
	imageOfThird->setAnchorPoint(Vec2(0,0));
	imageOfThird->setPosition(Vec2(origin.x+visibleSize.width*0.6,origin.y+visibleSize.height*0.65));
	this->addChild(imageOfThird,2);

	auto labelOfChampion=LabelTTF::create("冠军","Arial",35);
	labelOfChampion->setAnchorPoint(Vec2(0,0));
	labelOfChampion->setColor(Color3B(210,69,0));
	labelOfChampion->setPosition(Vec2(origin.x+visibleSize.width*0.32,origin.y+visibleSize.height*0.65));
	this->addChild(labelOfChampion,2);

	auto labelOfSecond=LabelTTF::create("亚军","Arial",35);
	labelOfSecond->setAnchorPoint(Vec2(0,0));
	labelOfSecond->setColor(Color3B(255,69,0));
	labelOfSecond->setPosition(Vec2(origin.x+visibleSize.width*0.5045,origin.y+visibleSize.height*0.65));
	this->addChild(labelOfSecond,2);

	auto labelOfThird=LabelTTF::create("季军","Arial",35);
	labelOfThird->setAnchorPoint(Vec2(0,0));
	labelOfThird->setColor(Color3B(255,69,0));
	labelOfThird->setPosition(Vec2(origin.x+visibleSize.width*0.689,origin.y+visibleSize.height*0.65));
	this->addChild(labelOfThird,2);

	char reward[5]={0};
	for(int i=0;i<3;i++)
	{
		auto labelOfReward=LabelTTF::create("奖励:","Arial",30);
		labelOfReward->setAnchorPoint(Vec2(1,0));
		labelOfReward->setColor(Color3B(128,128,0));
		labelOfReward->setPosition(Vec2(origin.x+visibleSize.width*(0.3+0.1845*i),origin.y+visibleSize.height*0.6));
		this->addChild(labelOfReward,2);

		sprintf(reward,"%d",6000-2000*i);
		auto labelImage13=LabelTTF::create(reward,"Arial",30);
		labelImage13->setAnchorPoint(Vec2(0,0));
		labelImage13->setColor(Color3B(128,128,0));
		labelImage13->setPosition(Vec2(origin.x+visibleSize.width*(0.3+0.1845*i),origin.y+visibleSize.height*0.6));
		this->addChild(labelImage13,2);
	}

	auto labelOfSignUp=LabelTTF::create("报名费:","Arial",30);
	labelOfSignUp->setAnchorPoint(Vec2(0,0));
	labelOfSignUp->setColor(Color3B(128,128,0));
	labelOfSignUp->setPosition(Vec2(origin.x+visibleSize.width*0.235,origin.y+visibleSize.height*0.415));
	this->addChild(labelOfSignUp,2);

	auto labelOfCost=LabelTTF::create("2000","Arial",35);
	labelOfCost->setAnchorPoint(Vec2(0,0));
	labelOfCost->setColor(Color3B(128,128,0));
	labelOfCost->setPosition(Vec2(origin.x+visibleSize.width*0.32,origin.y+visibleSize.height*0.415));
	this->addChild(labelOfCost,2);

	auto labelOfService=LabelTTF::create("服务费:","Arial",35);
	labelOfService->setAnchorPoint(Vec2(1,0));
	labelOfService->setColor(Color3B(128,128,0));
	labelOfService->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.415));
	this->addChild(labelOfService,2);

	auto labelOfServiceCost=LabelTTF::create("200","Arial",35);
	labelOfServiceCost->setAnchorPoint(Vec2(0,0));
	labelOfServiceCost->setColor(Color3B(128,128,0));
	labelOfServiceCost->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.415));
	this->addChild(labelOfServiceCost,2);

	auto gold=Sprite::createWithSpriteFrameName("jinbi.png");
	gold->setAnchorPoint(Vec2(0,0));
	gold->setScaleY(0.7);
	gold->setScaleX(0.8);
	gold->setPosition(Vec2(origin.x+visibleSize.width*0.6,origin.y+visibleSize.height*0.415));
	this->addChild(gold,2);

	auto labelOfGold=LabelTTF::create("999999","Arial",30);
	labelOfGold->setAnchorPoint(Vec2(0,0));
	labelOfGold->setColor(Color3B(128,128,0));
	labelOfGold->setPosition(Vec2(origin.x+visibleSize.width*0.65,origin.y+visibleSize.height*0.415));
	this->addChild(labelOfGold,2);

	auto labelOfSignedUp=LabelTTF::create("已报名:","Arial",25);
	labelOfSignedUp->setAnchorPoint(Vec2(1,0));
	labelOfSignedUp->setColor(Color3B(255,69,0));
	labelOfSignedUp->setPosition(Vec2(origin.x+visibleSize.width*0.48,origin.y+visibleSize.height*0.35));
	this->addChild(labelOfSignedUp,2);

	auto labelOfPeople=LabelTTF::create("人 ","Arial",25);
	labelOfPeople->setAnchorPoint(Vec2(0,0));
	labelOfPeople->setColor(Color3B(255,69,0));
	labelOfPeople->setPosition(Vec2(origin.x+visibleSize.width*0.52,origin.y+visibleSize.height*0.35));
	this->addChild(labelOfPeople,2);
	

	auto progressBg=Sprite::createWithSpriteFrameName("jindutiao.png");
	progressBg->setAnchorPoint(Vec2(0.5,0));
	progressBg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.27));
	this->addChild(progressBg,2);

	for(int i=0;i<24;i++)
	{
		auto progressDownLayer=Sprite::createWithSpriteFrameName("weibaoming.png");
		progressDownLayer->setAnchorPoint(Vec2(0,0));
		progressDownLayer->setPosition(Vec2(origin.x+visibleSize.width*(0.239+0.0215*i),origin.y+visibleSize.height*0.285));
		this->addChild(progressDownLayer,3);
	}

	srand(time(0));
	int j=rand()%24;
	for(int i=0;i<j;i++)
	{
		auto progressUpLayer=Sprite::createWithSpriteFrameName("yibaoming.png");
		progressUpLayer->setAnchorPoint(Vec2(0,0));
		progressUpLayer->setPosition(Vec2(origin.x+visibleSize.width*(0.239+0.0215*i),origin.y+visibleSize.height*0.285));
		this->addChild(progressUpLayer,3);
	}

	auto buttonQuit=Button::create("quxiaoanniu.png","quxiaoanniu2.png","quxiaoanniu2.png",TextureResType::PLIST);
	buttonQuit->setAnchorPoint(Vec2(0,0));
	buttonQuit->setPosition(Vec2(origin.x+visibleSize.width*0.3,origin.y+visibleSize.height*0.125));
	buttonQuit->addTouchEventListener(CC_CALLBACK_2(EnterForCompetitionLayer::onButtonQuit,this));
	this->addChild(buttonQuit,2,1);

	auto imageOfButtonQuit=Sprite::createWithSpriteFrameName("quxiao.png");
	imageOfButtonQuit->setAnchorPoint(Vec2(0,0));
	imageOfButtonQuit->setPosition(Vec2(origin.x+visibleSize.width*0.335,origin.y+visibleSize.height*0.14));
	this->addChild(imageOfButtonQuit,3);
	
	auto buttonSignUp=Button::create("baoming anniu2.png","baoming anniu.png","baoming anniu.png",TextureResType::PLIST);
	buttonSignUp->setAnchorPoint(Vec2(1,0));
	buttonSignUp->setPosition(Vec2(origin.x+visibleSize.width*0.7,origin.y+visibleSize.height*0.125));
	buttonSignUp->addTouchEventListener(CC_CALLBACK_2(EnterForCompetitionLayer::onButtonSignUp,this));
	this->addChild(buttonSignUp,2,2);

	auto imageOfSignUp=Sprite::createWithSpriteFrameName("baoming.png");
	imageOfSignUp->setAnchorPoint(Vec2(1,0));
	imageOfSignUp->setPosition(Vec2(origin.x+visibleSize.width*0.665,origin.y+visibleSize.height*0.14));
	this->addChild(imageOfSignUp,3);

	
	return true;
}

void EnterForCompetitionLayer::onButtonQuit(Ref* pSender,Widget::TouchEventType type)
{
	//auto competitionField=CompetitionField::create();
	//auto layer=this->getParent();
	auto buttonQuit=(Button*)this->getChildByTag(1);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		buttonQuit->setHighlighted(true);
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		//layer->removeChildByTag();
		//Director::getInstance()->replaceScene(competitionField);
		parentScene->removeChildByTag(BAO_MING_COMPETE_LAYER);
		buttonQuit->setHighlighted(false);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}
	
void EnterForCompetitionLayer::onButtonSignUp(Ref* pSender,Widget::TouchEventType type)
{
	auto buttonSignUp=(Button*)this->getChildByTag(2);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		buttonSignUp->setHighlighted(true);
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		buttonSignUp->setHighlighted(false);
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}