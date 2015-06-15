#include "RankingScroll.h"

RankingScroll::RankingScroll(void)
{
}
RankingScroll::~RankingScroll(void)
{
}
void RankingScroll::ScrollListCreate()
{
	for(int a=0;a<4;a++)
	{
		//滚动
		auto listScroll = ui::ScrollView::create();
		listScroll->setSize(Size(visibleSize.width*740/1218,visibleSize.height*375/716)); 
		listScroll->setInnerContainerSize(Size(visibleSize.width*740/1218,visibleSize.height*1880/716));
		listScroll->setPosition(Vec2(origin.x+visibleSize.width*224/1218,origin.y+visibleSize.height*90/716));
		listScroll->setDirection(ui::ScrollView::Direction::VERTICAL);    
		listScroll->setBounceEnabled(true);
		if(a==0)
			listScroll->setVisible(true);
		else
			listScroll->setVisible(false);
		this->addChild(listScroll,1,DAYIN_RANKING_SCROLL+a);
		//滚动层大小
		auto innerWidth = listScroll->getSize().width;
		auto innerHeight = listScroll->getSize().height;
		//滚动层初始化
		for (int i = 0; i < 20; i++)
		{
			auto spriteFirst=Sprite::createWithSpriteFrameName("liebiao.png");//条纹
			spriteFirst->setAnchorPoint(Vec2(1,1));
			spriteFirst->setPosition(Vec2(innerWidth,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*i+12));
			listScroll->addChild(spriteFirst,1);

			auto sprite1 = Sprite::createWithSpriteFrameName("shuzianwen.png");//数字暗纹
			sprite1->setAnchorPoint(Vec2(0,0));
			sprite1->setPosition(Vec2(0,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+6));
			listScroll->addChild(sprite1,2);

			if(i>=0&&i<3)
			{
				switch (i)//第1,2,3图
				{
				case 0:
					{
						auto spriteNUM1=Sprite::createWithSpriteFrameName("1.png");//1
						spriteNUM1->setAnchorPoint(Vec2(0.5,0));
						spriteNUM1->setPosition(Vec2(50,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
						listScroll->addChild(spriteNUM1,3);

						auto crown1=Sprite::createWithSpriteFrameName("guanjun.png");//冠军
						crown1->setAnchorPoint(Vec2::ZERO);
						crown1->setPosition(Vec2(innerWidth*608/740,listScroll->getInnerContainerSize().height-visibleSize.height*85/716+4));
						listScroll->addChild(crown1,2);		
						break;
					}
				case 1:
					{
						auto spriteNUM2=Sprite::createWithSpriteFrameName("2.png");//2
						spriteNUM2->setAnchorPoint(Vec2(0.5,0));
						spriteNUM2->setPosition(Vec2(46,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
						listScroll->addChild(spriteNUM2,3);

						auto crown2=Sprite::createWithSpriteFrameName("yajun.png");//亚军	
						crown2->setAnchorPoint(Vec2::ZERO);
						crown2->setPosition(Vec2(innerWidth*608/740+7,listScroll->getInnerContainerSize().height-visibleSize.height*175/716+6));
						listScroll->addChild(crown2,2);
						break;
					}
				case 2:
					{
						auto spriteNUM3=Sprite::createWithSpriteFrameName("3.png");//3
						spriteNUM3->setAnchorPoint(Vec2(0.5,0));
						spriteNUM3->setPosition(Vec2(46,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
						listScroll->addChild(spriteNUM3,3);

						auto crown3=Sprite::createWithSpriteFrameName("jijun.png");//季军
						crown3->setAnchorPoint(Vec2::ZERO);
						crown3->setPosition(Vec2(innerWidth*608/740+12,listScroll->getInnerContainerSize().height-visibleSize.height*265/716+8));
						listScroll->addChild(crown3,2);
						break;
					}
				default:
					break;
				}
			}
			else
			{
				char curRankNo[5];
				std::string RangingNo;
				sprintf(curRankNo,"%d",i+1);
				RangingNo=curRankNo;

				auto RankingNo=LabelAtlas::create(RangingNo,"fonts/paimingshuzi.png",44.6,60,'0');
				RankingNo->setAnchorPoint(Vec2(0.5,0));
				RankingNo->setPosition(Vec2(46,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
				listScroll->addChild(RankingNo,3);
			}

			auto Icon1=Sprite::createWithSpriteFrameName("812.png");//玩家头像
			Icon1->setScale(82/Icon1->getContentSize().width,82/Icon1->getContentSize().height);
			Icon1->setAnchorPoint(Vec2(0,0));
			Icon1->setPosition(Vec2(innerWidth*109/740,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+6));
			listScroll->addChild(Icon1,2);	
		}
	}
}
bool RankingScroll::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize=Director::getInstance()->getVisibleSize();//大小和原点
	origin=Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("denglupaihang.plist");

	auto InkScroll=Sprite::createWithSpriteFrameName("juanzhou.png");//卷轴
	InkScroll->setAnchorPoint(Vec2(0.5,0.5));
	InkScroll->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-33));
	this->addChild(InkScroll,0);

	//上部
	auto listTitle=Sprite::createWithSpriteFrameName("fengyunpaihangbang.png");//风云排行榜字体
	listTitle->setAnchorPoint(Vec2(0,1));
	listTitle->setPosition(Vec2(origin.x+visibleSize.width*407/1218,origin.y+visibleSize.height*628/716));
	this->addChild(listTitle,2);

	auto mojiTitle=Sprite::createWithSpriteFrameName("biaoti moji.png");//墨迹
	mojiTitle->setAnchorPoint(Vec2(0,0));
	mojiTitle->setPosition(Vec2(origin.x+visibleSize.width*365/1218,origin.y+visibleSize.height*533/716));
	this->addChild(mojiTitle,1);
	
	auto huahua=Sprite::createWithSpriteFrameName("hua.png");//花花
	huahua->setAnchorPoint(Vec2(0.5,0.5));
	huahua->setPosition(Vec2(origin.x+visibleSize.width*955/1218,origin.y+visibleSize.height*525/716));
	this->addChild(huahua,1);

	//日盈利--按钮
	auto dayin=Button::create("riyingli2.png","riyingli.png","riyingli2.png",UI_TEX_TYPE_PLIST);//字
	dayin->setTouchEnabled(false);
	dayin->setHighlighted(true);
	curListNo=0;
	dayin->addTouchEventListener(CC_CALLBACK_2(RankingScroll::dayInEvent,this));
	dayin->setAnchorPoint(Vec2(0,0));
	dayin->setPosition(Vec2(origin.x+visibleSize.width*235/1218,origin.y+visibleSize.height*465/716));
	this->addChild(dayin,2,DAYIN_RANKBUTTON);

	//财富榜--按钮
	auto money=Button::create("caifubang2.png","caifubang.png","caifubang2.png",UI_TEX_TYPE_PLIST);
	money->setTouchEnabled(true);
	money->addTouchEventListener(CC_CALLBACK_2(RankingScroll::moneyEvent,this));
	money->setAnchorPoint(Vec2(0,0));
	money->setPosition(Vec2(origin.x+visibleSize.width*415/1218,origin.y+visibleSize.height*465/716));
	this->addChild(money,2,ACCOUNT_RANKBUTTON);

	//最大番型--按钮
	auto maxtimes=Button::create("zuidafanxing2.png","zuidafanxing.png","zuidafanxing2.png",UI_TEX_TYPE_PLIST);
	maxtimes->setTouchEnabled(true);
	maxtimes->addTouchEventListener(CC_CALLBACK_2(RankingScroll::maxtimesEvent,this));
	maxtimes->setAnchorPoint(Vec2(0,0));
	maxtimes->setPosition(Vec2(origin.x+visibleSize.width*595/1218,origin.y+visibleSize.height*465/716));
	this->addChild(maxtimes,2,MAXTIME_RANKBUTTON);

	//战绩排名--按钮	
	auto record=Button::create("zhanjipaiming2.png","zhanjipaiming.png","zhanjipaiming2.png",UI_TEX_TYPE_PLIST);
	record->setTouchEnabled(true);
	record->addTouchEventListener(CC_CALLBACK_2(RankingScroll::recordEvent,this));
	record->setAnchorPoint(Vec2(0,0));
	record->setPosition(Vec2(origin.x+visibleSize.width*775/1218,origin.y+visibleSize.height*465/716));
	this->addChild(record,2,JIFEN_RANKBUTTON);
	
	ScrollListCreate();

	return true;
}

void RankingScroll::dayInEvent(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		{
			auto curButton=(Button*)pSender;
			auto curList=this->getChildByTag(DAYIN_RANKING_SCROLL);
			auto LastButton=(Button*)this->getChildByTag(DAYIN_RANKBUTTON+curListNo);
			auto LastList=this->getChildByTag(DAYIN_RANKING_SCROLL+curListNo);
			LastButton->setHighlighted(false);
			LastButton->setTouchEnabled(true);
			curButton->setTouchEnabled(false);
			LastList->setVisible(false);
			curList->setVisible(true);
			curButton->setHighlighted(true);
			curListNo=0;
		}
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void RankingScroll::moneyEvent(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curButton=(Button*)pSender;
			auto curList=this->getChildByTag(ACCOUNT_RANKING_SCROLL);
			auto LastButton=(Button*)this->getChildByTag(DAYIN_RANKBUTTON+curListNo);
			auto LastList=this->getChildByTag(DAYIN_RANKING_SCROLL+curListNo);
			LastButton->setHighlighted(false);
			LastButton->setTouchEnabled(true);
			curButton->setTouchEnabled(false);
			LastList->setVisible(false);
			curList->setVisible(true);
			curButton->setHighlighted(true);
			curListNo=1;
		}
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

void RankingScroll::maxtimesEvent(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curButton=(Button*)pSender;
			auto curList=this->getChildByTag(MAXTIME_RANKING_SCROLL);
			auto LastButton=(Button*)this->getChildByTag(DAYIN_RANKBUTTON+curListNo);
			auto LastList=this->getChildByTag(DAYIN_RANKING_SCROLL+curListNo);
			LastButton->setHighlighted(false);
			LastButton->setTouchEnabled(true);
			curButton->setTouchEnabled(false);
			LastList->setVisible(false);
			curList->setVisible(true);
			curButton->setHighlighted(true);
			curListNo=2;
		}
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


void RankingScroll::recordEvent(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curButton=(Button*)pSender;
			auto curList=this->getChildByTag(RECROLD_RANKING_SCROLL);
			auto LastButton=(Button*)this->getChildByTag(DAYIN_RANKBUTTON+curListNo);
			auto LastList=this->getChildByTag(DAYIN_RANKING_SCROLL+curListNo);
			LastButton->setHighlighted(false);
			LastButton->setTouchEnabled(true);
			curButton->setTouchEnabled(false);
			LastList->setVisible(false);
			curList->setVisible(true);
			curButton->setHighlighted(true);
			curListNo=3;
		}
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