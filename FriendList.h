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

#define FRIEND_ADD_BTN				7
#define FRIEND_DELETE_BKG			8

#define FRIEND_DELETE_BTN			1
public:
	FriendList(EnterRoom* p);
	~FriendList(void);

	Size visibleSize;
	Vec2 origin;
	virtual bool init();

	Layout* friendCell_create(int curNo);
	void friendList_Create();
	void nearyList_Create();
	void strangerList_Create();

	void ListBtnCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//列表切换按钮
	void Friend_Invitation(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//好友邀请按钮
	void listItemCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//好友Item回调
	void ListEventCall(cocos2d::Ref *pSender,ListViewEventType type);//listView的监听
	void friendDeleteCall(cocos2d::Ref *pSender,cocos2d::ui::Widget::TouchEventType type);//好友删除按钮

	void deleteBtn_Hint_create(float delta);//删除好友提示创建
	void deleteBtn_Hint_delete();//删除好友提示删除
	void InsertFriendItem(Layout* curItem,int insetNo);//插入一个好友
	void DeleteFriendItem(int deleteNo);//删除一个好友
private:
	EnterRoom* parentNode;
	int curSelectedItem;
	Point curTouchPos;
	std::vector <Friend_Info> MyFriendList;	
	std::vector <OtherPlayers_Info> NearyPeopleList;
	std::vector <OtherPlayers_Info> StrangersList;

};

#endif