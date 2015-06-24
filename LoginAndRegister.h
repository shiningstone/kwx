#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class LoginAndRegister : public Layer
{
public:
#define LOGIN_ACCOUNT				1
#define REGISTER_LAYER				2
		
#define LOGIN_REGISTER				3

#define CHOOSE_FLAG					1
#define COUNT_NUM					1
#define MESSAGE_COUNT				1
	LoginAndRegister(Node* p);
	~LoginAndRegister(void);
	Size visibleSize;
	Vec2 origin;
	Node* parentNode;
	int timing;
	virtual bool init();

	void RegisteredAccount();
	void LogInWithAccountBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void RememberChoose(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void RegistCall(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void LogInGame(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);

	void RegisterBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void GetVerificationCode(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);

	void updateNum(float delta);
};