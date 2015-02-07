#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class MyXbox :
	public Layer
{
public:
	MyXbox(void);
	~MyXbox(void);
	void menuCloseCallback(cocos2d::Ref* pSender);
	virtual bool init();
	CREATE_FUNC(MyXbox);
};

