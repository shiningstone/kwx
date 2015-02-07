#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class ActivityCenter :
	public Scene
{
public:
	ActivityCenter(void);
	~ActivityCenter(void);
	void onButtonBack(Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(ActivityCenter);
};

