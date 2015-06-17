#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class HelloWorld : public cocos2d::Layer
{
#define QQ_LOG_IN					1
#define LIJI_LOGIN					2
#define LOGIN_ACCOUNT				3
#define REGISTER_LAYER				4


#define CHOOSE_FLAG					1
public:
	HelloWorld();
	virtual ~HelloWorld();
	Size visibleSize;
	Vec2 origin;
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	float s_scale;
	int s_no;
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	void addButton();	//����?�����?��
	void set_userDefault();		//��?��?��y?Y
	virtual bool init();
	void qqLoadCallback(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void imLoadCallback(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);

	void LogInWithAccount();
	void RegisteredAccount();
	void LogInWithAccountBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void RememberChoose(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void RegistCall(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void LogInGame(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);

	void RegisterBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void GetVerificationCode(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);

	// a selector callback
	void enterroomCallback(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);	//�̣�?��Button
	void enterRoomStandAlone();

	void menuCloseCallback(cocos2d::Ref* pSender);	//��?��?��?

	void onEnterTransitionDidFinish();
	CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
