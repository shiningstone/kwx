#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class EnterForCompetitionLayer :
	public Layer
{
public:
	EnterForCompetitionLayer(void);
	~EnterForCompetitionLayer(void);
	void onButtonQuit(Ref* pSender,Widget::TouchEventType type);
	void onButtonSignUp(Ref* pSender,Widget::TouchEventType type);
	virtual bool init();
	CREATE_FUNC(EnterForCompetitionLayer);
};

