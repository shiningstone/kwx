#include "RankingScroll.h"

bool RankingScroll::init()
{
	if(!Layer::init())
	{
		return false;
	}
	Size visibleSize=Director::getInstance()->getVisibleSize();//大小和原点
	Vec2 origin=Director::getInstance()->getVisibleOrigin();

	auto cache=SpriteFrameCache::getInstance();//精灵缓冲帧
	cache->addSpriteFramesWithFile("denglupaihang.plist");

	Texture2D *pTexture=TextureCache::sharedTextureCache()->addImage("paimingshuzi.png");//精灵帧
	SpriteFrame* d[10];
	int a;
	for(a=0;a<10;a++)
	{
		d[a]=SpriteFrame::createWithTexture(pTexture,Rect(44.6*a,60*0,44.6,60));
	}

	auto InkScroll=Sprite::createWithSpriteFrame(cache->spriteFrameByName("juanzhou.png"));//卷轴
	InkScroll->setAnchorPoint(Vec2(0.5,0.5));
	InkScroll->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-33));
	this->addChild(InkScroll,0);

	//上部
	auto listTitle=Sprite::createWithSpriteFrame(cache->spriteFrameByName("fengyunpaihangbang.png"));//风云排行榜字体
	listTitle->setAnchorPoint(Vec2(0,1));
	listTitle->setPosition(Vec2(origin.x+visibleSize.width*407/1218,origin.y+visibleSize.height*628/716));
	this->addChild(listTitle,2);

	auto mojiTitle=Sprite::createWithSpriteFrame(cache->spriteFrameByName("biaoti moji.png"));//墨迹
	mojiTitle->setAnchorPoint(Vec2(0,0));
	mojiTitle->setPosition(Vec2(origin.x+visibleSize.width*365/1218,origin.y+visibleSize.height*533/716));
	this->addChild(mojiTitle,1);
	
	auto huahua=Sprite::createWithSpriteFrame(cache->spriteFrameByName("hua.png"));//花花
	huahua->setAnchorPoint(Vec2(0.5,0.5));
	huahua->setPosition(Vec2(origin.x+visibleSize.width*955/1218,origin.y+visibleSize.height*525/716));
	this->addChild(huahua,1);

	//日盈利--按钮
	auto dayin=Button::create("riyingli2.png","riyingli.png","riyingli2.png",UI_TEX_TYPE_PLIST);//字
	dayin->_ID=1;
	dayin->setTouchEnabled(true);
	dayin->addTouchEventListener(CC_CALLBACK_2(RankingScroll::dayInEvent,this));
	dayin->setAnchorPoint(Vec2(0,0));
	dayin->setPosition(Vec2(origin.x+visibleSize.width*235/1218,origin.y+visibleSize.height*465/716));
	this->addChild(dayin,2,10);
	dayin->setTouchEnabled(false);
	dayin->setHighlighted(true);

	//财富榜--按钮
	auto money=Button::create("caifubang2.png","caifubang.png","caifubang2.png",UI_TEX_TYPE_PLIST);
	money->_ID=2;
	money->setTouchEnabled(true);
	money->addTouchEventListener(CC_CALLBACK_2(RankingScroll::dayInEvent,this));
	money->setAnchorPoint(Vec2(0,0));
	money->setPosition(Vec2(origin.x+visibleSize.width*415/1218,origin.y+visibleSize.height*465/716));
	this->addChild(money,2,11);

	//最大番型--按钮
	auto maxtimes=Button::create("zuidafanxing2.png","zuidafanxing.png","zuidafanxing2.png",UI_TEX_TYPE_PLIST);
	maxtimes->_ID=3;
	maxtimes->setTouchEnabled(true);
	maxtimes->addTouchEventListener(CC_CALLBACK_2(RankingScroll::dayInEvent,this));
	maxtimes->setAnchorPoint(Vec2(0,0));
	maxtimes->setPosition(Vec2(origin.x+visibleSize.width*595/1218,origin.y+visibleSize.height*465/716));
	this->addChild(maxtimes,2,12);

	//战绩排名--按钮	
	auto record=Button::create("zhanjipaiming2.png","zhanjipaiming.png","zhanjipaiming2.png",UI_TEX_TYPE_PLIST);
	record->_ID=4;
	record->setTouchEnabled(true);
	record->addTouchEventListener(CC_CALLBACK_2(RankingScroll::dayInEvent,this));
	record->setAnchorPoint(Vec2(0,0));
	record->setPosition(Vec2(origin.x+visibleSize.width*775/1218,origin.y+visibleSize.height*465/716));
	this->addChild(record,2,13);
	//滚动
	ui::ScrollView* listScroll = ui::ScrollView::create();
	listScroll->setSize(Size(visibleSize.width*740/1218,visibleSize.height*375/716)); 
	listScroll->setInnerContainerSize(Size(visibleSize.width*740/1218,visibleSize.height*1880/716));
	listScroll->setPosition(Vec2(origin.x+visibleSize.width*224/1218,origin.y+visibleSize.height*90/716));
	listScroll->setDirection(ui::ScrollView::Direction::VERTICAL);    
	//listScroll->setBounceEnabled(true);
	this->addChild(listScroll,1);
	//滚动层大小
	auto innerWidth = listScroll->getSize().width;
	auto innerHeight = listScroll->getSize().height;
	//滚动层初始化
	for (int i = 0; i < 20; i++)
	{
		auto spriteFirst=Sprite::createWithSpriteFrame(cache->spriteFrameByName("liebiao.png"));//条纹
		spriteFirst->setAnchorPoint(Vec2(1,1));
		spriteFirst->setPosition(Vec2(innerWidth,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*i+12));
		/*spriteFirst->setScaleY(0.9);*/
		listScroll->addChild(spriteFirst,1);
		
		auto sprite1 = Sprite::createWithSpriteFrame(cache->spriteFrameByName("shuzianwen.png"));//数字暗纹
		sprite1->setAnchorPoint(Vec2(0,0));
		sprite1->setPosition(Vec2(0,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+6));
		listScroll->addChild(sprite1,2);	
		if(i>=0&&i<3)
		{
			switch (i)//第1,2,3图
			{
			case 0:
				{
					auto spriteNUM1=Sprite::createWithSpriteFrame(cache->spriteFrameByName("1.png"));//1
					spriteNUM1->setAnchorPoint(Vec2(0.5,0));
					spriteNUM1->setPosition(Vec2(50,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
					listScroll->addChild(spriteNUM1,3);
					break;
				}
			case 1:
				{
					auto spriteNUM2=Sprite::createWithSpriteFrame(cache->spriteFrameByName("2.png"));//2
					spriteNUM2->setAnchorPoint(Vec2(0.5,0));
					spriteNUM2->setPosition(Vec2(46,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
					listScroll->addChild(spriteNUM2,3);
					break;
				}
			case 2:
				{
					auto spriteNUM3=Sprite::createWithSpriteFrame(cache->spriteFrameByName("3.png"));//3
					spriteNUM3->setAnchorPoint(Vec2(0.5,0));
					spriteNUM3->setPosition(Vec2(46,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
					listScroll->addChild(spriteNUM3,3);
					break;
				}
			default:
				break;
			}
		}
		else if(i>=3&&i<9)//第4-9图
		{
			auto spriteNUM=Sprite::createWithSpriteFrame(d[i+1]);
			spriteNUM->setAnchorPoint(Vec2(0.5,0));
			spriteNUM->setPosition(Vec2(46,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
			listScroll->addChild(spriteNUM,3);
		}
		else if(i!=19)//第10-19图
		{
			auto spriteNUM4=Sprite::createWithSpriteFrame(d[1]);//十位
			spriteNUM4->setAnchorPoint(Vec2(1,0));
			spriteNUM4->setPosition(Vec2(50,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
			listScroll->addChild(spriteNUM4,3);

			auto spriteNUM5=Sprite::createWithSpriteFrame(d[i-9]);//个位
			spriteNUM5->setAnchorPoint(Vec2(0,0));
			spriteNUM5->setPosition(Vec2(34,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
			listScroll->addChild(spriteNUM5,3);
		}
		else//第20图
		{
			auto spriteNUM4=Sprite::createWithSpriteFrame(d[2]);//十位
			spriteNUM4->setAnchorPoint(Vec2(1,0));
			spriteNUM4->setPosition(Vec2(50,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
			listScroll->addChild(spriteNUM4,3);

			auto spriteNUM5=Sprite::createWithSpriteFrame(d[0]);//个位
			spriteNUM5->setAnchorPoint(Vec2(0,0));
			spriteNUM5->setPosition(Vec2(46,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+16));
			listScroll->addChild(spriteNUM5,3);
		}
						
		auto Icon1=Sprite::createWithSpriteFrame(cache->spriteFrameByName("812.png"));//玩家头像
		Icon1->setScale(82/Icon1->getContentSize().width,82/Icon1->getContentSize().height);
		Icon1->setAnchorPoint(Vec2(0,0));
		Icon1->setPosition(Vec2(innerWidth*109/740,listScroll->getInnerContainerSize().height-visibleSize.height*94/716*(i+1)+6));
		listScroll->addChild(Icon1,2);	

		if(i==0)//皇冠
		{
			auto crown1=Sprite::createWithSpriteFrame(cache->spriteFrameByName("guanjun.png"));//冠军
			crown1->setAnchorPoint(Vec2::ZERO);
			crown1->setPosition(Vec2(innerWidth*608/740,listScroll->getInnerContainerSize().height-visibleSize.height*85/716+4));
			listScroll->addChild(crown1,2);			
		}
		else if(i==1)
		{
			auto crown2=Sprite::createWithSpriteFrame(cache->spriteFrameByName("yajun.png"));//亚军	
			crown2->setAnchorPoint(Vec2::ZERO);
			crown2->setPosition(Vec2(innerWidth*608/740+7,listScroll->getInnerContainerSize().height-visibleSize.height*175/716+6));
			listScroll->addChild(crown2,2);
		}
		else if(i==2)
		{
			auto crown3=Sprite::createWithSpriteFrame(cache->spriteFrameByName("jijun.png"));//季军
			crown3->setAnchorPoint(Vec2::ZERO);
			crown3->setPosition(Vec2(innerWidth*608/740+12,listScroll->getInnerContainerSize().height-visibleSize.height*265/716+8));
			listScroll->addChild(crown3,2);
		}		
	}

	return true;
}

void RankingScroll::dayInEvent(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto button1=(Button*)this->getChildByTag(10);
	auto button2=(Button*)this->getChildByTag(11);
	auto button3=(Button*)this->getChildByTag(12);
	auto button4=(Button*)this->getChildByTag(13);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		switch (curButton->_ID)
		{
		case 1:
			curButton->setHighlighted(true);
			curButton->setTouchEnabled(false);
			button2->setHighlighted(false);
			button2->setTouchEnabled(true);
			button3->setHighlighted(false);
			button3->setTouchEnabled(true);
			button4->setHighlighted(false);
			button4->setTouchEnabled(true);
			break;
		case 2:
			curButton->setHighlighted(true);
			curButton->setTouchEnabled(false);
			button1->setHighlighted(false);
			button1->setTouchEnabled(true);
			button3->setHighlighted(false);
			button3->setTouchEnabled(true);
			button4->setHighlighted(false);
			button4->setTouchEnabled(true);
			break;
		case 3:
			curButton->setHighlighted(true);
			curButton->setTouchEnabled(false);
			button2->setHighlighted(false);
			button2->setTouchEnabled(true);
			button1->setHighlighted(false);
			button1->setTouchEnabled(true);
			button4->setHighlighted(false);
			button4->setTouchEnabled(true);
			break;
		case 4:
			curButton->setHighlighted(true);
			curButton->setTouchEnabled(false);
			button2->setHighlighted(false);
			button2->setTouchEnabled(true);
			button3->setHighlighted(false);
			button3->setTouchEnabled(true);
			button1->setHighlighted(false);
			button1->setTouchEnabled(true);
			break;
		default:
			break;
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void RankingScroll::moneyEvent(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
}

void RankingScroll::recordEvent(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
}

void RankingScroll::maxtimesEvent(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
}
void RankingScroll::btTouchEvent(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
}