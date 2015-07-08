#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class ShopScene : public Scene
{
public:
#define CANGJINGGE_MOJI			1
#define TONGCHIDAO_MOJI			2
#define	CANGJINGGE_BUTT			3
#define TONGCHIDAO_BUTT			4
#define CURENT_UI_LAYER			5
	ShopScene(void);
	~ShopScene(void);
	void onButtonBack(Ref* pSender,Widget::TouchEventType type);
	//void onButtonSutraHall(Ref* pSender,Widget::TouchEventType type);
	//void onButtonEatIsland(Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(ShopScene);
};

