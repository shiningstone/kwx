#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class DaiPiaoLayer :
	public Layer
{
public:
	DaiPiaoLayer(void);
	~DaiPiaoLayer(void);
	void onButtonKaWuXing(Ref* pSender,Widget::TouchEventType type);
	void onButtonTiaoYiSe(Ref* pSender,Widget::TouchEventType type);
	void onButtonGangShangHua(Ref* pSender,Widget::TouchEventType type);
	void onButtonAnSiGui(Ref* pSender,Widget::TouchEventType type);
	void onButtonHaiDilao(Ref* pSender,Widget::TouchEventType type);
	void onButtonPengPengHu(Ref* pSender,Widget::TouchEventType type);
	void actionKaWuXing();
	void entranceToKaWuXing(float t);
	void actionAnSiGui();
	void entranceToAnSiGui(float t);
	void actionTiaoYiSe();
	void entranceToTiaoYiSe(float t);
	void actionHaiDiLao();
	void entranceHaiDiLao(float t);
	void actionGangShangHua();
	void entranceGangShangHua(float t);
	void actionPengPengHu();
	void entrancePengPengHu(float t);
	virtual bool init();
	CREATE_FUNC(DaiPiaoLayer);
};

