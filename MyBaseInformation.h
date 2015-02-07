#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class MyBaseInformation :
	public Layer
{
public:
	MyBaseInformation(void);
	~MyBaseInformation(void);
	enum MyEnum
	{
		boy,
		girl
	}sex;
	virtual bool init();
	void onButtonBoy(Ref* pSender,Widget::TouchEventType type);
	void onButtonGirl(Ref* pSender,Widget::TouchEventType type);
	void menuCloseCallback(cocos2d::Ref* pSender);
	void onButtonChangePhoto(Ref* pSender,Widget::TouchEventType type);
	CREATE_FUNC(MyBaseInformation);
};

