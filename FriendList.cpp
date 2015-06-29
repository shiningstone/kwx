#include "FriendList.h"

FriendList::FriendList(EnterRoom* p)
{
	parentNode=p;
	MyFriendList=EnvVariable::getInstance()->get_MyFriendList();
	NearyPeopleList=EnvVariable::getInstance()->get_NearyPeopleList();
	StrangersList=EnvVariable::getInstance()->get_StrangersList();
	init();
	for(int a=0;a<5;a++)//ceshi_yusi
		TouchNum[a]=0;
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
			CCLOG("ListView------->Begin       TouchTime : %d",TouchNum[0]);
			TouchNum[0]++;
		}
		break;
	case LISTVIEW_ONSELECTEDITEM_END:
		{
			CCLOG("ListView------->Ended       TouchTime : %d",TouchNum[1]);
			TouchNum[1]++;
		}
		break;
	default:
		break;
	}
}

bool FriendList::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();

	//BackGround------
	auto FriendBKG=Sprite::createWithSpriteFrameName("Friends_reel.png");//好友
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
	/**********0000**************/
	auto defaultBtn=Button::create("Friends_cellBg.png","Friends_cellBg.png","Friends_cellBg.png",UI_TEX_TYPE_PLIST);
	defaultBtn->setAnchorPoint(Vec2(0.5,0.5));
	auto defaultItem=Layout::create();
	defaultItem->setTouchEnabled(true);
	defaultItem->setSize(defaultBtn->getContentSize());
	defaultBtn->setPosition(Vec2(defaultItem->getSize().width/2,defaultItem->getSize().height/2));
	defaultItem->addChild(defaultBtn);
	FriendsList->setItemModel(defaultItem);
	/**********1111**************/

	for(int a=0;a<MyFriendList.size();a++)
	{
		//auto curBtn=Sprite::createWithSpriteFrameName("Friends_cellBg.png");
		auto curBtn=Button::create("Friends_cellBg.png","Friends_cellBg.png","Friends_cellBg.png",UI_TEX_TYPE_PLIST);
		curBtn->setAnchorPoint(Vec2(0.5,0.5));
		curBtn->addTouchEventListener(CC_CALLBACK_2(FriendList::listItemCallBack,this));
		auto curItem=Layout::create();
		auto BtnSize=curBtn->getContentSize();

		auto curPhoto=Sprite::createWithSpriteFrameName(MyFriendList[a].photoFileName);
		curPhoto->setAnchorPoint(Vec2(0,0.5));
		curPhoto->setPosition(Vec2(3,BtnSize.height/2));
		curPhoto->setScale(76/curPhoto->getTextureRect().size.height);
		curBtn->addChild(curPhoto);
		auto curPhotoSize=curPhoto->getTextureRect().size*curPhoto->getScale();

		auto curName=LabelTTF::create(MyFriendList[a].NikeName,"Arial",20);
		curName->setAnchorPoint(Vec2(0,1));
		curName->setPosition(Vec2(curPhoto->getPosition().x+curPhotoSize.width+10,curPhoto->getPosition().y+curPhotoSize.height/2));
		curBtn->addChild(curName);

		Sprite* curSex;
		switch (MyFriendList[a].friendSex)
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
		sprintf(tempChar,"%d",MyFriendList[a].FriendGold+a);
		auto curGold=LabelTTF::create(tempChar,"Arial",20);
		curGold->setAnchorPoint(Vec2(1,0.5));
		curGold->setPosition(Vec2(BtnSize.width-10,BtnSize.height/2));
		curBtn->addChild(curGold);

		curItem->setTouchEnabled(true);
		//EventListenerTouchOneByOne* FriendCellListener=EventListenerTouchOneByOne::create();
		//FriendCellListener->setSwallowTouches(false);
		//FriendCellListener->onTouchBegan=[=](Touch* touch, Event* event){
		//	CCLOG("onTouchBegan : FriendCell");
		//	return true;};
		//FriendCellListener->onTouchMoved=[=](Touch* touch, Event* event){CCLOG("onTouchMoved : FriendCell");};
		//FriendCellListener->onTouchEnded=[=](Touch* touch, Event* event){CCLOG("onTouchEnd : FriendCell");};
		//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(FriendCellListener,curItem);
		curItem->setSize(BtnSize);
		curBtn->setPosition(Vec2(curItem->getSize().width/2,curItem->getSize().height/2));
		curItem->addChild(curBtn,1,a);

		FriendsList->pushBackCustomItem(curItem);
	}
	scheduleOnce(schedule_selector(FriendList::updateTest),2);//ceshi_yusi
	scheduleOnce(schedule_selector(FriendList::updateTest1),4);
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
void FriendList::ChatBtnCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curBtn=(Button*)pSender;
			auto anotherBtn=(Button*)parentNode->getChildByTag(THIS_FRIEND_CHAT_BKG)->getChildByTag(CHAT_CHILD_BAK)->getChildByTag(curBtn->getTag()%2+1);
			curBtn->setTouchEnabled(false);
			curBtn->setHighlighted(true);
			anotherBtn->setTouchEnabled(true);
			anotherBtn->setHighlighted(false);
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
void FriendList::MessageSendCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
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
void FriendList::ChatOneToOneCreate(Friend_Info chatFriend)
{
	auto FriendTalkBKG=LayerColor::create();
	FriendTalkBKG->setContentSize(Size(visibleSize.width,visibleSize.height));
	FriendTalkBKG->setColor(Color3B(28,120,135));
	FriendTalkBKG->setOpacity(50);
	FriendTalkBKG->setPosition(Vec2::ZERO);
	parentNode->addChild(FriendTalkBKG,5,THIS_FRIEND_CHAT_BKG);

	auto chatBkg=Sprite::createWithSpriteFrameName("chat_common_back.png");
	chatBkg->setAnchorPoint(Vec2(0.5,0.5));
	chatBkg->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	FriendTalkBKG->addChild(chatBkg,1,CHAT_CHILD_BAK);
	auto chatSize=chatBkg->getTextureRect().size;

	auto chatRecordBKG=Sprite::createWithSpriteFrameName("chat_common_back_main.png");
	chatRecordBKG->setAnchorPoint(Vec2(0.5,0));
	chatRecordBKG->setPosition(Vec2(chatSize.width/2,chatSize.height*10/487));
	chatBkg->addChild(chatRecordBKG,1);

	auto CharRecord=Button::create("chat_chatrecord.png","chat_chatrecord_sel.png","chat_chatrecord_sel.png",UI_TEX_TYPE_PLIST);
	CharRecord->setAnchorPoint(Vec2(0,0));
	CharRecord->setPosition(Vec2(chatSize.width*14.5/520,chatSize.height*420/487));
	CharRecord->addTouchEventListener(CC_CALLBACK_2(FriendList::ChatBtnCallBack,this));
	CharRecord->setTouchEnabled(false);
	CharRecord->setHighlighted(true);
	chatBkg->addChild(CharRecord,1,CHATRECORD_BTN);
	auto CommonLanguage=Button::create("chat_commonz_language.png","chat_commonz_language_sel.png","chat_commonz_language_sel.png",UI_TEX_TYPE_PLIST);
	CommonLanguage->setAnchorPoint(Vec2(0,0));
	CommonLanguage->setPosition(Vec2(chatSize.width*206.5/620,chatSize.height*420/487));
	CommonLanguage->addTouchEventListener(CC_CALLBACK_2(FriendList::ChatBtnCallBack,this));
	chatBkg->addChild(CommonLanguage,1,COMMON_BTN);

	auto MessangeEdit=Sprite::createWithSpriteFrameName("chat_text_input_area.png");
	MessangeEdit->setAnchorPoint(Vec2::ZERO);
	MessangeEdit->setPosition(Vec2(chatSize.width*14.5/620,chatSize.height*12/487));
	MessangeEdit->setOpacity(50);
	chatBkg->addChild(MessangeEdit,1);

	auto MessageBox=EditBox::create(Size(visibleSize.width*0.29,visibleSize.height*0.042),Scale9Sprite::create());
	//auto MessageBox=EditBox::create(Size(visibleSize.width*0.29,visibleSize.height*0.042),Scale9Sprite::createWithSpriteFrameName("chat_text_input_area.png"));
	MessageBox->setAnchorPoint(Vec2(0,0));
	MessageBox->setPosition(Vec2(chatSize.width*100/620,chatSize.height*30/487));
	MessageBox->setFontColor(ccBLACK);
	MessageBox->setPlaceHolder("请在这里输入聊天内容...");
	MessageBox->setMaxLength(15);
	MessageBox->setInputMode(cocos2d::extension::EditBox::InputMode::ANY);
	chatBkg->addChild(MessageBox,1,CHAT_EDIT_BOX);

	auto SendButton=Button::create("chat_send.png","chat_send_sel.png","chat_send.png",UI_TEX_TYPE_PLIST);
	SendButton->setAnchorPoint(Vec2(1,0.5));
	SendButton->setPosition(Vec2(chatSize.width*590/620,chatSize.height*52/487));
	SendButton->addTouchEventListener(CC_CALLBACK_2(FriendList::MessageSendCallBack,this));
	chatBkg->addChild(SendButton,1);

	EventListenerTouchOneByOne* SettingChooseListener=EventListenerTouchOneByOne::create();
	SettingChooseListener->setSwallowTouches(true);
	SettingChooseListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	SettingChooseListener->onTouchMoved=[=](Touch* touch, Event* event){};
	SettingChooseListener->onTouchEnded=[=](Touch* touch, Event* event){
		float x=visibleSize.width/2-chatSize.width/2;
		float y=visibleSize.height/2-chatSize.height/2;
		Rect rect=Rect(x,y,chatSize.width,chatSize.height);
		if(!rect.containsPoint(touch->getLocation()))
		{
			auto deleteFunc=CallFunc::create([=](){
				if(parentNode->getChildByTag(THIS_FRIEND_CHAT_BKG))
					parentNode->removeChildByTag(THIS_FRIEND_CHAT_BKG,true);
			});
			parentNode->runAction(deleteFunc);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(SettingChooseListener,FriendTalkBKG);
}
void FriendList::listItemCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	auto curBtn=(Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			CCLOG("Button------->Begin       TouchTime : %d",TouchNum[2]);
			TouchNum[2]++;
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		{
			//CCLOG("Button------->Moved       TouchTime : %d",TouchNum[3]);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			CCLOG("Button------->Ended       TouchTime : %d",TouchNum[4]);
			TouchNum[4]++;
			//auto curBtn=(Button*)pSender;
			//ChatOneToOneCreate(MyFriendList[curBtn->getTag()]);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		{
		}
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
void FriendList::updateTest(float delt)
{
	auto FriendsList=(ListView*)this->getChildByTag(FRIEND_LIST);
	//FriendsList->setItemsMargin(10);
	FriendsList->setInnerContainerSize(Size(visibleSize.width*0.4334975,visibleSize.height*0.3));
	FriendsList->insertDefaultItem(0);
}
void FriendList::updateTest1(float delt)
{
	auto FriendsList=(ListView*)this->getChildByTag(FRIEND_LIST);
	FriendsList->removeItem(1);
}