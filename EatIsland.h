#pragma once
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class EatIsland :
	public Layer
{
public:
	EatIsland(void);
	~EatIsland(void);
	void onButtonBuy(Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(EatIsland);
};

