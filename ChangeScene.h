#pragma once
#include"cocos2d.h"
USING_NS_CC;
class ChangeScene :
	public Scene
{
public:
	ChangeScene(void);
	~ChangeScene(void);
	void buttonToBaseInfo(Ref* pSender);
	void buttonToMyInfo(Ref* pSender);
	void buttonToActivity(Ref* pSender);
	void buttonToTaskReward(Ref* pSender);
	void buttonToSet(Ref* pSender);
	void buttonToShop(Ref* pSender);
	void buttonToCommonField(Ref* pSender);
	void buttonToCompetition(Ref* pSender);
	void buttonToWait(Ref* pSender);
	virtual bool init();
	CREATE_FUNC(ChangeScene);
};

