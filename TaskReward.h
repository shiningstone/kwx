#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class TaskReward :	public Scene
{
public:
	TaskReward(void);
	~TaskReward(void);
	void buttonBackToManage(Ref* pSender);
	virtual bool init();
	CREATE_FUNC(TaskReward);
};

