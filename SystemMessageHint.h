#ifndef SYSTEM_MESSAGE_HINT_H
#define SYSTEM_MESSAGE_HINT_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

typedef enum message_hint
{
	mes_Hint_Ensure_Only,//只有确认，返回登陆页面
	mes_Hint_Choose//确认和取消，确认ChooseEnsure待定，取消返回登陆页面
}MES_HINT_ACTION;


class SystemMessageHint : public Layer
{
public:
#define SYSTEM_MES_HINT_BKG						1
	SystemMessageHint(std::string str,MES_HINT_ACTION curAct);
	~SystemMessageHint(void);

	Size visibleSize;
	Vec2 origin;
	virtual bool init();

	void HintOnlyEnsure();
	void OnlyEnsureCall(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);

	void HintWihtChoose();
	void ChooseEnsure(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);
	void ChooseCancel(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);


private:
	std::string NoticeMessage;
	MES_HINT_ACTION curMesAction;
};

#endif