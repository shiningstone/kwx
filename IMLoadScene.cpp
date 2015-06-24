#include "IMLoadScene.h"
#include "EnterRoomScene.h"

USING_NS_CC;

IMLoad::IMLoad()
{
	s_scale=1.189;
	s_no=1;
}

IMLoad::~IMLoad()
{
}
void IMLoad::enterRoom()
{
    auto scene = Scene::create();
	auto layer = EnterRoom::create();
    scene->addChild(layer);
    Director::getInstance()->replaceScene(scene);
}
const std::string ResFileName[]={"load_room.plist","Personalinformation.plist","chongfuziyuan.plist","chongfuziyuan2.plist",
	"shezhijiemian.plist","changguichangbupiao.plist","changguichangdaipiao.plist","bisaichang.plist",
//<<<<<<< .mine
	"bisaidengdai.plist","bisaibaoming.plist",/*"RankImage.plist",*/"ChatImg.plist","FriendsImage.plist",
//=======
	"bisaidengdai.plist","bisaibaoming.plist","ChatImg.plist","FriendsImage.plist",
//>>>>>>> .r320
	"selectGameAndScene.plist","dengluzhutu.plist","changguichang2.plist",
//new
	"Createrole.plist","mall.plist"
};
//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ChatImg.plist");
//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("FriendsImage.plist");
//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("selectGameAndScene.plist");
//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dengluzhutu.plist");
void IMLoad::addRes()
{
	for(int insertNum=0;insertNum<21;insertNum++)
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ResFileName[insertNum]);
		curLoadingBar->setPercentage((float)(insertNum+1)/21*100);
	}
	auto EnterNextRoom=CallFunc::create(this,callfunc_selector(IMLoad::enterRoom));
	this->runAction(EnterNextRoom);

	//Vector<FiniteTimeAction *> addList;
	//for(int insertNum=0;insertNum<16;insertNum++)
	//{
	//	auto curFunc=CallFunc::create([=](){
	//		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ResFileName[insertNum]);
	//		auto to1 = ProgressTo::create(0.2,(float)(insertNum+1)/11*100);
	//		curLoadingBar->runAction(to1);
	//	});
	//	auto delayTime=DelayTime::create(0.1);
	//	addList.insert(insertNum,Sequence::create(curFunc,delayTime,NULL));
	//}
	//auto allSeq=Sequence::create(addList);
	//auto EnterNextRoom=CallFunc::create(this,callfunc_selector(IMLoad::enterRoom));
	//this->runAction(Sequence::create(allSeq,EnterNextRoom,NULL));
}

bool IMLoad::init()
{
	if(!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	Sprite *sprite;
	if(s_no == 1)
		sprite = Sprite::create("loadwin.png");
	else
		sprite = Sprite::create("loadwin2.png");
	sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	sprite->setScaleX(s_scale);
	sprite->setScaleY(s_scale);
	this->addChild(sprite, 0);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("load.plist");

  	auto ring= Sprite::createWithSpriteFrameName("ring.png");
	ring->setAnchorPoint(Vec2(0.5,0.5));
    ring->setPosition(Point(visibleSize.width*0.4945788 + origin.x, visibleSize.height*0.6 + origin.y));//0.5 7/12
    this->addChild(ring,1);

    auto chs=Sprite::createWithSpriteFrameName("character.png");
	chs->setAnchorPoint(Vec2(0.5,0.5));
    //chs->setPosition(Point(ring->getPosition().x,ring->getPosition().y));
    chs->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.6));
    this->addChild(chs,1);

    auto bar=Sprite::createWithSpriteFrameName("black_bar.png");
    curLoadingBar=ProgressTimer::create(bar);
    curLoadingBar->setType(ProgressTimer::Type::BAR);
    curLoadingBar->setBarChangeRate(Point(1,0));
    curLoadingBar->setMidpoint(Point(0,0));
	curLoadingBar->setPercentage(0.0f);
    curLoadingBar->setPosition(Point(ring->getPosition().x,ring->getPosition().y-ring->getContentSize().height*3/4));
    this->addChild(curLoadingBar,1,110);

    auto you=Sprite::createWithSpriteFrameName("you.png");
    you->setPosition(Point(ring->getPosition().x-bar->getTextureRect().size.width/3,ring->getPosition().y-ring->getContentSize().height*3/4+bar->getTextureRect().size.height/2+10));
    this->addChild(you,1);

    auto xi=Sprite::createWithSpriteFrameName("xi.png");
    xi->setPosition(Point(you->getPosition().x+you->getTextureRect().size.width,you->getPosition().y));
    this->addChild(xi,1);

    auto jia=Sprite::createWithSpriteFrameName("jia.png");
    jia->setPosition(Point(xi->getPosition().x+xi->getTextureRect().size.width,you->getPosition().y));
    this->addChild(jia,1);

    auto zai=Sprite::createWithSpriteFrameName("zai.png");
    zai->setPosition(Point(jia->getPosition().x+jia->getTextureRect().size.width,you->getPosition().y));
    this->addChild(zai,1);

    auto zhong=Sprite::createWithSpriteFrameName("zhong.png");
    zhong->setPosition(Point(zai->getPosition().x+zai->getTextureRect().size.width,you->getPosition().y));
    this->addChild(zhong,1);

    auto point1=Sprite::createWithSpriteFrameName("point.png");
    point1->setPosition(Point(zhong->getPosition().x+zhong->getTextureRect().size.width,you->getPosition().y));
    this->addChild(point1,1);

    auto point2=Sprite::createWithSpriteFrameName("point.png");
    point2->setPosition(Point(point1->getPosition().x+15,you->getPosition().y));
    this->addChild(point2,1);

    auto point3=Sprite::createWithSpriteFrameName("point.png");
    point3->setPosition(Point(point2->getPosition().x+15,you->getPosition().y));
    this->addChild(point3,1);

	auto positionOfYou=you->getPosition();
	auto positionOfXi=xi->getPosition();
	auto positionOfJia=jia->getPosition();
	auto positionOfZai=zai->getPosition();
	auto positionOfZhong=zhong->getPosition();

	auto moveH1=MoveTo::create(0.1,Vec2(positionOfYou.x,positionOfYou.y+20));
	auto moveL1=MoveTo::create(0.1,Vec2(positionOfYou.x,positionOfYou.y));
	auto jump1=Sequence::create(moveH1,moveL1,NULL);

	auto moveH2=MoveTo::create(0.1,Vec2(positionOfXi.x,positionOfYou.y+20));
	auto moveL2=MoveTo::create(0.1,Vec2(positionOfXi.x,positionOfYou.y));
	auto jump2=Sequence::create(moveH2,moveL2,NULL);

	auto moveH3=MoveTo::create(0.1,Vec2(positionOfJia.x,positionOfYou.y+20));
	auto moveL3=MoveTo::create(0.1,Vec2(positionOfJia.x,positionOfYou.y));
	auto jump3=Sequence::create(moveH3,moveL3,NULL);

	auto moveH4=MoveTo::create(0.1,Vec2(positionOfZai.x,positionOfYou.y+20));
	auto moveL4=MoveTo::create(0.1,Vec2(positionOfZai.x,positionOfYou.y));
	auto jump4=Sequence::create(moveH4,moveL4,NULL);

	auto moveH5=MoveTo::create(0.1,Vec2(positionOfZhong.x,positionOfYou.y+20));
	auto moveL5=MoveTo::create(0.1,Vec2(positionOfZhong.x,positionOfYou.y));
	auto jump5=Sequence::create(moveH5,moveL5,NULL);
		
	auto hide = Hide::create();
	auto show = Show::create();
	auto delay01=DelayTime::create(0.2f);
	auto delay02=DelayTime::create(0.4f);
	auto delay03=DelayTime::create(0.6f);
	auto delay04=DelayTime::create(0.8f);
	auto delay05=DelayTime::create(1.0f);

	auto delay1=DelayTime::create(1);
	auto delay2=DelayTime::create(2);
	auto delay3=DelayTime::create(3);
	
	auto seqOfLabel1=Sequence::create(delay01,jump1,delay05,jump1,delay05,jump1,delay05,jump1,delay05,jump1,delay05,jump1,delay05,NULL);
	auto seqOfLabel2=Sequence::create(delay02,jump2,delay05,jump2,delay05,jump2,delay05,jump2,delay05,jump2,delay05,jump2,delay05,NULL);
	auto seqOfLabel3=Sequence::create(delay03,jump3,delay05,jump3,delay05,jump3,delay05,jump3,delay05,jump3,delay05,jump3,delay05,NULL);
	auto seqOfLabel4=Sequence::create(delay04,jump4,delay05,jump4,delay05,jump4,delay05,jump4,delay05,jump4,delay05,jump4,delay05,NULL);
	auto seqOfLabel5=Sequence::create(delay05,jump5,delay05,jump5,delay05,jump5,delay05,jump5,delay05,jump5,delay05,jump5,delay05,NULL);

	you->runAction(seqOfLabel1);
	xi->runAction(seqOfLabel2);
	jia->runAction(seqOfLabel3);
	zai->runAction(seqOfLabel4);
	zhong->runAction(seqOfLabel5);

	auto seq1 = Sequence::create( show,delay3,hide,delay1,NULL);
	auto seq2 = Sequence::create( hide,delay1,show,delay2,hide,delay1,NULL);
	auto seq3 =  Sequence::create( hide,delay2,show,delay1,hide,delay1,NULL);

	point1->runAction(RepeatForever::create(seq1));

	point2->runAction(RepeatForever::create(seq2));

	point3->runAction(RepeatForever::create(seq3));

	auto rotate = RotateBy::create(1,360);

	ring->runAction(RepeatForever::create(rotate));

	addRes();

	//auto addAllRes=CallFunc::create(this,callfunc_selector(IMLoad::addRes));
	//auto EnterNextRoom=CallFunc::create(this,callfunc_selector(IMLoad::enterRoom));
	//this->runAction(Sequence::create(addAllRes,EnterNextRoom,NULL));

    return true;
}

