#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

#include "LoginType.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class BaseScene : public Scene
{
public:
#define MY_INFO_MOJI		1
#define MY_INFO_BUTON		2
#define MY_BAG_MOJI			3
#define MY_BAG_BUTTON		4
#define CUR_HAVED_LAYER		5
	BaseScene(Personal_Information curPlayer);
	~BaseScene(void);
	Size visibleSize;
	Vec2 origin;
	virtual bool init();
	void buttonMyInfo(Ref* pSender,Widget::TouchEventType type);		//ReWrite
	void buttonMyXbox(Ref* pSender,Widget::TouchEventType type);		//ReWrite
	void buttonBack(Ref* pSender,Widget::TouchEventType type);			//ReWrite
	//CREATE_FUNC(BaseScene);
private:
	Personal_Information PlayerInformation;
};

