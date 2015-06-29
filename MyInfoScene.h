#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"	

#include "Game/GameType.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class MyInfoScene :	public Scene
{
public:
#define MY_MESSAGE_MOJI			1
#define SYSTEM_MES_MOJI			2
#define MY_MESSAGE_BUTT			3
#define SYSTEM_MES_BUTT			4
#define CURENT_UI_LAYER			5
	MyInfoScene();
	~MyInfoScene(void);

	Size visiableSize;
	Vec2 origin;

	void buttonMyInfo(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void buttonSystemNotice(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void buttonBack(cocos2d::Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(MyInfoScene);
};

