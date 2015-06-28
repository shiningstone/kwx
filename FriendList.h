#ifndef FRIEND_LIST_H
#define FRIEND_LIST_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

//#include "EnterRoomScene.h"
#include "LoginType.h"
#include <vector>
using namespace std;

#include "network/KwxEnv.h"
#include "EnterRoomScene.h"

class FriendList : public Layer
{
#define FRIEND_BTN					1
#define NEAR_BTN					2
#define STRANGER_BTN				3

#define FRIEND_LIST					4
#define NEAR_LIST					5
#define STRANGER_LIST				6

#define CHAT_CHILD_BAK				1
#define CHATRECORD_BTN				1
#define COMMON_BTN					2
#define CHAT_EDIT_BOX				3
public:
	FriendList(EnterRoom* p);
	~FriendList(void);

	Size visibleSize;
	Vec2 origin;
	virtual bool init();

	void ChatOneToOneCreate(Friend_Info chatFriend);//聊天窗口

	void ListBtnCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//列表切换按钮
	void listItemCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//好友Item回调

	void ChatBtnCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type);//聊天窗口切换按钮
	void MessageSendCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type);//消息发送


	void insertItemForFriend();					//好友列表刷新
	void ListEventCall(cocos2d::Ref *pSender,ListViewEventType type);

	void updateTest(float delt);
	void updateTest1(float delt);
	//CREATE_FUNC(FriendList);
private:
	EnterRoom* parentNode;
	std::vector <Friend_Info> MyFriendList;	
	std::vector <OtherPlayers_Info> NearyPeopleList;
	std::vector <OtherPlayers_Info> StrangersList;

};

#endif