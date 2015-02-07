#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class Wait :
	public Layer
{
public:
	Wait(void);
	~Wait(void);
	static Scene* createScene();
	void onButtonMarket(Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(Wait);
};

