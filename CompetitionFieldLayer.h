#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class CompetitionFieldLayer :
	public Layer
{
public:
	CompetitionFieldLayer(void);
	~CompetitionFieldLayer(void);

	void onButtonNewbieField(Ref* pSender,Widget::TouchEventType type);
	void onButtonEliteField(Ref* pSender,Widget::TouchEventType type);
	void onButtonMasterField(Ref* pSender,Widget::TouchEventType type);
	void onButton99PersonField(Ref* pSender,Widget::TouchEventType type);
	void onButton24PersonField(Ref* pSender,Widget::TouchEventType type);
	void onButton6PersonField(Ref* pSender,Widget::TouchEventType type);
	void actionNewbieField();
	void entranceToNewbieField(float t);
	void actionEliteField();
	void entranceToEliteField(float t);
	void actionMasterField();
	void entranceToMasterField(float t);
	void action99PersonField();
	void entrance99PersonField(float t);
	void action24PersonField();
	void entrance24PersonField(float t);
	void action6PersonField();
	void entrance6PersonField(float t);
	virtual bool init();
	CREATE_FUNC(CompetitionFieldLayer);
};

