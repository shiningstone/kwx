#include "ReconnectRemind.h"
USING_NS_CC;

ReconnectRemind::ReconnectRemind()
{
	s_scale=1.189;
	s_no=1;
}

ReconnectRemind::~ReconnectRemind()
{
}

bool ReconnectRemind::init()
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

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("loading.plist");

  	auto ring= Sprite::createWithSpriteFrameName("loading-ring.png");
	ring->setAnchorPoint(Vec2(0.5,0.5));
    ring->setPosition(Point(visibleSize.width*0.4945788 + origin.x, visibleSize.height*0.6 + origin.y));
    this->addChild(ring,1);

    auto chs=Sprite::createWithSpriteFrameName("loading-character.png");
	chs->setAnchorPoint(Vec2(0.5,0.5));
    chs->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.6));
    this->addChild(chs,1);

    auto you=Sprite::createWithSpriteFrameName("loading-you.png");
    you->setPosition(Point(origin.x+visibleSize.width*0.3960566,origin.y+visibleSize.height*0.35));
    this->addChild(you,1);

    auto xi=Sprite::createWithSpriteFrameName("loading-xi.png");
    xi->setPosition(Point(you->getPosition().x+you->getTextureRect().size.width,you->getPosition().y));
    this->addChild(xi,1);

    auto jia=Sprite::createWithSpriteFrameName("loading-chong.png");
    jia->setPosition(Point(xi->getPosition().x+xi->getTextureRect().size.width,you->getPosition().y));
    this->addChild(jia,1);

    auto zai=Sprite::createWithSpriteFrameName("loading-lian.png");
    zai->setPosition(Point(jia->getPosition().x+jia->getTextureRect().size.width,you->getPosition().y));
    this->addChild(zai,1);

    auto zhong=Sprite::createWithSpriteFrameName("loading-zhong.png");
    zhong->setPosition(Point(zai->getPosition().x+zai->getTextureRect().size.width,you->getPosition().y));
    this->addChild(zhong,1);

    auto point1=Sprite::createWithSpriteFrameName("loading-point.png");
    point1->setPosition(Point(origin.x+visibleSize.width*0.5717545,origin.y+visibleSize.height*0.3318436));
    this->addChild(point1,1);

    auto point2=Sprite::createWithSpriteFrameName("loading-point.png");
    point2->setPosition(Point(point1->getPosition().x+15,point1->getPosition().y));
    this->addChild(point2,1);

    auto point3=Sprite::createWithSpriteFrameName("loading-point.png");
    point3->setPosition(Point(point2->getPosition().x+15,point1->getPosition().y));
    this->addChild(point3,1);

	auto positionOfYou=you->getPosition();
	auto positionOfXi=xi->getPosition();
	auto positionOfJia=jia->getPosition();
	auto positionOfZai=zai->getPosition();
	auto positionOfZhong=zhong->getPosition();
	auto positionOfPoint1=point1->getPosition();
	auto positionOfPoint2=point2->getPosition();
	auto positionOfPoint3=point3->getPosition();

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

	auto moveH6=MoveTo::create(0.1,Vec2(positionOfPoint1.x,positionOfPoint1.y+10));
	auto moveL6=MoveTo::create(0.1,Vec2(positionOfPoint1.x,positionOfPoint1.y));
	auto jump6=Sequence::create(moveH6,moveL6,NULL);

	auto moveH7=MoveTo::create(0.1,Vec2(positionOfPoint2.x,positionOfPoint1.y+10));
	auto moveL7=MoveTo::create(0.1,Vec2(positionOfPoint2.x,positionOfPoint1.y));
	auto jump7=Sequence::create(moveH7,moveL7,NULL);

	auto moveH8=MoveTo::create(0.1,Vec2(positionOfPoint3.x,positionOfPoint1.y+10));
	auto moveL8=MoveTo::create(0.1,Vec2(positionOfPoint3.x,positionOfPoint1.y));
	auto jump8=Sequence::create(moveH8,moveL8,NULL);

	you->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.2),jump1,DelayTime::create(1.4),NULL)));
	xi->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.4),jump2,DelayTime::create(1.2),NULL)));
	jia->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.6),jump3,DelayTime::create(1.0),NULL)));
	zai->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.8),jump4,DelayTime::create(0.8),NULL)));
	zhong->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.0),jump5,DelayTime::create(0.6),NULL)));

	point1->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.2),jump6,DelayTime::create(0.4),NULL)));
	point2->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.4),jump7,DelayTime::create(0.2),NULL)));
	point3->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.6),jump8,NULL)));

	auto rotate = RotateBy::create(1,360);
	ring->runAction(RepeatForever::create(rotate));

    return true;
}

