#include "FriendList.h"
#include "ChatOneToOne.h"

FriendList::FriendList(EnterRoom* p)
{
	parentNode=p;
	MyFriendList=EnvVariable::getInstance()->get_MyFriendList();
	NearyPeopleList=EnvVariable::getInstance()->get_NearyPeopleList();
	StrangersList=EnvVariable::getInstance()->get_StrangersList();
	init();
}

FriendList::~FriendList(void)
{
}
void FriendList::ListEventCall(cocos2d::Ref *pSender,ListViewEventType type)
{
	switch (type)
	{
	case LISTVIEW_ONSELECTEDITEM_START:
		{
			CCLOG("LIST_VIEW_START");
			auto listView=(ListView*)pSender;
			curSelectedItem=listView->getCurSelectedIndex();
		}
		break;
	case LISTVIEW_ONSELECTEDITEM_END:
		{
			CCLOG("LIST_VIEW_ENDED");
			//curSelectedItem=-1;
		}
		break;
	default:
		break;
	}
}
Layout* FriendList::friendCell_create(int curNo)
{
	auto curBtn=Button::create("Friends_cellBg.png","Friends_cellBg.png","Friends_cellBg.png",UI_TEX_TYPE_PLIST);
	curBtn->setAnchorPoint(Vec2(0.5,0.5));
	curBtn->addTouchEventListener(CC_CALLBACK_2(FriendList::listItemCallBack,this));
	auto curItem=Layout::create();
	auto BtnSize=curBtn->getContentSize();

	auto curPhoto=Sprite::createWithSpriteFrameName(MyFriendList[curNo].photoFileName);
	curPhoto->setAnchorPoint(Vec2(0,0.5));
	curPhoto->setPosition(Vec2(3,BtnSize.height/2));
	curPhoto->setScale(76/curPhoto->getTextureRect().size.height);
	curBtn->addChild(curPhoto);
	auto curPhotoSize=curPhoto->getTextureRect().size*curPhoto->getScale();

	auto curName=LabelTTF::create(MyFriendList[curNo].NikeName,"Arial",20);
	curName->setAnchorPoint(Vec2(0,1));
	curName->setPosition(Vec2(curPhoto->getPosition().x+curPhotoSize.width+10,curPhoto->getPosition().y+curPhotoSize.height/2));
	curBtn->addChild(curName);

	Sprite* curSex;
	switch (MyFriendList[curNo].friendSex)
	{
	case BOY:
		curSex=Sprite::createWithSpriteFrameName("Friends_man.png");
		break;
	case GIRL:
		curSex=Sprite::createWithSpriteFrameName("Friends_woman.png");
		break;
	default:
		break;
	}
	curSex->setAnchorPoint(Vec2(0.5,0));
	curSex->setPosition(Vec2(curName->getPosition().x+curName->getContentSize().width/2,curPhoto->getPosition().y-curPhotoSize.height/2));
	curBtn->addChild(curSex);

	char tempChar[10];
	sprintf(tempChar,"%d",MyFriendList[curNo].FriendGold+curNo);
	auto curGold=LabelTTF::create(tempChar,"Arial",20);
	curGold->setAnchorPoint(Vec2(1,0.5));
	curGold->setPosition(Vec2(BtnSize.width-10,BtnSize.height/2));
	curBtn->addChild(curGold);

	curItem->setTouchEnabled(true);
	curItem->setSize(BtnSize);
	curBtn->setPosition(Vec2(curItem->getSize().width/2,curItem->getSize().height/2));
	curItem->addChild(curBtn,1);

	return curItem;
}
void FriendList::friendList_Create()
{
	auto FriendsList=ListView::create();
	FriendsList->setDirection(SCROLLVIEW_DIR_VERTICAL);//SCROLLVIEW_DIR_VERTICAL
	FriendsList->setGravity(LISTVIEW_GRAVITY_CENTER_VERTICAL);
	FriendsList->setTouchEnabled(true);
	FriendsList->setBounceEnabled(true);
	FriendsList->setSize(Size(visibleSize.width*0.4334975,visibleSize.height*0.5726257));
	FriendsList->ignoreAnchorPointForPosition(false);
	FriendsList->setAnchorPoint(Vec2(0.5,0));
	FriendsList->setPosition(Vec2(visibleSize.width*0.27358,45));
	FriendsList->addEventListenerListView(this,listvieweventselector(FriendList::ListEventCall));
	this->addChild(FriendsList,2,FRIEND_LIST);

	//myFriendList_______setModel
	auto defaultBtn=Button::create("Friends_cellBg.png","Friends_cellBg.png","Friends_cellBg.png",UI_TEX_TYPE_PLIST);
	defaultBtn->setAnchorPoint(Vec2(0.5,0.5));
	auto defaultItem=Layout::create();
	defaultItem->setTouchEnabled(true);
	defaultItem->setSize(defaultBtn->getContentSize());
	defaultBtn->setPosition(Vec2(defaultItem->getSize().width/2,defaultItem->getSize().height/2));
	defaultItem->addChild(defaultBtn);
	FriendsList->setItemModel(defaultItem);

	for(int a=0;a<MyFriendList.size();a++)
	{
		Layout* curItem=friendCell_create(a);
		FriendsList->pushBackCustomItem(curItem);
	}
}
void FriendList::deleteBtn_Hint_create(float delta)
{
	auto deleteHintBkg=LayerColor::create();
	deleteHintBkg->setContentSize(Size(visibleSize.width,visibleSize.height));
	deleteHintBkg->setColor(ccWHITE);
	deleteHintBkg->ignoreAnchorPointForPosition(false);
	deleteHintBkg->setAnchorPoint(Vec2(0.5,0.5));
	deleteHintBkg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(deleteHintBkg,4,FRIEND_DELETE_BKG);

	auto friendDeleteBtn=Button::create("Friend_Delete.png","Friend_Delete.png","Friend_Delete.png",UI_TEX_TYPE_LOCAL);
	friendDeleteBtn->setAnchorPoint(Vec2(0.5,0));
	friendDeleteBtn->setPosition(curTouchPos);
	deleteHintBkg->addChild(friendDeleteBtn,1,FRIEND_DELETE_BTN);

	EventListenerTouchOneByOne* deleteHintBkgListener=EventListenerTouchOneByOne::create();
	deleteHintBkgListener->setSwallowTouches(true);
	deleteHintBkgListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	deleteHintBkgListener->onTouchMoved=[=](Touch* touch, Event* event){};
	deleteHintBkgListener->onTouchEnded=[=](Touch* touch, Event* event){
		if(this->getChildByTag(FRIEND_DELETE_BKG))
		{
			auto curDeleteBtn=this->getChildByTag(FRIEND_DELETE_BKG)->getChildByTag(FRIEND_DELETE_BTN);
			auto deleteBtnSize=friendDeleteBtn->getContentSize();
			float x=curDeleteBtn->getPosition().x-deleteBtnSize.width/2;
			float y=curDeleteBtn->getPosition().y;
			Rect rect=Rect(x,y,deleteBtnSize.width,deleteBtnSize.height);
			if(!rect.containsPoint(touch->getLocation()))
			{
				auto deleteFunc=CallFunc::create([=](){
					deleteBtn_Hint_delete();	
				});
				this->runAction(deleteFunc);
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(deleteHintBkgListener,deleteHintBkg);
}
void FriendList::deleteBtn_Hint_delete()
{
	if(this->getChildByTag(FRIEND_DELETE_BKG))
		this->removeChildByTag(FRIEND_DELETE_BKG,true);
}
void FriendList::InsertFriendItem(Layout* curItem,int insetNo)
{
	auto FriendsList=(ListView*)this->getChildByTag(FRIEND_LIST);
	FriendsList->insertCustomItem(curItem,insetNo);
}
void FriendList::DeleteFriendItem(int deleteNo)
{
	auto FriendsList=(ListView*)this->getChildByTag(FRIEND_LIST);
	FriendsList->removeItem(deleteNo);
}
void FriendList::friendDeleteCall(cocos2d::Ref *pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			DeleteFriendItem(curSelectedItem);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void FriendList::nearyList_Create()
{
}
void FriendList::strangerList_Create()
{
}
bool FriendList::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();
	curSelectedItem=-1;

	auto ListPosGet=LayerColor::create();
	ListPosGet->setContentSize(Size(visibleSize.width,visibleSize.height));
	ListPosGet->setColor(ccWHITE);
	ListPosGet->ignoreAnchorPointForPosition(false);
	ListPosGet->setAnchorPoint(Vec2(0.5,0.5));
	ListPosGet->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	ListPosGet->setPosition(Vec2::ZERO);
	this->addChild(ListPosGet,5);

	EventListenerTouchOneByOne* ListPosGetListener=EventListenerTouchOneByOne::create();
	ListPosGetListener->setSwallowTouches(false);
	ListPosGetListener->onTouchBegan=[=](Touch* touch, Event* event){
		CCLOG("LISTENER_TOUCH_BEGAN");
		curTouchPos=touch->getLocation();
		return true;};
	ListPosGetListener->onTouchMoved=[=](Touch* touch, Event* event){};
	ListPosGetListener->onTouchEnded=[=](Touch* touch, Event* event){};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(ListPosGetListener,ListPosGet);
	
	//BackGround------
	auto FriendBKG=Sprite::createWithSpriteFrameName("Friends_reel.png");//ºÃÓÑ
	auto ProgressBar=ProgressTimer::create(FriendBKG);
	ProgressBar->setType(ProgressTimer::Type::BAR);
	ProgressBar->setBarChangeRate(Point(0,1));
	ProgressBar->setMidpoint(Point(0,1));
	ProgressBar->setPercentage(0);
	ProgressBar->setAnchorPoint(Vec2(0.5,0.5));
	ProgressBar->setPosition(Vec2(origin.x+visibleSize.width*0.29,origin.y+visibleSize.height*0.38));
	this->addChild(ProgressBar,1);
	auto ProgressAction=EaseIn::create(Sequence::create(DelayTime::create(0.1),ProgressTo::create(0.4,100),NULL),5);
	ProgressBar->runAction(ProgressAction);

	//ListView-----myFriend
	friendList_Create();

	//ListView-----nearPeople
	auto nearPerponList=ListView::create();
	nearPerponList->setDirection(SCROLLVIEW_DIR_VERTICAL);//SCROLLVIEW_DIR_VERTICAL
	nearPerponList->setGravity(LISTVIEW_GRAVITY_CENTER_VERTICAL);
	nearPerponList->setTouchEnabled(true);
	nearPerponList->setBounceEnabled(true);
	nearPerponList->setSize(Size(visibleSize.width*0.4334975,visibleSize.height*0.5726257));
	nearPerponList->ignoreAnchorPointForPosition(false);
	nearPerponList->setAnchorPoint(Vec2(0.5,0));
	nearPerponList->setPosition(Vec2(visibleSize.width*0.27358,45));
	//nearPerponList->addEventListenerListView(this,listvieweventselector(FriendList::selectedItemEvent));
	nearPerponList->setVisible(false);
	this->addChild(nearPerponList,2,NEAR_LIST);

	for(int a=0;a<NearyPeopleList.size();a++)
	{
		auto curBtn=Button::create("Friends_cellBg.png","Friends_cellBg.png","Friends_cellBg.png",UI_TEX_TYPE_PLIST);
		curBtn->setAnchorPoint(Vec2(0.5,0.5));
		auto curItem=Layout::create();
		auto BtnSize=curBtn->getSize();

		auto curPhoto=Sprite::createWithSpriteFrameName(NearyPeopleList[a].photoFileName);
		curPhoto->setAnchorPoint(Vec2(0,0.5));
		curPhoto->setPosition(Vec2(3,BtnSize.height/2));
		curPhoto->setScale(76/curPhoto->getTextureRect().size.width,76/curPhoto->getTextureRect().size.height);
		curBtn->addChild(curPhoto);
		auto curPhotoSize=curPhoto->getTextureRect().size*curPhoto->getScale();

		auto curName=LabelTTF::create(NearyPeopleList[a].NikeName,"Arial",20);
		curName->setAnchorPoint(Vec2(0,1));
		curName->setPosition(Vec2(curPhoto->getPosition().x+curPhotoSize.width+10,curPhoto->getPosition().y+curPhotoSize.height/2));
		curBtn->addChild(curName);

		Sprite* curSex;
		switch (NearyPeopleList[a].friendSex)
		{
		case BOY:
			curSex=Sprite::createWithSpriteFrameName("Friends_man.png");
			break;
		case GIRL:
			curSex=Sprite::createWithSpriteFrameName("Friends_woman.png");
			break;
		default:
			break;
		}
		curSex->setAnchorPoint(Vec2(0.5,0));
		curSex->setPosition(Vec2(curName->getPosition().x+curName->getContentSize().width/2,curPhoto->getPosition().y-curPhotoSize.height/2));
		curBtn->addChild(curSex);

		curItem->setTouchEnabled(true);
		curItem->setSize(BtnSize);
		curBtn->setPosition(Vec2(curItem->getSize().width/2,curItem->getSize().height/2));
		curItem->addChild(curBtn);

		nearPerponList->pushBackCustomItem(curItem);
	}

	//ListView-----strangers
	auto strangerList=ListView::create();
	strangerList->setDirection(SCROLLVIEW_DIR_VERTICAL);//SCROLLVIEW_DIR_VERTICAL
	strangerList->setGravity(LISTVIEW_GRAVITY_CENTER_VERTICAL);
	strangerList->setTouchEnabled(true);
	strangerList->setBounceEnabled(true);
	strangerList->setSize(Size(visibleSize.width*0.4334975,visibleSize.height*0.5726257));
	strangerList->ignoreAnchorPointForPosition(false);
	strangerList->setAnchorPoint(Vec2(0.5,0));
	strangerList->setPosition(Vec2(visibleSize.width*0.27358,45));
	//strangerList->addEventListenerListView(this,listvieweventselector(FriendList::selectedItemEvent));
	strangerList->setVisible(false);
	this->addChild(strangerList,2,STRANGER_LIST);

	for(int a=0;a<StrangersList.size();a++)
	{
		auto curBtn=Button::create("Friends_cellBg.png","Friends_cellBg.png","Friends_cellBg.png",UI_TEX_TYPE_PLIST);
		curBtn->setAnchorPoint(Vec2(0.5,0.5));
		auto curItem=Layout::create();
		auto BtnSize=curBtn->getSize();

		auto curPhoto=Sprite::createWithSpriteFrameName(StrangersList[a].photoFileName);
		curPhoto->setAnchorPoint(Vec2(0,0.5));
		curPhoto->setPosition(Vec2(3,BtnSize.height/2));
		curPhoto->setScale(76/curPhoto->getTextureRect().size.width,76/curPhoto->getTextureRect().size.height);
		curBtn->addChild(curPhoto);
		auto curPhotoSize=curPhoto->getTextureRect().size*curPhoto->getScale();

		auto curName=LabelTTF::create(StrangersList[a].NikeName,"Arial",20);
		curName->setAnchorPoint(Vec2(0,1));
		curName->setPosition(Vec2(curPhoto->getPosition().x+curPhotoSize.width+10,curPhoto->getPosition().y+curPhotoSize.height/2));
		curBtn->addChild(curName);

		Sprite* curSex;
		switch (StrangersList[a].friendSex)
		{
		case BOY:
			curSex=Sprite::createWithSpriteFrameName("Friends_man.png");
			break;
		case GIRL:
			curSex=Sprite::createWithSpriteFrameName("Friends_woman.png");
			break;
		default:
			break;
		}
		curSex->setAnchorPoint(Vec2(0.5,0));
		curSex->setPosition(Vec2(curName->getPosition().x+curName->getContentSize().width/2,curPhoto->getPosition().y-curPhotoSize.height/2));
		curBtn->addChild(curSex);

		curItem->setTouchEnabled(true);
		curItem->setSize(BtnSize);
		curBtn->setPosition(Vec2(curItem->getSize().width/2,curItem->getSize().height/2));
		curItem->addChild(curBtn);

		strangerList->pushBackCustomItem(curItem);
	}
	//Button------
	auto FriendListBtn=Button::create("Myfriend.png","Myfriend_sel.png","Myfriend_sel.png",UI_TEX_TYPE_PLIST);
	FriendListBtn->setAnchorPoint(Vec2(0.5,0.5));
	FriendListBtn->setPosition(Vec2(visibleSize.width*0.118396,visibleSize.height*0.6676));
	FriendListBtn->addTouchEventListener(CC_CALLBACK_2(FriendList::ListBtnCallBack,this));
	FriendListBtn->setTouchEnabled(false);
	FriendListBtn->setHighlighted(true);
	FriendListBtn->setScale(0,1);
	this->addChild(FriendListBtn,2,FRIEND_BTN);
	auto CharRecordAction=EaseBounceIn::create(Sequence::create(DelayTime::create(0.5),ScaleTo::create(0.3,1),NULL));
	FriendListBtn->runAction(CharRecordAction);

	auto NearListBtn=Button::create("Near_the_game player.png","Near_the_game player_sel.png","Near_the_game player_sel.png",UI_TEX_TYPE_PLIST);
	NearListBtn->setAnchorPoint(Vec2(0.5,0.5));
	NearListBtn->setPosition(Vec2(visibleSize.width*0.241549,visibleSize.height*0.6676));
	NearListBtn->addTouchEventListener(CC_CALLBACK_2(FriendList::ListBtnCallBack,this));
	NearListBtn->setScale(0,1);
	this->addChild(NearListBtn,2,NEAR_BTN);
	auto CommonLanguageAction=EaseBounceIn::create(Sequence::create(DelayTime::create(0.5),ScaleTo::create(0.3,1),NULL));
	NearListBtn->runAction(CommonLanguageAction);

	auto StrangerListBtn=Button::create("Stranger.png","Stranger_sel.png","Stranger_sel.png",UI_TEX_TYPE_PLIST);
	StrangerListBtn->setAnchorPoint(Vec2(0.5,0.5));
	StrangerListBtn->setPosition(Vec2(visibleSize.width*0.364702,visibleSize.height*0.6676));
	StrangerListBtn->addTouchEventListener(CC_CALLBACK_2(FriendList::ListBtnCallBack,this));
	StrangerListBtn->setScale(0,1);
	this->addChild(StrangerListBtn,2,STRANGER_BTN);
	auto StrangerListAction=EaseBounceIn::create(Sequence::create(DelayTime::create(0.5),ScaleTo::create(0.3,1),NULL));
	StrangerListBtn->runAction(StrangerListAction);

	auto Friends_Add=Button::create("Friends_Add.png","Friends_Add.png","Friends_Add.png",UI_TEX_TYPE_PLIST);
	Friends_Add->setScale(0.6857143);
	Friends_Add->setAnchorPoint(Vec2(0,0.5));
	Friends_Add->setPosition(Vec2(origin.x+visibleSize.width*0.44,origin.y+visibleSize.height*0.6676));
	Friends_Add->addTouchEventListener(CC_CALLBACK_2(FriendList::Friend_Invitation,this));
	this->addChild(Friends_Add,2,FRIEND_ADD_BTN);

	auto FriendBKup=Sprite::createWithSpriteFrameName("Friends_reelHead.png");
	FriendBKup->setAnchorPoint(Vec2(0.5,0.5));
	FriendBKup->setPosition(Vec2(origin.x+visibleSize.width*0.29-2,origin.y+visibleSize.height*0.73-4));
	this->addChild(FriendBKup,3);

	auto FriendBKdown=Sprite::createWithSpriteFrameName("Friends_reelfoot.png");
	FriendBKdown->setAnchorPoint(Vec2(0.5,0));
	FriendBKdown->setPosition(Vec2(origin.x+visibleSize.width*0.29-2,origin.y+visibleSize.height*0.73-4-FriendBKdown->getTextureRect().size.height/2));
	this->addChild(FriendBKdown,3);
	auto DownAction=EaseIn::create(Sequence::create(DelayTime::create(0.1),MoveTo::create(0.4,Vec2(origin.x+visibleSize.width*0.29-2,7)),NULL),5);
	FriendBKdown->runAction(DownAction);

	return true;
}
void FriendList::listItemCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	auto curBtn=(Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			CCLOG("BUTTON***BEGAN");
			deleteBtn_Hint_delete();
			scheduleOnce(schedule_selector(FriendList::deleteBtn_Hint_create),0.5);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		{
			CCLOG("BUTTON***MOVED");
			deleteBtn_Hint_delete();
			unschedule(schedule_selector(FriendList::deleteBtn_Hint_create));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			CCLOG("BUTTON***ENDED");
			unschedule(schedule_selector(FriendList::deleteBtn_Hint_create));
			auto ChatLayer=new ChatOneToOne(parentNode,MyFriendList[curSelectedItem]);
			parentNode->addChild(ChatLayer,5,THIS_FRIEND_CHAT_BKG);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void FriendList::Friend_Invitation(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
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
void FriendList::ListBtnCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curBtn=(Button*)pSender;
			auto curTag=curBtn->getTag();
			auto Btn1=(Button*)this->getChildByTag(curTag%3+1);
			auto Btn2=(Button*)this->getChildByTag((curTag+1)%3+1);
			auto curList=(ListView*)this->getChildByTag(curTag+3);
			auto List1=(ListView*)this->getChildByTag(curTag%3+4);
			auto List2=(ListView*)this->getChildByTag((curTag+1)%3+4);
			curBtn->setTouchEnabled(false);
			curBtn->setHighlighted(true);
			Btn1->setTouchEnabled(true);
			Btn1->setHighlighted(false);
			Btn2->setTouchEnabled(true);
			Btn2->setHighlighted(false);
			curList->setVisible(true);
			List1->setVisible(false);
			List2->setVisible(false);
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