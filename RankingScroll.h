#ifndef RANKINGSCROLL_H
#define RANKINGSCROLL_H
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class RankingScroll:public cocos2d::Layer/*,public ScrollViewDelegate*/
{
public:
	virtual bool init();
	CREATE_FUNC(RankingScroll);
	void dayInEvent(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void moneyEvent(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void recordEvent(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void maxtimesEvent(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void btTouchEvent(cocos2d::Ref* pSender,Widget::TouchEventType type);
};

#endif