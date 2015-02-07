#include "IMLoadScene.h"
#include "EnterRoomScene.h"
//#include "QQLoadScene.h"

USING_NS_CC;

IMLoad::IMLoad()
{
	s_scale=1.189;
	s_no=1;
}

IMLoad::~IMLoad()
{
}
void IMLoad::enterRoom(float dt)
{
    auto scene = Scene::create();

	auto layer = EnterRoom::create();

    scene->addChild(layer);

    Director::getInstance()->replaceScene(scene);
}

void IMLoad::addRes(float dt)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("load_room.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("volume.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("jibenxinximian.plist"); 
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("chongfuziyuan.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("chongfuziyuan2.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shezhijiemian.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("changguichangbupiao.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("changguichangdaipiao.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bisaichang.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bisaidengdai.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bisaibaoming.plist");
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
	// position the sprite on the center of the screen
	sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	sprite->setScaleX(s_scale);
	sprite->setScaleY(s_scale);
	this->addChild(sprite, 0);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("load.plist");
  	auto ring= Sprite::createWithSpriteFrameName("ring.png");
    ring->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height*7/12 + origin.y));
    this->addChild(ring,1);

    auto chs=Sprite::createWithSpriteFrameName("character.png");
    chs->setPosition(Point(ring->getPosition().x,ring->getPosition().y));
    this->addChild(chs,1);

    auto bar=Sprite::createWithSpriteFrameName("black_bar.png");
    auto to1 = ProgressTo::create(3,100);
    auto s_bar=ProgressTimer::create(bar);
    s_bar->setType(ProgressTimer::Type::BAR);
    s_bar->setMidpoint(Point(0,0));
    s_bar->setBarChangeRate(Point(1,0));
    this->addChild(s_bar,1);
    s_bar->setPosition(Point(ring->getPosition().x,ring->getPosition().y-ring->getContentSize().height*3/4));
    s_bar->runAction(RepeatForever::create(to1));

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

	//SpriteFrameCache::getInstance()->removeSpriteFrames();
	//scheduleOnce(schedule_selector(IMLoad::addRes),0.5);
	scheduleOnce(schedule_selector(IMLoad::enterRoom),3.0);
	scheduleOnce(schedule_selector(IMLoad::addRes),0.5f);

    return true;
}

