#ifndef RANKINGSCROLL_H
#define RANKINGSCROLL_H
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

#include <vector>
#include <string>
#include "network/KwxEnv.h"

class RankingScroll:public cocos2d::Layer
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
	void ScrollListCreate(std::vector <TheCharts> curList,int curTag);
	void litBtnCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type);
	CREATE_FUNC(RankingScroll);
private:
	std::vector <TheCharts> dayInList;
	std::vector <TheCharts> moneyList;
	std::vector <TheCharts> recordList;
	std::vector <TheCharts> maxtimeList;
};

#endif