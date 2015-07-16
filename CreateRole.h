#ifndef _CREATE_ROLE_H_
#define _CREATE_ROLE_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

#include "EnterRoomScene.h"
#include "network/KwxEnv.h"

class CreateRole : public Layer
{
public:
#define CREATE_ROLE_BKG				1

#define DEFAULT_PHOTO				1//1-8
#define PHOTO_CHOOSED_BKG			9//9-16
#define MY_CHOOSED_PHOTO			17
	CreateRole(EnterRoom* p);
	~CreateRole(void);

	Size visibleSize;
	Vec2 origin;
	virtual bool init();
	void DefaultPhotoChooseCall(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);
private:
	EnterRoom* parentNode;
	int lastChooseNum;
};
#endif