
/*********************************************************************
    this is just a reference to show how to use HTTPManagerDelegate
*********************************************************************/

#ifdef REFERENCE_ONLY

#include "cocos2d.h"
#include "HTTPManager.h"

USING_NS_CC;   

class HelloWorld : public cocos2d::Layer,public HTTPManagerDelegate 
{ 
public:     
    // there's no 'id' in cpp, so we recommend returning the class instance pointer     
    static cocos2d::Scene* createScene();       
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone     
    virtual bool init();             
    // a selector callback     
    void menuCloseCallback(cocos2d::Ref* pSender);           
    virtual void onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response) ;           
    void writeFileFromRequest(cocos2d::network::HttpResponse *response,std::string filename);             
    // implement the "static create()" method manually     
    CREATE_FUNC(HelloWorld); 
}; 

Scene* HelloWorld::createScene() 
{     
    auto scene = Scene::create();           
    auto layer = HelloWorld::create();       
    scene->addChild(layer);       
    return scene; 
}   

// on "init" you need to initialize your instance 
bool HelloWorld::init() 
{     
    //////////////////////////////     
    // 1. super init first     
    if ( !Layer::init() ) {         
        return false;     
    }           

    Size visibleSize = Director::getInstance()->getVisibleSize();     
    Vec2 origin = Director::getInstance()->getVisibleOrigin();       

    /////////////////////////////     
    // 2. add a menu item with "X" image, which is clicked to quit the program     
    //    you may modify it.       
    // add a "close" icon to exit the progress. it's an autorelease object     
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",                                            
        "CloseSelected.png",                                            
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));           

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
        origin.y + closeItem->getContentSize().height/2));       

    // create menu, it's an autorelease object     
    auto menu = Menu::create(closeItem, NULL);     
    menu->setPosition(Vec2::ZERO);     
    this->addChild(menu, 1);       

    auto httpManager = new HTTPManager();     
    httpManager->retain();
    httpManager->setHttpDelegate(this);     
    httpManager->sendGetRequest("localhost:8080/qwe.json", "update_time");     

    return true; 
}   

void HelloWorld::onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response) {
    if (strcmp(response->getHttpRequest()->getTag(), "update_time") == 0) {             
        writeFileFromRequest(response,"qwe.json");             
        log("下载更新包");             

        std::string version = "version";             
        std::string table = "version";             

        if (!(strcmp(version.c_str(), table.c_str()) == 0)) {
            log("下载更新包"); 
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)                   
            #endif             
        }                                     
    }                             

    if (strcmp(response->getHttpRequest()->getTag(), "update_mobile") == 0) {                           
        log("update_mobile");
    }           
}   

void HelloWorld::writeFileFromRequest(cocos2d::network::HttpResponse *response,std::string filename) {
    std::vector<CHAR>* buffer = response->getResponseData();
    printf("Http Test,dump data:");

    std::string path     = FileUtils::getInstance()->getWritablePath();
    std::string fullPath =  path + filename;
    FILE* fp = fopen(fullPath.c_str(), "wb");
    log("将文件写入本地 %s",fullPath.c_str());
    //    auto size = buffer->size();
    //    unsigned char bf[size];
    //    fwrite(bf, size, 1, fp);

    unsigned char bf;     
    for (unsigned int i  = 0; i < buffer->size(); i++) {
        bf = buffer->at(i);
        fwrite(&bf, 1, 1, fp);
    }

    fclose(fp);
}   

void HelloWorld::menuCloseCallback(Ref* pSender) {
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    #else
        Director::getInstance()->end();
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
        #endif 
    #endif
} 

#endif/*REFERENCE_ONLY*/
