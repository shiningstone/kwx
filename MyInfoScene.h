#pragma once
#include"cocos2d.h"
USING_NS_CC;
class MyInfoScene :
	public Scene
{
public:
	MyInfoScene(void);
	~MyInfoScene(void);
	void buttonMyInfo(Ref* pSender);
	void buttonSystemNotice(Ref* pSender);
	void buttonBack(Ref* pSender);
	virtual bool init();
	CREATE_FUNC(MyInfoScene);
};

