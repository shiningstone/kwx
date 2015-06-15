#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class BuDaiPiaoLayer :	public Layer
{
public:
#define KAWUXING_BUTTON				11
#define KAWUXING_SHADE				12
#define TIAOYISE_BUTTON				33
#define TIAOYISE_SHADE				34
#define GANGSHANGHUA_BT				54
#define GANGHUA_SHADE				55
#define	ANSIGUI_BUTTON				22
#define ANSIGUI_SHADE				23
#define	HAIDILAO_BUTTON				44
#define HAIDILAO_SHADE				45
#define PENGPENG_BUTTON				65
#define PENGPENG_SHADE				66
#define ACTION_PNG_KWX				"kawuxingBtn.png"
#define ACTION_PNG_TYS				"tiaoyise.png"
#define ACTION_PNG_GSH				"gangshanghua.png"
#define ACTION_PNG_ASG				"ansigui.png"
#define ACTION_PNG_HDL				"haidilao.png"
#define ACTION_PNG_PPH				"pengpenghu.png"
	BuDaiPiaoLayer(void);
	~BuDaiPiaoLayer(void);
	virtual bool init();
	Size visibleSize;
	Vec2 origin;
	TargetedAction* TouchShadeAction(Vec2 curPos,std::string actionName);
	void enterRoom(int id);

	void onButtonKaWuXing(Ref* pSender,Widget::TouchEventType type);
	void entranceToKaWuXing();

	void onButtonTiaoYiSe(Ref* pSender,Widget::TouchEventType type);
	void entranceToTiaoYiSe();

	void onButtonGangShangHua(Ref* pSender,Widget::TouchEventType type);
	void entranceGangShangHua();

	void onButtonAnSiGui(Ref* pSender,Widget::TouchEventType type);
	void entranceToAnSiGui();

	void onButtonHaiDilao(Ref* pSender,Widget::TouchEventType type);
	void entranceHaiDiLao();

	void onButtonPengPengHu(Ref* pSender,Widget::TouchEventType type);
	void entrancePengPengHu();

	CREATE_FUNC(BuDaiPiaoLayer);
};

