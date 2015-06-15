#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class CompetitionField : public Scene
{
public:
#define BAO_MING_COMPETE_LAYER			1
	CompetitionField(void);
	~CompetitionField(void);

	void onButtonMarket(Ref* pSender,Widget::TouchEventType type);
	void onButtonBack(Ref* pSender,Widget::TouchEventType type);
	void onButtonHelp(Ref* pSender,Widget::TouchEventType type);

	virtual bool init();
	CREATE_FUNC(CompetitionField);
};

