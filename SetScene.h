#pragma once
#include"cocos2d.h"
USING_NS_CC;
class SetScene :
	public Scene
{
public:
	SetScene(void);
	~SetScene(void);
	void onButtonBack(Ref* pSender);
	virtual bool init();
	CREATE_FUNC(SetScene);
};

