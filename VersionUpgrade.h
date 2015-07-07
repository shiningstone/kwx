#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class VersionManager;
class VersionUpgrade : public Layer
{
public:
#define SMALL_MESSAGE					1
#define BIG_MESSAGE						2
#define UP_CANCEL_BUT					3
#define UP_ENSURE_BUT					4
#define DOWN_BAR						5

#define HAVE_DOWN_SIZE					6
#define HAVE_DOWN_UNIT					7
#define DOWN_SPEED_KB					8
#define DOWN_PERSETTAGE					9

	const bool      _forceUpgrade;
    VersionManager *_vm;
    
	Size visibleSize;
	Vec2 origin;
    
	int downSpeed;
	float targetFileSize;
	float haveDownSize;

	VersionUpgrade(bool forceUpgrade);
	~VersionUpgrade(void);

	virtual bool init();
	void upCancelCallBack(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);
	void upEnsureCallBack(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);
	void downCancelCallBack(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);
	void downDataUpdate(float delta);
private:
    std::string curVersion() const;
    std::string newVersion() const;
};