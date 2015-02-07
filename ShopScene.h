#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class ShopScene :
	public Scene
{
public:
	ShopScene(void);
	~ShopScene(void);
	void onButtonBack(Ref* pSender,Widget::TouchEventType type);
	void onButtonSutraHall(Ref* pSender,Widget::TouchEventType type);
	void onButtonEatIsland(Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(ShopScene);
};

