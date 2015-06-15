#include "EnterRoomScene.h"
#include "BaseScene.h"
#include "MyInfoScene.h"
#include "ActivityCenter.h"
#include "TaskReward.h"
#include "SetScene.h"
#include "ShopScene.h"
#include "CommonField.h"
#include "CompetitionField.h"
#include "Wait.h"
#include "RankingScroll.h"
#include "dayByDay.h"
#include "HelloWorldScene.h"

EnterRoom::EnterRoom()
{
	s_scale=1.189;
	s_no=1;
}

EnterRoom::~EnterRoom()
{
}

void EnterRoom::message_callback(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			auto MyInfo=MyInfoScene::create();
			Director::getInstance()->pushScene(MyInfo);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}
void EnterRoom::prize_callback(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			auto taskReward=TaskReward::create();
			Director::getInstance()->pushScene(taskReward);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}
void EnterRoom::rank_callback(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			auto FriendTalkBKG=LayerColor::create();
			FriendTalkBKG->setContentSize(Size(visibleSize.width,visibleSize.height));
			FriendTalkBKG->setColor(Color3B(28,120,135));
			FriendTalkBKG->setOpacity(50);
			FriendTalkBKG->setPosition(Vec2::ZERO);
			this->addChild(FriendTalkBKG,5,RANKSCROLL_LAYER);

			auto rankLayer=RankingScroll::create();
			FriendTalkBKG->addChild(rankLayer,1,NODE_CHILD_ID_ONE);
			auto RankingSize=Size(visibleSize.width*926/1218,visibleSize.height*547/716);
			EventListenerTouchOneByOne* SettingChooseListener=EventListenerTouchOneByOne::create();
			SettingChooseListener->setSwallowTouches(true);
			SettingChooseListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
			SettingChooseListener->onTouchMoved=[=](Touch* touch, Event* event){};
			SettingChooseListener->onTouchEnded=[=](Touch* touch, Event* event){
				float x=visibleSize.width/2-RankingSize.width/2;
				float y=visibleSize.height/2-RankingSize.height/2;
				Rect rect=Rect(x,y,RankingSize.width,RankingSize.height);
				if(!rect.containsPoint(touch->getLocation()))
				{
					auto deleteFunc=CallFunc::create([=](){
						if(this->getChildByTag(RANKSCROLL_LAYER))
							this->removeChildByTag(RANKSCROLL_LAYER,true);
					});
					this->runAction(deleteFunc);
				}
			};
			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(SettingChooseListener,FriendTalkBKG);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}

void EnterRoom::active_callback(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			auto activityScene=ActivityCenter::create();
			Director::getInstance()->pushScene(activityScene);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}
void EnterRoom::mall_callback(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			auto shopScene=ShopScene::create();
			Director::getInstance()->pushScene(shopScene);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}

}
void EnterRoom::set_callback(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			auto setScene=SetScene::create();
			Director::getInstance()->pushScene(setScene);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}

void EnterRoom::back_callback(Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			auto scene = Scene::create();
			auto startLayer=HelloWorld::create();
			scene->addChild(startLayer,1);
			Director::sharedDirector()->replaceScene(scene);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}

void EnterRoom::call_gold_prize()
{
	auto GetDayGold=LayerColor::create();
	GetDayGold->setContentSize(Size(visibleSize.width,visibleSize.height));
	//GetDayGold->setColor(Color3B(28,120,135));
	//GetDayGold->setOpacity(50);
	GetDayGold->setPosition(Vec2::ZERO);
	this->addChild(GetDayGold,5,THIS_GET_DAYGOLD_BKG);
	//GetDayGold->runAction(Sequence::create(DelayTime::create(0.8),TintTo::create(0.3,28,120,135),NULL));

	auto layerPerDay=new dayAward(this);
	layerPerDay->setScale(0);
	GetDayGold->addChild(layerPerDay,1,NODE_CHILD_ID_ONE);
	auto SpawnAction=Spawn::create(FadeIn::create(0.3),ScaleTo::create(0.3,1.1),NULL);
	layerPerDay->runAction(Sequence::create(DelayTime::create(0.8),SpawnAction,ScaleTo::create(0.1,1),NULL));

	EventListenerTouchOneByOne* GetDayGoldListener=EventListenerTouchOneByOne::create();
	GetDayGoldListener->setSwallowTouches(true);
	GetDayGoldListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	GetDayGoldListener->onTouchMoved=[=](Touch* touch, Event* event){};
	GetDayGoldListener->onTouchEnded=[=](Touch* touch, Event* event){
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(GetDayGoldListener,GetDayGold);

}
void EnterRoom::MessageSendCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto FeiendCharBKG=this->getChildByTag(THIS_FRIEND_CHAT_BKG);
			auto MessageEditBox=(EditBox*)FeiendCharBKG->getChildByTag(NODE_CHILD_ID_ONE)->getChildByTag(NODE_CHILD_ID_THREE);
			std::string editMessage=MessageEditBox->getText();
			if(editMessage.length()==0)
				CCLOG("Empty Message.");
			else
				CCLOG("%s",MessageEditBox->getText());
			MessageEditBox->setText("");
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void EnterRoom::FriendScCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto FriendTalkBKG=LayerColor::create();
			FriendTalkBKG->setContentSize(Size(visibleSize.width,visibleSize.height));
			FriendTalkBKG->setColor(Color3B(28,120,135));
			FriendTalkBKG->setOpacity(50);
			FriendTalkBKG->setPosition(Vec2::ZERO);
			this->addChild(FriendTalkBKG,5,THIS_FRIEND_CHAT_BKG);

			auto chatBkg=Sprite::createWithSpriteFrameName("chat_common_back.png");
			chatBkg->setAnchorPoint(Vec2(0.5,0.5));
			chatBkg->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
			FriendTalkBKG->addChild(chatBkg,1,NODE_CHILD_ID_ONE);
			auto chatSize=chatBkg->getTextureRect().size;

			auto chatRecordBKG=Sprite::createWithSpriteFrameName("chat_common_back_main.png");
			chatRecordBKG->setAnchorPoint(Vec2(0.5,0));
			chatRecordBKG->setPosition(Vec2(chatSize.width/2,chatSize.height*10/487));
			chatBkg->addChild(chatRecordBKG,1);

			auto CharRecord=Button::create("chat_common_table.png","chat_common_table_sel.png","chat_common_table.png",UI_TEX_TYPE_PLIST);
			CharRecord->setAnchorPoint(Vec2(0,0));
			CharRecord->setPosition(Vec2(chatSize.width*14.5/520,chatSize.height*420/487));
			CharRecord->addTouchEventListener(CC_CALLBACK_2(EnterRoom::ChatRecordCallBack,this));
			CharRecord->setTouchEnabled(false);
			CharRecord->setHighlighted(true);
			chatBkg->addChild(CharRecord,1,NODE_CHILD_ID_ONE);
			auto CommonLanguage=Button::create("chat_common_table.png","chat_common_table_sel.png","chat_common_table.png",UI_TEX_TYPE_PLIST);
			CommonLanguage->setAnchorPoint(Vec2(0,0));
			CommonLanguage->setPosition(Vec2(chatSize.width*206.5/620,chatSize.height*420/487));
			CommonLanguage->addTouchEventListener(CC_CALLBACK_2(EnterRoom::CommonLanguageCallBack,this));
			chatBkg->addChild(CommonLanguage,1,NODE_CHILD_ID_TWO);

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
			chatBkg->addChild(MessageBox,1,NODE_CHILD_ID_THREE);

			auto SendButton=Button::create("chat_send.png","chat_send_sel.png","chat_send.png",UI_TEX_TYPE_PLIST);
			SendButton->setAnchorPoint(Vec2(1,0.5));
			SendButton->setPosition(Vec2(chatSize.width*590/620,chatSize.height*52/487));
			SendButton->addTouchEventListener(CC_CALLBACK_2(EnterRoom::MessageSendCallBack,this));
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
						if(this->getChildByTag(THIS_FRIEND_CHAT_BKG))
							this->removeChildByTag(THIS_FRIEND_CHAT_BKG,true);
					});
					this->runAction(deleteFunc);
				}
			};
			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(SettingChooseListener,FriendTalkBKG);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void EnterRoom::MyFriendCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curButton=(Button*)pSender;
			auto curSc=this->getChildByTag(THIS_FRIEND_SCROLLVIEW);
			auto anotherButton=(Button*)this->getChildByTag(THIS_NEARPEOPLE_BUTTON);
			auto anotherSc=this->getChildByTag(THIS_NEARLY_SCROLLVIEW);	
			curButton->setHighlighted(true);
			curButton->setTouchEnabled(false);
			anotherButton->setHighlighted(false);
			anotherButton->setTouchEnabled(true);
			curSc->setVisible(true);
			anotherSc->setVisible(false);
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
void EnterRoom::NearPersonCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curButton=(Button*)pSender;
			auto curSc=this->getChildByTag(THIS_NEARLY_SCROLLVIEW);
			auto anotherButton=(Button*)this->getChildByTag(THIS_MYFEIEND_BUTTON);
			auto anotherSc=this->getChildByTag(THIS_FRIEND_SCROLLVIEW);
			curButton->setHighlighted(true);
			curButton->setTouchEnabled(false);
			anotherButton->setHighlighted(false);
			anotherButton->setTouchEnabled(true);
			curSc->setVisible(true);
			anotherSc->setVisible(false);
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
void EnterRoom::ChatRecordCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto FeiendCharBKG=this->getChildByTag(THIS_FRIEND_CHAT_BKG);
			auto anotherButton=(Button*)FeiendCharBKG->getChildByTag(NODE_CHILD_ID_ONE)->getChildByTag(NODE_CHILD_ID_TWO);
			curButton->setHighlighted(true);
			curButton->setTouchEnabled(false);
			anotherButton->setHighlighted(false);
			anotherButton->setTouchEnabled(true);
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
void EnterRoom::CommonLanguageCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto FeiendCharBKG=this->getChildByTag(THIS_FRIEND_CHAT_BKG);
			auto anotherButton=(Button*)FeiendCharBKG->getChildByTag(NODE_CHILD_ID_ONE)->getChildByTag(NODE_CHILD_ID_ONE);
			curButton->setHighlighted(true);
			curButton->setTouchEnabled(false);
			anotherButton->setHighlighted(false);
			anotherButton->setTouchEnabled(true);
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
void EnterRoom::MyDataUIPrepare()
{
	auto MyFhoto=Button::create("free_box.png","free_box.png","free_box.png",TextureResType::PLIST);//头像信息框
	MyFhoto->setAnchorPoint(Vec2(0,0));
	MyFhoto->setScale(1.03);
	MyFhoto->setPosition(Vec2(origin.x-visibleSize.height*0.005,origin.y+visibleSize.height*0.84));
	MyFhoto->addTouchEventListener(CC_CALLBACK_2(EnterRoom::myBaseInfo_callback,this));
	this->addChild(MyFhoto,1);
	auto FreeBox_pos=MyFhoto->getPosition();
	auto FreeBox_size=MyFhoto->getContentSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("userhead.plist");//测试用，删
	std::string PathOfPhoto="Head17.png";
	auto PhotoForPlayer=Sprite::createWithSpriteFrameName(PathOfPhoto);
	auto PhotoSize=PhotoForPlayer->getTextureRect().size;
	PhotoForPlayer->setScale(100/PhotoSize.width,100/PhotoSize.height);
	PhotoForPlayer->setAnchorPoint(Vec2(0.5,0.5));
	PhotoForPlayer->setPosition(Vec2(FreeBox_pos.x+FreeBox_size.width/2,FreeBox_pos.y+FreeBox_size.height/2));
	this->addChild(PhotoForPlayer,1,SPRITE_FOR_PHOTO);

	auto goodNameBar=Sprite::createWithSpriteFrameName("shang.png");
	goodNameBar->setAnchorPoint(Vec2(0,1));
	goodNameBar->setPosition(Vec2(origin.x+visibleSize.width*0.1065,origin.y+visibleSize.height*0.997));
	this->addChild(goodNameBar,1);

	auto goodName=Sprite::createWithSpriteFrameName("812.png");
	goodName->setAnchorPoint(Vec2(0,1));
	goodName->setPosition(Vec2(origin.x+visibleSize.width*0.105,origin.y+visibleSize.height*0.999));
	this->addChild(goodName,2);

	auto LabelForName=LabelTTF::create("TempName","Arial",20);
	LabelForName->setAnchorPoint(Vec2(0,1));
	LabelForName->setPosition(Vec2(origin.x+visibleSize.width*0.15,origin.y+visibleSize.height*0.984));
	this->addChild(LabelForName,2,LABEL_FOR_PLAYERNAME);
	
	auto competeBar=Sprite::createWithSpriteFrameName("middle.png");
	competeBar->setAnchorPoint(Vec2(0,1));
	competeBar->setPosition(Vec2(origin.x+visibleSize.width*0.1065,origin.y+visibleSize.height*0.9423));
	this->addChild(competeBar,1);

	auto compete=Sprite::createWithSpriteFrameName("jinbi.png");
	compete->setAnchorPoint(Vec2(0,1));
	compete->setPosition(Vec2(origin.x+visibleSize.width*0.107,origin.y+visibleSize.height*0.94));
	compete->setScale(0.5f);
	this->addChild(compete,2);

	auto LabelForGold=LabelTTF::create("5000","Arial",20);
	LabelForGold->setAnchorPoint(Vec2(0,1));
	LabelForGold->setPosition(Vec2(origin.x+visibleSize.width*0.15,origin.y+visibleSize.height*0.93));
	this->addChild(LabelForGold,2,LABEL_FOR_GOLD);

	auto buttonPlus=Button::create("jiahao.png","jiahao.png","jiahao.png",UI_TEX_TYPE_PLIST);
	buttonPlus->setAnchorPoint(Vec2(0,1));
	buttonPlus->setPosition(Vec2(origin.x+visibleSize.width*0.2632,origin.y+visibleSize.height*0.943));
	buttonPlus->addTouchEventListener(CC_CALLBACK_2(EnterRoom::mall_callback,this));
	this->addChild(buttonPlus,2);

	auto TaskBar=Sprite::createWithSpriteFrameName("xia.png");
	TaskBar->setAnchorPoint(Vec2(0,1));
	TaskBar->setPosition(Vec2(origin.x+visibleSize.width*0.1065,origin.y+visibleSize.height*0.891));
	this->addChild(TaskBar,1);

	auto task=Sprite::createWithSpriteFrameName("juan.png");
	task->setAnchorPoint(Vec2(0,1));
	task->setPosition(Vec2(origin.x+visibleSize.width*0.1115,origin.y+visibleSize.height*0.89));
	this->addChild(task,2);

	auto LabelForQuan=LabelTTF::create("5000","Arial",20);
	LabelForQuan->setAnchorPoint(Vec2(0,1));
	LabelForQuan->setPosition(Vec2(origin.x+visibleSize.width*0.15,visibleSize.height*0.88));
	this->addChild(LabelForQuan,2,LABEL_FOR_QUAN);
}
void EnterRoom::FunctionBarUIPrepare()
{
	float y= visibleSize.height*0.855+origin.y;
	auto button_bar= Sprite::createWithSpriteFrameName("button_bar.png");
	button_bar->setAnchorPoint(Point(1,0));
	button_bar->setPosition(Point(origin.x+visibleSize.width*1.001,origin.y+visibleSize.height*0.838));
	this->addChild(button_bar,1);

	auto buttonMessage=ui::Button::create("message.png","message.png","message.png",TextureResType::PLIST);
	buttonMessage->setAnchorPoint(Vec2(0,0));
	buttonMessage->setPosition(Vec2(origin.x+visibleSize.width*0.35,y));
	buttonMessage->addTouchEventListener(CC_CALLBACK_2(EnterRoom::message_callback,this));
	this->addChild(buttonMessage,2);

	auto buttonReward=ui::Button::create("prize.png","prize.png","prize.png",TextureResType::PLIST);
	buttonReward->setAnchorPoint(Vec2(0,0));
	buttonReward->setPosition(Vec2(origin.x+visibleSize.width*0.45,y));
	buttonReward->addTouchEventListener(CC_CALLBACK_2(EnterRoom::prize_callback,this));
	this->addChild(buttonReward,2);

	auto buttonRank=ui::Button::create("cup.png","cup.png","cup.png",TextureResType::PLIST);//REWRITE
	buttonRank->setAnchorPoint(Vec2(0,0));
	buttonRank->setPosition(Vec2(origin.x+visibleSize.width*0.55,y));
	buttonRank->addTouchEventListener(CC_CALLBACK_2(EnterRoom::rank_callback,this));
	this->addChild(buttonRank,2);

	auto buttonActivity=ui::Button::create("activity_box.png","activity_box.png","activity_box.png",TextureResType::PLIST);
	buttonActivity->setAnchorPoint(Vec2(0,0));
	buttonActivity->setPosition(Vec2(origin.x+visibleSize.width*0.65,y));
	buttonActivity->addTouchEventListener(CC_CALLBACK_2(EnterRoom::active_callback,this));
	this->addChild(buttonActivity,2);

	auto buttonMall=ui::Button::create("mall.png","mall.png","mall.png",TextureResType::PLIST);
	buttonMall->setAnchorPoint(Vec2(0,0));
	buttonMall->setPosition(Vec2(origin.x+visibleSize.width*0.75,y));
	buttonMall->addTouchEventListener(CC_CALLBACK_2(EnterRoom::mall_callback,this));
	this->addChild(buttonMall,2);

	auto buttonSet=ui::Button::create("set.png","set.png","set.png",TextureResType::PLIST);
	buttonSet->setAnchorPoint(Vec2(0,0));
	buttonSet->setPosition(Vec2(origin.x+visibleSize.width*0.85,y));
	buttonSet->addTouchEventListener(CC_CALLBACK_2(EnterRoom::set_callback,this));
	this->addChild(buttonSet,2);

	auto buttonBack=ui::Button::create("back.png","back.png","back.png",TextureResType::PLIST);
	buttonBack->setAnchorPoint(Vec2(0,0));
	buttonBack->setPosition(Vec2(origin.x+visibleSize.width*0.932,y));
	buttonBack->addTouchEventListener(CC_CALLBACK_2(EnterRoom::back_callback,this));
	this->addChild(buttonBack,2);
}
void EnterRoom::competeButtons()
{
	auto NormalRace=Button::create("game_blood_cg.png","game_blood_cg.png","game_blood_cg.png",UI_TEX_TYPE_PLIST);
	NormalRace->setAnchorPoint(Vec2(0.5,0.5));
	NormalRace->setPosition(Vec2(origin.x+visibleSize.width*0.77,origin.y+visibleSize.height*0.623));
	NormalRace->addTouchEventListener(CC_CALLBACK_2(EnterRoom::normal_callback,this));
	this->addChild(NormalRace,1);
	auto CompeteRace=Button::create("game_blood_bs.png","game_blood_bs.png","game_blood_bs.png",UI_TEX_TYPE_PLIST);
	CompeteRace->setAnchorPoint(Vec2(0.5,0.5));
	CompeteRace->setPosition(Vec2(origin.x+visibleSize.width*0.77,origin.y+visibleSize.height*0.414));
	CompeteRace->addTouchEventListener(CC_CALLBACK_2(EnterRoom::compete_callback,this));
	this->addChild(CompeteRace,1);
	auto PleaseWaiting=Button::create("game_await.png","game_await.png","game_await.png",UI_TEX_TYPE_PLIST);
	PleaseWaiting->setAnchorPoint(Vec2(0.5,0.5));
	PleaseWaiting->setPosition(Vec2(origin.x+visibleSize.width*0.77,origin.y+visibleSize.height*0.205));
	this->addChild(PleaseWaiting,1);
}
void EnterRoom::FriendAndStrangers()//photo sex name Lv score 
{
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

	auto FriendSC = ui::ScrollView::create(); 
	FriendSC->setSize(Size(visibleSize.width*0.4334975,visibleSize.height*0.5726257));
	FriendSC->setBounceEnabled(true);
	FriendSC->setDirection(ui::ScrollView::Direction::VERTICAL); 
	FriendSC->setAnchorPoint(Vec2(0.5,0));
	FriendSC->setPosition(Vec2(visibleSize.width*0.27358,45));
	FriendSC->setInnerContainerSize(Size(528,1640));
	FriendSC->setScale(0,1);
	this->addChild(FriendSC,2,THIS_FRIEND_SCROLLVIEW);
	auto FriendInSize=Size(visibleSize.width*0.4334975,visibleSize.height*2.29);
	for(int i=0;i<20;i++)//int FriendNum
	{
		auto FriendBar=Button::create("Friends_cellBg.png","Friends_cellBg.png","Friends_cellBg.png",UI_TEX_TYPE_PLIST);
		FriendBar->setAnchorPoint(Vec2(0,1));
		FriendBar->setPosition(Vec2(0,FriendInSize.height*(1-i*0.05)));
		FriendBar->addTouchEventListener(CC_CALLBACK_2(EnterRoom::FriendScCallBack,this));
		FriendSC->addChild(FriendBar,1);
	}
	auto FriendScAction=EaseBounceIn::create(Sequence::create(DelayTime::create(0.5),ScaleTo::create(0.3,1),NULL));
	FriendSC->runAction(FriendScAction);

	auto NearPeopel = ui::ScrollView::create(); 
	NearPeopel->setSize(Size(visibleSize.width*0.4334975,visibleSize.height*0.5726257));
	NearPeopel->setBounceEnabled(true);
	NearPeopel->setDirection(ui::ScrollView::Direction::VERTICAL); 
	NearPeopel->setAnchorPoint(Vec2(0.5,0));
	NearPeopel->setPosition(Vec2(visibleSize.width*0.27358,45));
	NearPeopel->setInnerContainerSize(Size(528,1640));
	NearPeopel->setVisible(false);
	this->addChild(NearPeopel,3,THIS_NEARLY_SCROLLVIEW);
	auto NearPeopelInSize=Size(visibleSize.width*0.4334975,visibleSize.height*2.29);
	for(int i=0;i<20;i++)//int FriendNum
	{
		auto FriendBar=Button::create("Friends_cellBg.png","Friends_cellBg.png","Friends_cellBg.png",UI_TEX_TYPE_PLIST);
		FriendBar->setAnchorPoint(Vec2(0,1));
		FriendBar->setPosition(Vec2(0,NearPeopelInSize.height*(1-i*0.05)));
		FriendBar->addTouchEventListener(CC_CALLBACK_2(EnterRoom::FriendScCallBack,this));
		NearPeopel->addChild(FriendBar,1);
	}

	auto CharRecord=Button::create("Friends_friendRankBtn.png","Friends_friendBtnSel.png","Friends_friendRankBtn.png",UI_TEX_TYPE_PLIST);
	CharRecord->setAnchorPoint(Vec2(0.5,0.5));
	CharRecord->setPosition(Vec2(visibleSize.width*0.118396,visibleSize.height*0.6676));
	CharRecord->addTouchEventListener(CC_CALLBACK_2(EnterRoom::MyFriendCallBack,this));
	CharRecord->setTouchEnabled(false);
	CharRecord->setHighlighted(true);
	CharRecord->setScale(0,1);
	this->addChild(CharRecord,2,THIS_MYFEIEND_BUTTON);
	auto CharRecordAction=EaseBounceIn::create(Sequence::create(DelayTime::create(0.5),ScaleTo::create(0.3,1),NULL));
	CharRecord->runAction(CharRecordAction);
	auto CommonLanguage=Button::create("Friends_friendRankBtn.png","Friends_friendBtnSel.png","Friends_friendRankBtn.png",UI_TEX_TYPE_PLIST);
	CommonLanguage->setAnchorPoint(Vec2(0.5,0.5));
	CommonLanguage->setPosition(Vec2(visibleSize.width*0.241549,visibleSize.height*0.6676));
	CommonLanguage->addTouchEventListener(CC_CALLBACK_2(EnterRoom::NearPersonCallBack,this));
	CommonLanguage->setScale(0,1);
	this->addChild(CommonLanguage,2,THIS_NEARPEOPLE_BUTTON);
	auto CommonLanguageAction=EaseBounceIn::create(Sequence::create(DelayTime::create(0.5),ScaleTo::create(0.3,1),NULL));
	CommonLanguage->runAction(CommonLanguageAction);

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
}
//void EnterRoom::addRes()
//{
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ChatImg.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("FriendsImage.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("selectGameAndScene.plist");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dengluzhutu.plist");
//}
bool EnterRoom::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	Sprite *sprite;
	if(s_no == 1)
		sprite = Sprite::create("loadwin.png");
	else
		sprite = Sprite::create("loadwin2.png");
	sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	sprite->setScaleX(s_scale);
	sprite->setScaleY(s_scale);
	this->addChild(sprite, 0);

	//addRes();//加载plist
	MyDataUIPrepare();//头像等个人信息
	FunctionBarUIPrepare();//功能

	auto blackbar= Sprite::create("blackbar.png");//弹幕条
	blackbar->setAnchorPoint(Vec2(0.5,0));
	blackbar->setPosition(Point(origin.x+visibleSize.width/2+30,origin.y+visibleSize.height*0.76));
	this->addChild(blackbar,1);

	FriendAndStrangers();//好友列表
	competeButtons();//比赛按钮
	/////////////////////////////////////////////////////////////////////

	ifDayAwardGetted=false;
	if(!ifDayAwardGetted)
		call_gold_prize();

	return true;
}

void EnterRoom::normal_callback(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	auto commonButton=(Button*)pSender;
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		commonButton->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			commonButton->setTouchEnabled(false);
			auto TargetForButton=TargetedAction::create(commonButton,ScaleTo::create(0.1,1));
			auto entranceToCommonField=CallFunc::create([=](){
				auto commonField=CommonField::create();
				Director::getInstance()->replaceScene(commonField);
			});
			this->runAction(Sequence::create(TargetForButton,entranceToCommonField,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		commonButton->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void EnterRoom::compete_callback(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	auto competeButton=(Button*)pSender;
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		competeButton->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			competeButton->setTouchEnabled(false);
			auto TargetForButton=TargetedAction::create(competeButton,ScaleTo::create(0.1,1));
			auto entranceToCompeteField=CallFunc::create([=](){
				auto competitionField=CompetitionField::create();
				Director::getInstance()->replaceScene(competitionField);
			});
			this->runAction(Sequence::create(TargetForButton,entranceToCompeteField,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		competeButton->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

//void EnterRoom::fastRace_callback(cocos2d::Ref* pSender,Widget::TouchEventType type)
//{
//	auto fastRaceButton=(Button*)pSender;
//	auto shade=(Sprite*)this->getChildByTag(22);
//	switch(type)
//	{
//	case Widget::TouchEventType::BEGAN:
//		fastRaceButton->runAction(ScaleTo::create(0.1,1.1));
//		shade->runAction(ScaleTo::create(0.1,1.1));
//		break;
//	case Widget::TouchEventType::MOVED:
//		break;
//	case Widget::TouchEventType::ENDED:
//		fastRaceButton->runAction(Sequence::create(ScaleTo::create(0.1,1),CallFunc::create(this,callfunc_selector(EnterRoom::fastRaceAction)),NULL));
//		shade->runAction(ScaleTo::create(0.1,1));
//		scheduleOnce(schedule_selector(EnterRoom::entranceToFastRace),0.5);
//		break;
//	case Widget::TouchEventType::CANCELED:
//		fastRaceButton->runAction(ScaleTo::create(0.1,1));
//		shade->runAction(ScaleTo::create(0.1,1));
//		break;
//	}
//}

void EnterRoom::myBaseInfo_callback(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			auto myBaseInfo=BaseScene::create();
			Director::getInstance()->pushScene(myBaseInfo);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	}
}