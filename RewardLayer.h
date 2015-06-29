#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class RewardLayer :	public Layer
{
public:
	RewardLayer(void);
	~RewardLayer(void);
	void onButtonGet(Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(RewardLayer);
};
