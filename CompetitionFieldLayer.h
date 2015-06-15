#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class CompetitionFieldLayer : public Layer
{
public:
#define BUTTON_NEWBIEFIELD					1
#define SHADE_NEWBIEFIELD					7
#define BUTTON_MASTERFIELD					2
#define	SHADE_MASTERFIELD					8
#define BUTTON_ELITEFIELD					3
#define	SHADE_ELITEFIELD					9
#define BUTTON_6PERSONFIELD					4
#define	SHADE_6PERSONFIELD					10
#define BUTTON_24PERSONFIELD				5
#define	SHADE_24PERSONFIELD					11
#define BUTTON_99PERSONFIELD				6
#define	SHADE_99PERSONFIELD					12
#define ACTION_PNG_CLS					"cainiaochang.png"
#define ACTION_PNG_DSS					"dashichang.png"
#define ACTION_PNG_JYS					"jingyingchang.png"
#define ACTION_PNG_6RS					"6rensai.png"
#define ACTION_PNG_24R					"24rensai.png"
#define ACTION_PNG_99R					"99rensai.png"
	CompetitionFieldLayer(void);
	~CompetitionFieldLayer(void);
	Size visibleSize;
	Vec2 origin;
	virtual bool init();
	TargetedAction* TouchShadeAction(Vec2 curPos,std::string actionName);
	void entranceToEachRoom();

	void onButtonNewbieField(Ref* pSender,Widget::TouchEventType type);
	void onButtonEliteField(Ref* pSender,Widget::TouchEventType type);
	void onButtonMasterField(Ref* pSender,Widget::TouchEventType type);
	void onButton99PersonField(Ref* pSender,Widget::TouchEventType type);
	void onButton24PersonField(Ref* pSender,Widget::TouchEventType type);
	void onButton6PersonField(Ref* pSender,Widget::TouchEventType type);

	CREATE_FUNC(CompetitionFieldLayer);
};

