
#include "network/KwxEnv.h"

#include "ChatOneToOne.h"


ChatOneToOne::ChatOneToOne(EnterRoom* p,Friend_Info curFriend)
{
	parentNode=p;
	chatFriend=curFriend;
	init();
}
ChatOneToOne::~ChatOneToOne()
{
}
bool ChatOneToOne::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();
	chatSize=Size(visibleSize.width*0.4770115,visibleSize.height*0.4469274);

	auto FriendTalkBKG=LayerColor::create();
	FriendTalkBKG->setContentSize(Size(visibleSize.width,visibleSize.height));
	FriendTalkBKG->setColor(ccWHITE);
	//FriendTalkBKG->setColor(Color3B(28,120,135));
	//FriendTalkBKG->setOpacity(50);
	FriendTalkBKG->ignoreAnchorPointForPosition(false);
	FriendTalkBKG->setAnchorPoint(Vec2(0.5,0.5));
	FriendTalkBKG->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(FriendTalkBKG,0);

	auto chatBkg=Sprite::createWithSpriteFrameName("chat_common_back.png");
	chatBkg->setAnchorPoint(Vec2(0.5,0.5));
	chatBkg->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(chatBkg,1);
	auto chatBkgSize=chatBkg->getContentSize();

	EventListenerTouchOneByOne* SettingChooseListener=EventListenerTouchOneByOne::create();
	SettingChooseListener->setSwallowTouches(true);
	SettingChooseListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	SettingChooseListener->onTouchMoved=[=](Touch* touch, Event* event){};
	SettingChooseListener->onTouchEnded=[=](Touch* touch, Event* event){
		float x=visibleSize.width/2-chatBkgSize.width/2;
		float y=visibleSize.height/2-chatBkgSize.height/2;
		Rect rect=Rect(x,y,chatBkgSize.width,chatBkgSize.height);
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

	auto chatRecordBKG=Sprite::createWithSpriteFrameName("chat_common_back_main.png");
	chatRecordBKG->setAnchorPoint(Vec2(0.5,0));
	chatRecordBKG->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.175419));
	this->addChild(chatRecordBKG,1);

	auto ChatLogView=ListView::create();
	ChatLogView->setDirection(SCROLLVIEW_DIR_VERTICAL);//SCROLLVIEW_DIR_VERTICAL
	ChatLogView->setGravity(LISTVIEW_GRAVITY_CENTER_VERTICAL);
	ChatLogView->setTouchEnabled(true);
	ChatLogView->setBounceEnabled(false);
	ChatLogView->setSize(chatSize);
	ChatLogView->ignoreAnchorPointForPosition(false);
	ChatLogView->setAnchorPoint(Vec2(0,0));
	ChatLogView->setPosition(Vec2(visibleSize.width*0.26,chatRecordBKG->getPosition().y+90*1218/visibleSize.width));
	ChatLogView->setItemsMargin(10);
	this->addChild(ChatLogView,2,CHAT_LOG_VIEW);

	getLocalChatLog();
	for(int a=0;a<chatLog.size();a++)
	{
		auto curCellItem=Message_cell_create(chatLog[a]);
		ChatLogView->pushBackCustomItem(curCellItem);
	}
	receiveChatMessage("asdfasdfasdfasd");
	ChatLogView->jumpToBottom();

	auto CharRecord=Button::create("chat_chatrecord.png","chat_chatrecord_sel.png","chat_chatrecord_sel.png",UI_TEX_TYPE_PLIST);
	CharRecord->setAnchorPoint(Vec2(0,0));
	CharRecord->setPosition(Vec2(origin.x+visibleSize.width*0.2589033,origin.y+visibleSize.height*0.7475207));
	CharRecord->addTouchEventListener(CC_CALLBACK_2(ChatOneToOne::ChatBtnCallBack,this));
	CharRecord->setTouchEnabled(false);
	CharRecord->setHighlighted(true);
	this->addChild(CharRecord,2,CHATRECORD_BTN);
	auto CommonLanguage=Button::create("chat_commonz_language.png","chat_commonz_language_sel.png","chat_commonz_language_sel.png",UI_TEX_TYPE_PLIST);
	CommonLanguage->setAnchorPoint(Vec2(0,0));
	CommonLanguage->setPosition(Vec2(origin.x+visibleSize.width*0.4147505,origin.y+visibleSize.height*0.7475207));
	CommonLanguage->addTouchEventListener(CC_CALLBACK_2(ChatOneToOne::ChatBtnCallBack,this));
	this->addChild(CommonLanguage,2,COMMON_BTN);

	auto MessangeEdit=Sprite::createWithSpriteFrameName("chat_text_input_area.png");
	MessangeEdit->setAnchorPoint(Vec2::ZERO);
	MessangeEdit->setPosition(Vec2(origin.x+visibleSize.width*0.2566065,origin.y+visibleSize.height*0.1753482));
	MessangeEdit->setOpacity(50);
	this->addChild(MessangeEdit,3);

	auto MessageBox=EditBox::create(Size(visibleSize.width*0.29,visibleSize.height*0.042),Scale9Sprite::create());
	MessageBox->setAnchorPoint(Vec2(0,0));
	MessageBox->setPosition(Vec2(origin.x+visibleSize.width*0.32703,origin.y+visibleSize.height*0.2005911));
	MessageBox->setFontColor(ccBLACK);
	MessageBox->setPlaceHolder("请在这里输入聊天内容...");
	MessageBox->setMaxLength(15);
	MessageBox->setInputMode(cocos2d::extension::EditBox::InputMode::ANY);
	this->addChild(MessageBox,3,CHAT_EDIT_BOX);

	auto SendButton=Button::create("chat_send.png","chat_send_sel.png","chat_send.png",UI_TEX_TYPE_PLIST);
	SendButton->setAnchorPoint(Vec2(1,0.5));
	SendButton->setPosition(Vec2(origin.x+visibleSize.width*0.7306266,origin.y+visibleSize.height*0.2314435));
	SendButton->addTouchEventListener(CC_CALLBACK_2(ChatOneToOne::MessageSendCallBack,this));
	this->addChild(SendButton,3);

	return true;
}
void ChatOneToOne::ChatBtnCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			auto curBtn=(Button*)pSender;
			auto anotherBtn=(Button*)this->getChildByTag(curBtn->getTag()%2+1);
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
void ChatOneToOne::MessageSendCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto editKuang=(EditBox*)this->getChildByTag(CHAT_EDIT_BOX);
			auto listChat=(ListView*)this->getChildByTag(CHAT_LOG_VIEW);
			std::string curMes=editKuang->getText();
			if(curMes!="")
			{
				auto mesSend=CallFunc::create([=](){
					auto savedMes=Message_local_save(curMes,true);
					auto curCellItem=Message_cell_create(savedMes);
					listChat->pushBackCustomItem(curCellItem);
					editKuang->setText("");
				});
				auto ToBottom=CallFunc::create([=](){
					listChat->jumpToBottom();
				});
				this->runAction(Sequence::create(mesSend,ToBottom,NULL));
			}
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void ChatOneToOne::getLocalChatLog()
{
	auto userDefault=UserDefault::getInstance();
	for(int a=0;;a++)
	{
		char tempChar[30];
		std::string tempStr="";
		sprintf(tempChar,"%s%d%d",KEY_MIDDLE_WORD,chatFriend._id,a);
		tempStr=userDefault->getStringForKey(tempChar,"");
		if(tempStr=="")
			break;
		else
			chatLog.push_back(tempStr);
	}
}
Layout* ChatOneToOne::Message_cell_create(std::string message)
{
	std::string flagMes=message.substr(0,11);
	std::string realMes=message.substr(11,message.size());
	int RowNum=0;
	int ColumnNum=0;

	auto otherPhoto=Sprite::createWithSpriteFrameName(chatFriend.photoFileName);
	auto myPhoto=Sprite::createWithSpriteFrameName((char *)EnvVariable::getInstance()->_dailyLogin.image);

	auto chatMes=LabelTTF::create(realMes,"Arial",20);
	chatMes->setContentSize(Size(chatSize.width*0.7,0));
	chatMes->setColor(ccBLACK);

	RowAndColumn(chatMes->getContentSize(),RowNum,ColumnNum);

	auto cellSize=Size(chatSize.width,RowNum*20*716/visibleSize.height);
	auto curItem=Layout::create();
	curItem->setTouchEnabled(false);
	curItem->setSize(cellSize);

	if(flagMes==LOCAL_MY_MES)
	{
		myPhoto->setScale(60/myPhoto->getTextureRect().size.width,60/myPhoto->getTextureRect().size.height);
		myPhoto->setAnchorPoint(Vec2(1,1));
		myPhoto->setPosition(Vec2(cellSize.width,cellSize.height));
		curItem->addChild(myPhoto);

		float startPointX=myPhoto->getPosition().x-myPhoto->getContentSize().width*myPhoto->getScaleX();
		float x,y;
		for(int a=1;a<=RowNum;a++)
		{
			x=startPointX;
			y=cellSize.height-(a-1)*20*716/visibleSize.height;
			for(int b=1;b<=ColumnNum;b++)
			{
				Point anchorPoint=Vec2(1,1);
				Point curPosition=Vec2(x-(b-1)*24*1218/visibleSize.width,y);
				Sprite* squareItem;
				if(a==1)
				{
					if(b==1)
						squareItem=Sprite::create("3_Green.png");
					else if(b==ColumnNum)
						squareItem=Sprite::create("1_Green.png");
					else
						squareItem=Sprite::create("2_Green.png");
				}
				else if(a==RowNum)
				{
					if(b==1)
						squareItem=Sprite::create("9_Green.png");
					else if(b==ColumnNum)
						squareItem=Sprite::create("7_Green.png");
					else
						squareItem=Sprite::create("8_Green.png");
				}
				else
				{
					if(b==1)
					{
						if(a==2)
							squareItem=Sprite::create("6_Green.png");
						else
						{
							squareItem=Sprite::create("10_Green.png");
						}
					}
					else if(b==ColumnNum)
						squareItem=Sprite::create("4_Green.png");
					else
						squareItem=Sprite::create("5_Green.png");
				}
				squareItem->setAnchorPoint(anchorPoint);
				squareItem->setPosition(curPosition);
				curItem->addChild(squareItem);
			}
		}
		chatMes->setAnchorPoint(Vec2(0.5,0.5));
		chatMes->setPosition(Vec2(startPointX-ColumnNum*24*1218/visibleSize.width/2,cellSize.height/2));
		curItem->addChild(chatMes);
	}
	else if(flagMes==LOCAL_OTHER_MES)
	{
		otherPhoto->setScale(60/myPhoto->getTextureRect().size.width,60/myPhoto->getTextureRect().size.height);
		otherPhoto->setAnchorPoint(Vec2(0,1));
		otherPhoto->setPosition(Vec2(0,cellSize.height));
		curItem->addChild(otherPhoto);

		float startPointX=otherPhoto->getPosition().x+otherPhoto->getContentSize().width*otherPhoto->getScaleX();
		float x,y;
		for(int a=1;a<=RowNum;a++)
		{
			x=startPointX;
			y=cellSize.height-(a-1)*20*716/visibleSize.height;
			for(int b=1;b<=ColumnNum;b++)
			{
				Point anchorPoint=Vec2(0,1);
				Point curPosition=Vec2(x+(b-1)*24*1218/visibleSize.width,y);
				Sprite* squareItem;
				if(a==1)
				{
					if(b==1)
						squareItem=Sprite::create("1_White.png");
					else if(b==ColumnNum)
						squareItem=Sprite::create("3_White.png");
					else
						squareItem=Sprite::create("2_White.png");
				}
				else if(a==RowNum)
				{
					if(b==1)
						squareItem=Sprite::create("7_White.png");
					else if(b==ColumnNum)
						squareItem=Sprite::create("9_White.png");
					else
						squareItem=Sprite::create("8_White.png");
				}
				else
				{
					if(b==1)
					{
						if(a==2)
							squareItem=Sprite::create("4_White.png");
						else
						{
							squareItem=Sprite::create("10_White.png");
						}
					}
					else if(b==ColumnNum)
						squareItem=Sprite::create("6_White.png");
					else
						squareItem=Sprite::create("5_White.png");
				}
				squareItem->setAnchorPoint(anchorPoint);
				squareItem->setPosition(curPosition);
				curItem->addChild(squareItem);
			}
		}
		chatMes->setAnchorPoint(Vec2(0.5,0.5));
		chatMes->setPosition(Vec2(startPointX+ColumnNum*24*1218/visibleSize.width/2,cellSize.height/2));
		curItem->addChild(chatMes);
	}
	else 
	{
		return false;
	}

	return curItem;
}
void ChatOneToOne::RowAndColumn(Size labelSize,int &row,int &column)
{
	float tempRowTime=(float)(labelSize.height/20);
	int Row_IntegralPart=(int)tempRowTime;
	int Row_DecimalPart=((tempRowTime-Row_IntegralPart)>0)?1:0;
	if(tempRowTime<3)
		row=3;
	else
		row=Row_IntegralPart+Row_DecimalPart;

	float tempColumnTime=(float)((labelSize.width+20)/24);
	int Column_IntegralPart=(int)tempColumnTime;
	int Column_DecimalPart=((tempColumnTime-Column_IntegralPart)>0)?1:0;
	if(tempColumnTime<2)
		column=2;
	else
		column=tempColumnTime+Column_DecimalPart;
}
std::string ChatOneToOne::Message_local_save(std::string curMes,bool ifMySend)
{
	auto userDefault=UserDefault::getInstance();
	char tempChar[30];
	std::string saveMes="";
	if(ifMySend)
		saveMes=LOCAL_MY_MES+curMes;
	else
		saveMes=LOCAL_OTHER_MES+curMes;
	sprintf(tempChar,"%s%d%d",KEY_MIDDLE_WORD,chatFriend._id,chatLog.size());
	userDefault->setStringForKey(tempChar,saveMes);
	userDefault->flush();
	chatLog.push_back(saveMes);
	return saveMes;
}
void ChatOneToOne::receiveChatMessage(std::string receiveStr)
{
	auto listChat=(ListView*)this->getChildByTag(CHAT_LOG_VIEW);
	auto realMes=Message_local_save(receiveStr,false);
	auto receiveItem=Message_cell_create(realMes);
	listChat->pushBackCustomItem(receiveItem);
}