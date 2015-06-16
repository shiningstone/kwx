#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class DaiPiaoLayer : public Layer
{
public:
#define BUTTON_BINGYISE				11
#define SHADE_BINGYISE				12
#define BUTTON_DASANYUAN			33
#define SHADE_DASANYUAN				34
#define BUTTON_MINGSIGUI			54
#define SHADE_MINGSIGUI				55
#define	BUTTON_GANGGANG				22
#define SHADE_GANGGANG				23
#define	BUTTON_XIAOSANYUAN			44
#define SHADE_XIAOSANYUAN			45
#define BUTTON_LONGQIDUI			65
#define SHADE_LONGQIDUI				66
#define ACTION_PNG_BYS				"bingyise.png"
#define ACTION_PNG_DSY				"dasanyuan.png"
#define ACTION_PNG_MSG				"mingsigui.png"
#define ACTION_PNG_GSG				"gangshanggang.png"
#define ACTION_PNG_XSY				"xiaosanyuan.png"
#define ACTION_PNG_LQD				"longqidui.png"
	DaiPiaoLayer(void);
	~DaiPiaoLayer(void);
	Size visibleSize;
	Vec2 origin;
	virtual bool init();
	TargetedAction* TouchShadeAction(Vec2 curPos,std::string actionName);
	void enterRoom(int roomPath);

	void onbuttonBingYiSe(Ref* pSender,Widget::TouchEventType type);
	void onbuttonDaSanYuan(Ref* pSender,Widget::TouchEventType type);
	void onbuttonMingSiGui(Ref* pSender,Widget::TouchEventType type);
	void onButtonGangGang(Ref* pSender,Widget::TouchEventType type);
	void onButtonXiaoSanYuan(Ref* pSender,Widget::TouchEventType type);
	void onButtonLongQiDui(Ref* pSender,Widget::TouchEventType type);

	CREATE_FUNC(DaiPiaoLayer);
};

