#include "CreateRole.h"

CreateRole::CreateRole(EnterRoom* p)
{
	parentNode=p;
}
CreateRole::~CreateRole()
{
}
bool CreateRole::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();
	lastChooseNum=1;

	auto RoleCreateBKG=LayerColor::create();
	RoleCreateBKG->setContentSize(Size(visibleSize.width,visibleSize.height));
	RoleCreateBKG->setColor(ccWHITE);
	RoleCreateBKG->ignoreAnchorPointForPosition(false);
	RoleCreateBKG->setAnchorPoint(Vec2(0.5,0.5));
	RoleCreateBKG->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(RoleCreateBKG,0);
	EventListenerTouchOneByOne* RoleCreateListener=EventListenerTouchOneByOne::create();
	RoleCreateListener->setSwallowTouches(true);
	RoleCreateListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	RoleCreateListener->onTouchMoved=[=](Touch* touch, Event* event){};
	RoleCreateListener->onTouchEnded=[=](Touch* touch, Event* event){};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(RoleCreateListener,RoleCreateBKG);

	auto mainBkg=Sprite::create("Createrolebj.png");
	mainBkg->setAnchorPoint(Vec2(0.5,0.5));
	mainBkg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(mainBkg,1,CREATE_ROLE_BKG);

	for(int a=0;a<4;a++)
	{
		float positionX=(440+180*a)*1218/visibleSize.width;
		float positionYUp=408/716*visibleSize.height;
		float positionYLow=239/716*visibleSize.height;
		char tempChar[15];

		sprintf(tempChar,"Head%d.png",a+1);
		auto photoUp=Button::create(tempChar,tempChar,tempChar,UI_TEX_TYPE_PLIST);
		photoUp->addTouchEventListener(CC_CALLBACK_2(CreateRole::DefaultPhotoChooseCall,this));
		photoUp->setAnchorPoint(Vec2::ZERO);
		photoUp->setPosition(Vec2(positionX,positionYUp));
		mainBkg->addChild(photoUp,2,DEFAULT_PHOTO+a);
		if(a==0)
			photoUp->setTouchEnabled(false);

		auto upChoosed=Sprite::createWithSpriteFrameName("Createrole-Checkthebox.png");
		upChoosed->setAnchorPoint(Vec2(0.5,0.5));
		upChoosed->setPosition(Vec2(photoUp->getPosition().x+photoUp->getContentSize().width/2,photoUp->getPosition().y+photoUp->getContentSize().height/2));
		mainBkg->addChild(upChoosed,1,PHOTO_CHOOSED_BKG+a);
		if(a!=0)
			upChoosed->setVisible(false);
		else
			upChoosed->setVisible(true);


		sprintf(tempChar,"Head%d.png",a+1);
		auto photoLow=Button::create(tempChar,tempChar,tempChar,UI_TEX_TYPE_PLIST);
		photoLow->addTouchEventListener(CC_CALLBACK_2(CreateRole::DefaultPhotoChooseCall,this));
		photoLow->setAnchorPoint(Vec2::ZERO);
		photoLow->setPosition(Vec2(positionX,positionYUp));
		mainBkg->addChild(photoLow,2,DEFAULT_PHOTO+a+4);

		auto downChoosed=Sprite::createWithSpriteFrameName("Createrole-Checkthebox.png");
		downChoosed->setAnchorPoint(Vec2(0.5,0.5));
		downChoosed->setPosition(Vec2(photoLow->getPosition().x+photoLow->getContentSize().width/2,photoLow->getPosition().y+photoLow->getContentSize().height/2));
		mainBkg->addChild(downChoosed,1,PHOTO_CHOOSED_BKG+a);
		downChoosed->setVisible(false);
	}

	auto choosedPhoto=Sprite::createWithSpriteFrameName("Head1.png");
	choosedPhoto->setAnchorPoint(Vec2::ZERO);
	choosedPhoto->setPosition(Vec2(origin.x+visibleSize.width*0.0517241,origin.y+visibleSize.height*0.472067));
	mainBkg->addChild(choosedPhoto,1,MY_CHOOSED_PHOTO);

	return true;
}
void CreateRole::DefaultPhotoChooseCall(cocos2d::Ref* pSender,ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto curBkg=this->getChildByTag(CREATE_ROLE_BKG);
			auto curBtn=(Button*)pSender;
			int curTag=curBtn->getTag();
			auto curChooseBkg=curBkg->getChildByTag(curTag+8);

			auto lastBtn=(Button*)curBkg->getChildByTag(lastChooseNum);
			auto lastChooseBkg=curBkg->getChildByTag(lastChooseNum+8);

			curBtn->setTouchEnabled(false);
			curChooseBkg->setVisible(true);
			lastBtn->setTouchEnabled(true);
			lastChooseBkg->setVisible(false);

			lastChooseNum=curTag;

			char tempChar[15];
			sprintf(tempChar,"Head%d.png",lastChooseNum);
			if(curBkg->getChildByTag(MY_CHOOSED_PHOTO))
			{
				curBkg->removeChildByTag(MY_CHOOSED_PHOTO,true);
				auto curMyChoose=Sprite::createWithSpriteFrameName(tempChar);
				curMyChoose->setAnchorPoint(Vec2::ZERO);
				curMyChoose->setPosition(Vec2(origin.x+visibleSize.width*0.0517241,origin.y+visibleSize.height*0.472067));
				curBkg->addChild(curMyChoose,1,MY_CHOOSED_PHOTO);
			}
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}