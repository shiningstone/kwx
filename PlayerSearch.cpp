#include "PlayerSearch.h"
PlayerSearch::PlayerSearch(EnterRoom* p)
{
	parentNode=p;
}
PlayerSearch::~PlayerSearch()
{
}
bool PlayerSearch::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();

	auto SearchBKG=LayerColor::create();
	SearchBKG->setContentSize(Size(visibleSize.width,visibleSize.height));
	SearchBKG->setColor(ccWHITE);
	SearchBKG->ignoreAnchorPointForPosition(false);
	SearchBKG->setAnchorPoint(Vec2(0.5,0.5));
	SearchBKG->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(SearchBKG,0);

	auto searchBkg=Sprite::createWithSpriteFrameName("FriendSearch_BKG.png");
	searchBkg->setAnchorPoint(Vec2(0.5,0.5));
	searchBkg->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(searchBkg,1,FRIEND_SEARCH_BKG);
	auto searchBkg_size=searchBkg->getContentSize();

	auto idEdit=EditBox::create(Size(visibleSize.width*0.3037767,visibleSize.height*0.0614525),Scale9Sprite::create());
	idEdit->setAnchorPoint(Vec2::ZERO);
	idEdit->setPosition(Vec2(visibleSize.width*0.04516,visibleSize.height*0.3980447));
	idEdit->setColor(ccBLACK);
	idEdit->setPlaceHolder("请输入玩家ID...");
	idEdit->setMaxLength(9);
	idEdit->setInputMode(cocos2d::extension::EditBox::InputMode::NUMERIC);
	searchBkg->addChild(idEdit,2,ID_EDIT_BOX);

	char tempId[20];
	sprintf(tempId,"%d",123456789);
	std::string tempID_Str=tempId;
	std::string tempStr="也可告诉好友你的ID:"+tempID_Str;
	auto logForMineId=LabelTTF::create(tempStr,"Arial",20);
	logForMineId->setColor(Color3B(105,99,75));
	logForMineId->setAnchorPoint(Vec2(0,0.5));
	logForMineId->setPosition(Vec2(visibleSize.width*0.0467981,visibleSize.height*0.3603352));
	searchBkg->addChild(logForMineId);

	auto searchButton=Button::create("baoming anniu2.png","baoming anniu.png","baoming anniu.png",UI_TEX_TYPE_PLIST);
	searchButton->setAnchorPoint(Vec2(0.5,0.5));
	searchButton->setPosition(Vec2(visibleSize.width*0.4039409,visibleSize.height*0.4301676));
	searchButton->addTouchEventListener(CC_CALLBACK_2(PlayerSearch::Friend_Search_Call,this));
	searchBkg->addChild(searchButton,2);

	auto Btn_font=Sprite::create("SearchFont.png");
	Btn_font->setAnchorPoint(Vec2(0.5,0.5));
	Btn_font->setPosition(Vec2(searchButton->getContentSize().width/2,searchButton->getContentSize().height/2));
	searchButton->addChild(Btn_font);

	EventListenerTouchOneByOne* SearchListener=EventListenerTouchOneByOne::create();
	SearchListener->setSwallowTouches(true);
	SearchListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	SearchListener->onTouchMoved=[=](Touch* touch, Event* event){};
	SearchListener->onTouchEnded=[=](Touch* touch, Event* event){
		float x=visibleSize.width/2-searchBkg_size.width/2;
		float y=visibleSize.height/2-searchBkg_size.height/2;
		Rect rect=Rect(x,y,searchBkg_size.width,searchBkg_size.height);
		if(!rect.containsPoint(touch->getLocation()))
		{
			auto deleteFunc=CallFunc::create([=](){
				if(parentNode->getChildByTag(FRIEND_SERCH_HINT))
					parentNode->removeChildByTag(FRIEND_SERCH_HINT,true);
			});
			parentNode->runAction(deleteFunc);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(SearchListener,SearchBKG);

	return true;
}
void PlayerSearch::SearchResult(OtherPlayers_Info searchedFriend)
{
	searchedFriend=EnvVariable::getInstance()->get_Searched_Friend();

	auto resultBkg=this->getChildByTag(FRIEND_SEARCH_BKG);
	auto neededSize=Size(visibleSize.width*0.4811166,visibleSize.height*0.1117318);

	auto resultLayer=Layout::create();
	resultLayer->setAnchorPoint(Vec2(0.5,1));
	resultLayer->setPosition(Vec2(resultBkg->getContentSize().width/2,resultBkg->getContentSize().height-120));
	resultLayer->setSize(neededSize);
	resultBkg->addChild(resultLayer);

	auto searchedPhoto=Sprite::createWithSpriteFrameName(searchedFriend.photoFileName);
	auto photoScaleY=neededSize.height/searchedPhoto->getContentSize().height;
	auto photoScaleX=searchedPhoto->getContentSize().height*photoScaleY/searchedPhoto->getContentSize().width;
	searchedPhoto->setScale(photoScaleY,photoScaleX);
	searchedPhoto->setAnchorPoint(Vec2(0,0.5));
	searchedPhoto->setPosition(Vec2(0,neededSize.height/2));
	resultLayer->addChild(searchedPhoto);

	auto playerName=LabelTTF::create(searchedFriend.NikeName,"Arial",20);
	playerName->setAnchorPoint(Vec2(0,1));
	playerName->setPosition(Vec2(searchedPhoto->getPosition().x+searchedPhoto->getContentSize().width*photoScaleX,neededSize.height-4));
	resultLayer->addChild(playerName);

	Sprite* playerSex;
	switch (searchedFriend.friendSex)
	{
	case BOY:
		playerSex=Sprite::createWithSpriteFrameName("Friends_man.png");
		break;
	case GIRL:
		playerSex=Sprite::createWithSpriteFrameName("Friends_woman.png");
	default:
		break;
	}
	playerSex->setAnchorPoint(Vec2(0.5,0));
	playerSex->setPosition(Vec2(playerName->getPosition().x+playerName->getContentSize().width/2,4));
	resultLayer->addChild(playerSex);

	auto friendAddBtn=Button::create("FriendAddBtn1.png","FriendAddBtn2.png","FriendAddBtn2.png",UI_TEX_TYPE_LOCAL);
	friendAddBtn->setAnchorPoint(Vec2(1,0.5));
	friendAddBtn->setPosition(Vec2(neededSize.width-5,neededSize.height/2));
	friendAddBtn->addTouchEventListener(CC_CALLBACK_2(PlayerSearch::Friend_Add_Call,this));
	resultLayer->addChild(friendAddBtn);

	auto resultLine=Sprite::create("FriendLine.png");
	resultLine->setAnchorPoint(Vec2(0.5,0.5));
	resultLine->setPosition(Vec2(neededSize.width/2,resultLayer->getPosition().y-resultLine->getContentSize().height-10));
	resultLayer->addChild(resultLine);
}
void PlayerSearch::Friend_Add_Call(cocos2d::Ref* pSender,ui::Widget::TouchEventType type)
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
void PlayerSearch::Friend_Search_Call(cocos2d::Ref* pSender,ui::Widget::TouchEventType type)
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