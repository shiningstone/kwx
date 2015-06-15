#include "LoginAndRegister.h"

LoginAndRegister::LoginAndRegister(void)
{
}
LoginAndRegister::~LoginAndRegister(void)
{
}
bool LoginAndRegister::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("register-login.plist");

	return true;
}