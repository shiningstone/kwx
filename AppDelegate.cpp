#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "VersionUpgrade.h"
#include "SystemMessageHint.h"
#include "hall/ErrorManager.h"

#include "IMLoadScene.h"

USING_NS_CC;

#include <string>
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
	glview->setDesignResolutionSize(1218 ,716 ,kResolutionExactFit);
	//glview->setDesignResolutionSize(1218,716,kResolutionShowAll);//kResolutionFixedHeight
    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    //director->setAnimationInterval(1.0 / 60);

	//============================2a¨º?==========================//
	//auto scene = Scene::create();
	//SpriteFrameCache::getInstance()->removeSpriteFrames();
	//TextureCache::sharedTextureCache()->removeAllTextures();
	//auto layer = RaceLayer::create();
	//scene->addChild(layer);
	//============================2a¨º?==========================//

    ErrorInfoHandle *aHandle = new ErrorInfoHandle;
    ErrorManager::getInstance()->set_handle(aHandle);

     //create a scene. it's an autorelease object
	auto scene = Scene::create();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("systemprompt.plist");
	Layer* startLayer = HelloWorld::create();
	scene->addChild(startLayer,1);
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void ErrorInfoHandle::HandleMsg(void *aMsg) {
    Layer* prompt = NULL;
    
    std::string *err = static_cast<std::string *>(aMsg);
    
    if(!strcmp(err->c_str(),"RECONNECT_REQUIRED")) {
        prompt = new SystemMessageHint(*err,mes_Hint_Choose);
    } else {
        prompt = new SystemMessageHint(*err,mes_Hint_Ensure_Only);
    }
    
    delete err;
    
    prompt->setVisible(true);
    Director::getInstance()->getRunningScene()->addChild(prompt,99);
}

