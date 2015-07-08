#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class SetLayer :public cocos2d::Layer
{
public:
	SetLayer(void);
	~SetLayer(void);
	bool voiceOnFlag;
	bool voiceEffectOnFlag;
	bool vibrationOnFlag;
	void onButtonVoice(Ref *pSender,Widget::TouchEventType type);
	void onButtonVoiceEffect(Ref* pSender,Widget::TouchEventType type);
	void onButtonVibration(Ref* pSender,Widget::TouchEventType type);
	void onButtonOnceClick(Ref* pSender,Widget::TouchEventType type);
	void onButtonDoubleClick(Ref* pSender,Widget::TouchEventType type);
	void onButtonInterface(Ref* pSender);
	virtual bool init();
	CREATE_FUNC(SetLayer);
};

