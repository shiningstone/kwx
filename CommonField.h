#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class CommonField :
	public Scene
{
public:
#define NO_PIAO_CP_MOJI				1
#define WITH_PIAO_CP_MJ				2
#define NO_PIAO_BUTTON_				3
#define WITH_PIAO_BUTTO				4
#define CURREN_UI_LAYER				5
	CommonField(void);
	~CommonField(void);
	Size visibleSize;
	Vec2 origin;
	void onButtonMarket(Ref* pSender,Widget::TouchEventType type);
	void onButtonBack(Ref* pSender,Widget::TouchEventType type);
	void onButtonDaiPiao(Ref* pSender,Widget::TouchEventType type);
	void onButtonBuPiao(Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(CommonField);
};

