#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class LoginAndRegister : public Layer
{
public:
	LoginAndRegister(void);
	~LoginAndRegister(void);
	Size visibleSize;
	Vec2 origin;
	virtual bool init();
};