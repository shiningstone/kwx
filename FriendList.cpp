#include "FriendList.h"

FriendList::FriendList(Friend_Info friendsList[],OtherPlayers_Info NearbyPlayers[],OtherPlayers_Info Stranger[])
{
	//int a=0;
	//while (friendsList[a].photoFileName!="")
	//{
	//	MyFriendList.insert(a,friendsList[a]);
	//	a++;
	//}
	//a=0;
	//while (NearbyPlayers[a].photoFileName!="")
	//{
	//	NearyPeopleList.insert(a,NearbyPlayers[a]);
	//	a++;
	//}
	//a=0;
	//while (Stranger[a].photoFileName!="")
	//{
	//	StrangersList.insert(a,Stranger[a]);
	//	a++;
	//}
}

FriendList::~FriendList(void)
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
	auto FriendList=ListView::create();
	FriendList->setDirection(SCROLLVIEW_DIR_VERTICAL);//SCROLLVIEW_DIR_VERTICAL
	FriendList->setTouchEnabled(true);
	FriendList->setBounceEnabled(true);
	FriendList->setSize(Size(visibleSize.width*0.4334975,visibleSize.height*0.5726257));
	FriendList->ignoreAnchorPointForPosition(false);
	FriendList->setAnchorPoint(Vec2(0.5,0));
	FriendList->setPosition(Vec2(visibleSize.width*0.27358,45));
	this->addChild(FriendList,2,FRIEND_LIST);
	//ListView-----nearPeople
	//ListView-----strangers
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

	auto NearListBtn=Button::create("Near_the_game player.png","Near_the_game player.png","Near_the_game player.png",UI_TEX_TYPE_PLIST);
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
void FriendList::ChatRecordCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
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
void FriendList::CommonLanguageCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
		}
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
void FriendList::ChatOneToOneCreate()
{
	auto FriendTalkBKG=LayerColor::create();
	FriendTalkBKG->setContentSize(Size(visibleSize.width,visibleSize.height));
	FriendTalkBKG->setColor(Color3B(28,120,135));
	FriendTalkBKG->setOpacity(50);
	FriendTalkBKG->setPosition(Vec2::ZERO);
	this->addChild(FriendTalkBKG,5,FRIEND_CHAT_BKG);

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
	CharRecord->addTouchEventListener(CC_CALLBACK_2(FriendList::ChatRecordCallBack,this));
	CharRecord->setTouchEnabled(false);
	CharRecord->setHighlighted(true);
	chatBkg->addChild(CharRecord,1,CHATRECORD_BTN);
	auto CommonLanguage=Button::create("chat_commonz_language.png","chat_commonz_language_sel.png","chat_commonz_language_sel.png",UI_TEX_TYPE_PLIST);
	CommonLanguage->setAnchorPoint(Vec2(0,0));
	CommonLanguage->setPosition(Vec2(chatSize.width*206.5/620,chatSize.height*420/487));
	CommonLanguage->addTouchEventListener(CC_CALLBACK_2(FriendList::CommonLanguageCallBack,this));
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
				if(this->getChildByTag(FRIEND_CHAT_BKG))
					this->removeChildByTag(FRIEND_CHAT_BKG,true);
			});
			this->runAction(deleteFunc);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(SettingChooseListener,FriendTalkBKG);
}
void FriendList::FriendCellBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			ChatOneToOneCreate();
		}
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
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
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
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}