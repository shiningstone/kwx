#ifndef DAYBYDAY_H
#define DAYBYDAY_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"	
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
#include "EnterRoomScene.h"

class dayAward:public cocos2d::Layer
{
public:
	dayAward(Node* p);
	virtual bool init();
	EnterRoom* parent;
	//CREATE_FUNC(dayAward);
	void backToMain(float t);
	void callBack(cocos2d::Ref* pSender,Widget::TouchEventType type);
	int receiveTime;
};
#endif