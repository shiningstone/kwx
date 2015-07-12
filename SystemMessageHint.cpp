#include "SystemMessageHint.h"
#include "HelloWorldScene.h"

SystemMessageHint::SystemMessageHint(Node* p,std::string str,int tag)
{
	parentNode=p;
	CurTag=tag;
	NoticeMessage=str;
	init();
}
SystemMessageHint::~SystemMessageHint(void)
{
}
bool SystemMessageHint::init()
{
	if(!Layer::init())
	{
		return false;
	}

	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();

	auto NoticeBKG=LayerColor::create();
	NoticeBKG->setContentSize(Size(visibleSize.width,visibleSize.height));
	NoticeBKG->setColor(ccWHITE);
	NoticeBKG->ignoreAnchorPointForPosition(false);
	NoticeBKG->setAnchorPoint(Vec2(0.5,0.5));
	NoticeBKG->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(NoticeBKG,0);

	auto NotieHint=Sprite::create("system_Notice.png");
	NotieHint->setAnchorPoint(Vec2(0.5,0.5));
	NotieHint->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(NotieHint,1);
	auto NotieHintSize=NotieHint->getContentSize();

	auto MessageFont=LabelTTF::create(NoticeMessage,"Arial",20);
	MessageFont->setAnchorPoint(Vec2(0.5,0.5));
	MessageFont->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(MessageFont,2);

	EventListenerTouchOneByOne* NoticeListener=EventListenerTouchOneByOne::create();
	NoticeListener->setSwallowTouches(true);
	NoticeListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	NoticeListener->onTouchMoved=[=](Touch* touch, Event* event){};
	NoticeListener->onTouchEnded=[=](Touch* touch, Event* event){
		float x=visibleSize.width/2-NotieHintSize.width/2;
		float y=visibleSize.height/2-NotieHintSize.height/2;
		Rect rect=Rect(x,y,NotieHintSize.width,NotieHintSize.height);
		if(!rect.containsPoint(touch->getLocation()))
		{
			auto deleteFunc=CallFunc::create([=](){
				if(parentNode->getChildByTag(CurTag))
					parentNode->removeChildByTag(CurTag,true);
			});
			auto replaceFunc=CallFunc::create([=](){
				auto scene=Scene::create();
				auto layer=HelloWorld::create();
				scene->addChild(layer);
				Director::getInstance()->replaceScene(scene);
			});
			parentNode->runAction(Sequence::create(deleteFunc,replaceFunc,NULL));
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(NoticeListener,NoticeBKG);


	return true;
}