#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class ChatOneToOne : public Layer
{
public:
#define FRIEND_CHAT_BKG							1
#define CHAT_BIG_KUANG_BKG						1
#define CHAT_WHITE_BKG							2
	ChatOneToOne(std::string Object);
	~ChatOneToOne(void);
	Size visibleSize;
	Vec2 origin;
	virtual bool init();
	std::string ChatObject;
};