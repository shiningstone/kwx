#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class BaseScene :
	public Scene
{
public:
	BaseScene(void);
	~BaseScene(void);
	virtual bool init();
	void buttonMyInfo(Ref* pSender,Widget::TouchEventType type);
	void buttonMyXbox(Ref* pSender,Widget::TouchEventType type);
	void buttonBack(Ref* pSender,Widget::TouchEventType type);
	CREATE_FUNC(BaseScene);
};

