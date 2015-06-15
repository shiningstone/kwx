#include "AppDelegate.h"
#include "HelloWorldScene.h"
//#include "GameScene.h"
#include "VersionUpgrade.h"

USING_NS_CC;

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
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	//============================2a¨º?==========================//
	//auto scene = Scene::create();
	//SpriteFrameCache::getInstance()->removeSpriteFrames();
	//TextureCache::sharedTextureCache()->removeAllTextures();
	//auto layer = RaceLayer::create();
	//scene->addChild(layer);
	//============================2a¨º?==========================//

     //create a scene. it's an autorelease object
	auto scene = Scene::create();
	
	std::string curEdition="1.1.1.0";
	std::string newestEdition="1.1.1.1";
	Layer* startLayer;
	if(newestEdition>curEdition)
		startLayer=new VersionUpgrade(curEdition,newestEdition);
	else
		startLayer=HelloWorld::create();
	// run
	scene->addChild(startLayer,1);
    // run
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
