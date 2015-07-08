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

	void ListBtnCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//�б��л���ť
	void Friend_Invitation(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//�������밴ť
	void listItemCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//����Item�ص�
	void ListEventCall(cocos2d::Ref *pSender,ListViewEventType type);//listView�ļ���
	void friendDeleteCall(cocos2d::Ref *pSender,cocos2d::ui::Widget::TouchEventType type);//����ɾ����ť

	void deleteBtn_Hint_create(float delta);//ɾ��������ʾ����
	void deleteBtn_Hint_delete();//ɾ��������ʾɾ��
	void InsertFriendItem(Layout* curItem,int insetNo);//����һ������
	void DeleteFriendItem(int deleteNo);//ɾ��һ������
private:
	EnterRoom* parentNode;
	int curSelectedItem;
	Point curTouchPos;
	std::vector <Friend_Info> MyFriendList;	
	std::vector <OtherPlayers_Info> NearyPeopleList;
	std::vector <OtherPlayers_Info> StrangersList;

};

#endif