#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class MyBaseInformation :
	public Layer
{
public:
	MyBaseInformation(void);
	~MyBaseInformation(void);
#define MY_BASEINFO_ID				1
#define MY_ACCOUNT_LABEL			2
#define EDITBOX_FOR_NAME			3
#define LABEL_FOR_NAME				4
#define SEX_BOY_SIGN				5
#define SEX_GIRL_SIGN				6
#define WIN_RATE					7
#define MAX_HISTORY					8
	Size visibleSize;
	Vec2 origin;
	enum MySex
	{
		BOY,
		GIRL
	}my_sex;
	virtual bool init();
	void onButtonChangePhoto(Ref* pSender,Widget::TouchEventType type);
	void ChangeAccountCallBack(Ref* pSender,Widget::TouchEventType type);
	void UpgradeAccount(Ref* pSender,Widget::TouchEventType type);
	void ManagePassWord(Ref* pSender,Widget::TouchEventType type);
	void ChangeNameForPenCallBack(Ref* pSender,Widget::TouchEventType type);
	void CallBack_ForJiaHao(Ref* pSender,Widget::TouchEventType type);


	//void editBoxEditingDidEnd(EditBox* editBox);
	CREATE_FUNC(MyBaseInformation);
};

