#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class ShopLayer :
	public Layer
{
public:
	ShopLayer(void);
	~ShopLayer(void);
	void onButtonBuy(Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(ShopLayer);
};

