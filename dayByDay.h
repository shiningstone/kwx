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
#define JUANZHOU_BKG			100//100
#define GOLD_PNG_UI				1
#define GOLD_HAVE_GETTED		9
#define GOLD_ENABLE_DUIGOU		17
	Size visibleSize;
	Vec2 origin;
	dayAward(Node* p);
	EnterRoom* parent;
	int receiveTime;
	
	virtual bool init();
	TargetedAction* GoldGetEffect(Vec2 curPos);
	void LingQucallBack(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void receiveTimeCount();
};
#endif