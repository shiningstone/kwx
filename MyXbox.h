#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

#include "LoginType.h"

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
#define	DETAILED_OF_MIJI					2
#define CURMJ_ITEM_ONE						3
#define CURMJ_ITEM_TWO						4
#define CURMJ_ITEM_THREE					5
#define CURMJ_ITEM_FOUR						6

#define NUM_ITEM_ONE						7
#define NUM_ITEM_TWO						8
#define NUM_ITEM_THREE						9
#define NUM_ITEM_FOUR						10
public:
	MyXbox(Backpack_Item MyGoods[]=NULL);
	~MyXbox(void);
	Size visibleSize;
	Vec2 origin;
	int curEffectNum;
	void convertCallback(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void itemsInBox_callBack(cocos2d::Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	//CREATE_FUNC(MyXbox);
};

