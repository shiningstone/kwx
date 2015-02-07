#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "cocos2d.h"
USING_NS_CC;

class account:public cocos2d::Layer
{
public:
	account();
	~account();
	virtual bool init();
	CREATE_FUNC(account);
};
#endif

