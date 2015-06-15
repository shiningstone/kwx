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
#define DAYIN_RANKBUTTON			1
#define ACCOUNT_RANKBUTTON			2
#define MAXTIME_RANKBUTTON			3
#define JIFEN_RANKBUTTON			4
#define DAYIN_RANKING_SCROLL		5
#define ACCOUNT_RANKING_SCROLL		6
#define	MAXTIME_RANKING_SCROLL		7
#define	RECROLD_RANKING_SCROLL		8
	RankingScroll(void);
	~RankingScroll(void);
	virtual bool init();

	int curListNo;
	Size visibleSize;
	Vec2 origin;
	void ScrollListCreate();
	void dayInEvent(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void moneyEvent(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void recordEvent(cocos2d::Ref* pSender,Widget::TouchEventType type);
	void maxtimesEvent(cocos2d::Ref* pSender,Widget::TouchEventType type);
	CREATE_FUNC(RankingScroll);
};

#endif