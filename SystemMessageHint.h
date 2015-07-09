#ifndef SYSTEM_MESSAGE_HINT_H
#define SYSTEM_MESSAGE_HINT_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class SystemMessageHint : public Layer
{
public:
	SystemMessageHint(Node* p,std::string str,int tag);
	~SystemMessageHint(void);

	Size visibleSize;
	Vec2 origin;
	virtual bool init();
private:
	Node* parentNode;
	int CurTag;
	std::string NoticeMessage;
};

#endif