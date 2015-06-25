#ifndef __ENTERROOM_SCENE_H__
#define __ENTERROOM_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
//#include "LoginType.h"
#include "network/KwxEnv.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;


class EnterRoom : public cocos2d::Layer
{
public:
	EnterRoom();
	virtual ~EnterRoom();
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
#define NODE_CHILD_ID_ONE			1	
#define NODE_CHILD_ID_TWO			2	
#define NODE_CHILD_ID_THREE			3
#define MAX_CONTINE_LOAD_DAYS		7

#define THIS_FRIEND_CHAT_BKG		1
#define THIS_MYFEIEND_BUTTON		2
#define THIS_NEARPEOPLE_BUTTON		3
#define THIS_STRANGERS_BUTTON		4
#define	THIS_FRIEND_SCROLLVIEW		5
#define	THIS_NEARLY_SCROLLVIEW		6
#define THIS_GET_DAYGOLD_BKG		7
#define RANKSCROLL_LAYER			8
#define CREATE_ROLE_LAYER			9

#define LABEL_FOR_PLAYERNAME		8
#define LABEL_FOR_GOLD				9
#define LABEL_FOR_QUAN				10
#define SPRITE_FOR_PHOTO			11
	Size visibleSize;
	Point origin;
	float s_scale;
	int s_no;
	bool ifDayAwardGetted;
	/////////////////////////////////////////////////
	//int cur_days;
	//int cur_status;
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	//void addRes();
	void MyDataUIPrepare();
	void FunctionBarUIPrepare();
	void competeButtons();
	//void FriendAndStrangers();
	void myBaseInfo_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);		//ReWrite 
	void mall_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void message_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void prize_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void rank_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);				//ReWrite
	void active_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void set_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void back_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);				//ReWrite
	void normal_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);			//ReWrite
	void compete_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);			//ReWrite

	void call_gold_prize();																//ReWrite

	void createRole();
		
	CREATE_FUNC(EnterRoom);
private:
	MainLayer_myInfo personalSimpleInfo;
};
#endif  //__ENTERROOM_SCENE_H__

