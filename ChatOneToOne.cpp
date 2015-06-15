#include "ChatOneToOne.h"
#include "EnterRoomScene.h"

ChatOneToOne::ChatOneToOne(std::string Object)
{
	ChatObject=Object;
}

ChatOneToOne::~ChatOneToOne(void)
{
}

bool ChatOneToOne::init()
{
	if(!Layer::init())
	{
		return false;
	}
	////SpriteFrameCache::getInstance()->addSpriteFramesWithFile("FriendsImage.plist");

	//auto FriendTalkBKG=LayerColor::create();
	//FriendTalkBKG->setContentSize(Size(visibleSize.width,visibleSize.height));
	//FriendTalkBKG->setColor(Color3B(28,120,135));
	//FriendTalkBKG->setOpacity(50);
	//FriendTalkBKG->setPosition(Vec2::ZERO);
	//this->addChild(FriendTalkBKG,1,FRIEND_CHAT_BKG);

	//auto chatBkg=Sprite::createWithSpriteFrameName("chat_common_back.png");
	//chatBkg->setAnchorPoint(Vec2(0.5,0.5));
	//chatBkg->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	//FriendTalkBKG->addChild(chatBkg,1,CHAT_BIG_KUANG_BKG);
	//auto chatSize=chatBkg->getTextureRect().size;

	//auto chatRecordBKG=Sprite::createWithSpriteFrameName("chat_common_back_main.png");
	//chatRecordBKG->setAnchorPoint(Vec2(0.5,0));
	//chatRecordBKG->setPosition(Vec2(chatSize.width/2,chatSize.height*10/487));
	//chatBkg->addChild(chatRecordBKG,1);

	//auto CharRecord=Button::create("chat_common_table.png","chat_common_table_sel.png","chat_common_table.png",UI_TEX_TYPE_PLIST);
	//CharRecord->setAnchorPoint(Vec2(0,0));
	//CharRecord->setPosition(Vec2(chatSize.width*14.5/520,chatSize.height*420/487));
	//CharRecord->addTouchEventListener(CC_CALLBACK_2(EnterRoom::ChatRecordCallBack,this));
	//CharRecord->setTouchEnabled(false);
	//CharRecord->setHighlighted(true);
	//chatBkg->addChild(CharRecord,1,NODE_CHILD_ID_ONE);
	//auto CommonLanguage=Button::create("chat_common_table.png","chat_common_table_sel.png","chat_common_table.png",UI_TEX_TYPE_PLIST);
	//CommonLanguage->setAnchorPoint(Vec2(0,0));
	//CommonLanguage->setPosition(Vec2(chatSize.width*206.5/620,chatSize.height*420/487));
	//CommonLanguage->addTouchEventListener(CC_CALLBACK_2(EnterRoom::CommonLanguageCallBack,this));
	//chatBkg->addChild(CommonLanguage,1,NODE_CHILD_ID_TWO);

	//auto MessangeEdit=Sprite::createWithSpriteFrameName("chat_text_input_area.png");
	//MessangeEdit->setAnchorPoint(Vec2::ZERO);
	//MessangeEdit->setPosition(Vec2(chatSize.width*14.5/620,chatSize.height*12/487));
	//MessangeEdit->setOpacity(50);
	//chatBkg->addChild(MessangeEdit,1);

	//auto MessageBox=EditBox::create(Size(visibleSize.width*0.29,visibleSize.height*0.042),Scale9Sprite::create());
	////auto MessageBox=EditBox::create(Size(visibleSize.width*0.29,visibleSize.height*0.042),Scale9Sprite::createWithSpriteFrameName("chat_text_input_area.png"));
	//MessageBox->setAnchorPoint(Vec2(0,0));
	//MessageBox->setPosition(Vec2(chatSize.width*100/620,chatSize.height*30/487));
	//MessageBox->setFontColor(ccBLACK);
	//MessageBox->setPlaceHolder("请在这里输入聊天内容...");
	//MessageBox->setMaxLength(15);
	//MessageBox->setInputMode(cocos2d::extension::EditBox::InputMode::ANY);
	//chatBkg->addChild(MessageBox,1,NODE_CHILD_ID_THREE);

	//auto SendButton=Button::create("chat_send.png","chat_send_sel.png","chat_send.png",UI_TEX_TYPE_PLIST);
	//SendButton->setAnchorPoint(Vec2(1,0.5));
	//SendButton->setPosition(Vec2(chatSize.width*590/620,chatSize.height*52/487));
	//SendButton->addTouchEventListener(CC_CALLBACK_2(EnterRoom::MessageSendCallBack,this));
	//chatBkg->addChild(SendButton,1);

	//EventListenerTouchOneByOne* SettingChooseListener=EventListenerTouchOneByOne::create();
	//SettingChooseListener->setSwallowTouches(true);
	//SettingChooseListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	//SettingChooseListener->onTouchMoved=[=](Touch* touch, Event* event){};
	//SettingChooseListener->onTouchEnded=[=](Touch* touch, Event* event){
	//	float x=visibleSize.width/2-chatSize.width/2;
	//	float y=visibleSize.height/2-chatSize.height/2;
	//	Rect rect=Rect(x,y,chatSize.width,chatSize.height);
	//	if(!rect.containsPoint(touch->getLocation()))
	//	{
	//		auto deleteFunc=CallFunc::create([=](){
	//			if(this->getChildByTag(THIS_FRIEND_CHAT_BKG))
	//				this->removeChildByTag(THIS_FRIEND_CHAT_BKG,true);
	//		});
	//		this->runAction(deleteFunc);
	//	}
	//};
	//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(SettingChooseListener,FriendTalkBKG);

	return true;
}