#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

//#include "LoginType.h"
#include "network/KwxEnv.h"

class MyXbox :	public Layer
{
#define MY_BOX_MIJIK_TAG					1
#define ITEMS_CHOOSED_EFC					1
#define ITEMS_MIJI_BOOK						2
#define NUM_OF_MIJI							3
#define NUM_COUNT_MIJI						4

#define ZI_PNG								1
#define ZI_MUN_PNG							2
#define ZI_NUM								3

#define SC_FOR_MYBOX						1
#define ITEM_INFORMATION_LAY				2
#define SYSTEM_NOTICE						3

#define ZI_KUANG_PNG						1
#define ZI_COUNT_NUM						1


#define NUM_ITEM_ONE						7
#define NUM_ITEM_TWO						8
#define NUM_ITEM_THREE						9
#define NUM_ITEM_FOUR						10
public:
	MyXbox(void);
	~MyXbox(void);
	Size visibleSize;
	Vec2 origin;
	int curEffectNum;
	void convertCallback(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void itemsInBox_callBack(cocos2d::Ref* pSender,Widget::TouchEventType type);

	void composeCreate(Backpack_Item curItem);//∫œ≥…√Ê∞Â
	void inforPanelCreate(Backpack_Item curItem);
	void updateGoodCount();
	void NoticeCreate(std::string photoName);
	void NoticeEnsure(cocos2d::Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(MyXbox);
private:
	std::vector <Backpack_Item> myBackpack;	
};

