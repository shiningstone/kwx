#include "RankingScroll.h"

RankingScroll::RankingScroll(void)
{
	dayInList=EnvVariable::getInstance()->get_RaningList_DayGain();
	moneyList=EnvVariable::getInstance()->get_RaningList_Riches();
	recordList=EnvVariable::getInstance()->get_RaningList_Record();
	maxtimeList=EnvVariable::getInstance()->get_RaningList_MaxType();
}
RankingScroll::~RankingScroll(void)
{
}
void RankingScroll::ScrollListCreate(std::vector <TheCharts> curList,int curTag)
{
	auto listScroll=ui::ScrollView::create();
	listScroll->setSize(Size(visibleSize.width*740/1218,visibleSize.height*375/716)); 
	listScroll->setInnerContainerSize(Size(visibleSize.width*740/1218,visibleSize.height*1880/716));
	listScroll->setPosition(Vec2(origin.x+visibleSize.width*224/1218,origin.y+visibleSize.height*90/716));
	listScroll->setDirection(ui::ScrollView::Direction::VERTICAL);    
	listScroll->setBounceEnabled(true);
	if(curTag!=DAYIN_RANKING_SCROLL)
		listScroll->setVisible(false);
	this->addChild(listScroll,2,curTag);

	auto innerWidth = listScroll->getSize().width;
	auto innerHeight = listScroll->getSize().height;

	//滚动层初始化
	for (int i = 0; i < curList.size(); i++)
	{
		char tempChar[20];

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
			sprintf(tempChar,"%d",i+1);
			auto RankingNo=LabelAtlas::create(tempChar,"fonts/paimingshuzi.png",45,60,'0');
			RankingNo->setAnchorPoint(Vec2(0.5,0));
			RankingNo->setPosition(Vec2(46,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
			listScroll->addChild(RankingNo,3);
		}

		auto Icon1=Sprite::createWithSpriteFrameName(curList[i].photoFileName);//玩家头像
		Icon1->setScale(82/Icon1->getContentSize().width,82/Icon1->getContentSize().height);
		Icon1->setAnchorPoint(Vec2(0,0));
		Icon1->setPosition(Vec2(innerWidth*109/740,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+6));
		listScroll->addChild(Icon1,2);
		auto PhotoSize=Icon1->getContentSize();

		auto curNikeNmae=LabelTTF::create(curList[i].NikeName,"Arial",20);
		curNikeNmae->setAnchorPoint(Vec2(0,0.5));
		curNikeNmae->setPosition(Vec2(Icon1->getPosition().x+PhotoSize.width*Icon1->getScaleX()*1.15,Icon1->getPosition().y+PhotoSize.height/2*Icon1->getScaleY()));
		listScroll->addChild(curNikeNmae,2);

		sprintf(tempChar,"%d",curList[i].RankingNum);
		auto curRankNum=LabelTTF::create(tempChar,"Arial",20);
		curRankNum->setAnchorPoint(Vec2(1,0.5));
		curRankNum->setPosition(Vec2(innerWidth*600/740,Icon1->getPosition().y+PhotoSize.height/2*Icon1->getScaleY()));
		listScroll->addChild(curRankNum,2);

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
	dayin->addTouchEventListener(CC_CALLBACK_2(RankingScroll::litBtnCallBack,this));
	dayin->setAnchorPoint(Vec2(0,0));
	dayin->setPosition(Vec2(origin.x+visibleSize.width*235/1218,origin.y+visibleSize.height*465/716));
	this->addChild(dayin,2,DAYIN_RANKBUTTON);

	//财富榜--按钮
	auto money=Button::create("caifubang2.png","caifubang.png","caifubang2.png",UI_TEX_TYPE_PLIST);
	money->setTouchEnabled(true);
	money->addTouchEventListener(CC_CALLBACK_2(RankingScroll::litBtnCallBack,this));
	money->setAnchorPoint(Vec2(0,0));
	money->setPosition(Vec2(origin.x+visibleSize.width*415/1218,origin.y+visibleSize.height*465/716));
	this->addChild(money,2,ACCOUNT_RANKBUTTON);

	//最大番型--按钮
	auto maxtimes=Button::create("zuidafanxing2.png","zuidafanxing.png","zuidafanxing2.png",UI_TEX_TYPE_PLIST);
	maxtimes->setTouchEnabled(true);
	maxtimes->addTouchEventListener(CC_CALLBACK_2(RankingScroll::litBtnCallBack,this));
	maxtimes->setAnchorPoint(Vec2(0,0));
	maxtimes->setPosition(Vec2(origin.x+visibleSize.width*595/1218,origin.y+visibleSize.height*465/716));
	this->addChild(maxtimes,2,MAXTIME_RANKBUTTON);

	//战绩排名--按钮	
	auto record=Button::create("zhanjipaiming2.png","zhanjipaiming.png","zhanjipaiming2.png",UI_TEX_TYPE_PLIST);
	record->setTouchEnabled(true);
	record->addTouchEventListener(CC_CALLBACK_2(RankingScroll::litBtnCallBack,this));
	record->setAnchorPoint(Vec2(0,0));
	record->setPosition(Vec2(origin.x+visibleSize.width*775/1218,origin.y+visibleSize.height*465/716));
	this->addChild(record,2,JIFEN_RANKBUTTON);

	ScrollListCreate(dayInList,DAYIN_RANKING_SCROLL);
	ScrollListCreate(moneyList,ACCOUNT_RANKING_SCROLL);
	ScrollListCreate(recordList,MAXTIME_RANKING_SCROLL);
	ScrollListCreate(maxtimeList,RECROLD_RANKING_SCROLL);

	return true;
}
void RankingScroll::litBtnCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curBtn=(Button*)pSender;
			auto curTag=curBtn->getTag();
			auto btn1=(Button*)this->getChildByTag(curTag%4+1);
			auto btn2=(Button*)this->getChildByTag((curTag+1)%4+1);
			auto btn3=(Button*)this->getChildByTag((curTag+2)%4+1);

			auto curList=(ui::ScrollView*)this->getChildByTag(curTag+4);
			auto list1=(ui::ScrollView*)this->getChildByTag(curTag%4+5);
			auto list2=(ui::ScrollView*)this->getChildByTag((curTag+1)%4+5);
			auto list3=(ui::ScrollView*)this->getChildByTag((curTag+2)%4+5);

			curBtn->setTouchEnabled(false);
			btn1->setTouchEnabled(true);
			btn2->setTouchEnabled(true);
			btn3->setTouchEnabled(true);
			curBtn->setHighlighted(true);
			btn1->setHighlighted(false);
			btn2->setHighlighted(false);
			btn3->setHighlighted(false);

			curList->setVisible(true);
			list1->setVisible(false);
			list2->setVisible(false);
			list3->setVisible(false);
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