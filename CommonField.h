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
	CommonField(void);
	~CommonField(void);
	void onButtonMarket(Ref* pSender,Widget::TouchEventType type);
	void onButtonBack(Ref* pSender,Widget::TouchEventType type);
	void onButtonDaiPiao(Ref* pSender,Widget::TouchEventType type);
	void onButtonBuPiao(Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(CommonField);
};

