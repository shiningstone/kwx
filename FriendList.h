#ifndef FRIEND_LIST_H
#define FRIEND_LIST_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

//#include "EnterRoomScene.h"
#include "LoginType.h"

class FriendList : public Layer
{
#define FRIEND_BTN					1
#define NEAR_BTN					2
#define STRANGER_BTN				3

#define FRIEND_LIST					4
#define NEAR_LIST					5
#define STRANGER_LIST				6

#define FRIEND_CHAT_BKG				7

#define CHAT_CHILD_BAK				1
#define CHATRECORD_BTN				2
#define COMMON_BTN					3
#define CHAT_EDIT_BOX				4
public:
	FriendList(Friend_Info friendsList[],OtherPlayers_Info NearbyPlayers[],OtherPlayers_Info Stranger[]);
	~FriendList(void);

	Size visibleSize;
	Vec2 origin;
	virtual bool init();

	void ListBtnCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void FriendCellBack(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void ChatOneToOneCreate();

	void ChatRecordCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void CommonLanguageCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void MessageSendCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type);
private:
	//Vector <Friend_Info> MyFriendList;
	//Vector <OtherPlayers_Info> NearyPeopleList;
	//Vector <OtherPlayers_Info> StrangersList;
};

#endif