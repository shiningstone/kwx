#ifndef _RECONNECT_REMIND_
#define _RECONNECT_REMIND_

#include "cocos2d.h"

class ReconnectRemind : public cocos2d::Layer
{
public:
	ReconnectRemind();
	virtual ~ReconnectRemind();

	float s_scale;
	int s_no;

	virtual bool init();
	CREATE_FUNC(ReconnectRemind);
};
#endif


