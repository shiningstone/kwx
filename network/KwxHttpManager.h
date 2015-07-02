
#ifndef __KWX_HTTP_MANAGER__
#define __KWX_HTTP_MANAGER__

#include "cocos2d.h"
#include "../cocos2d/extensions/cocos-ext.h"
#include "network/HttpClient.h"

#include "./../utils/LogManager.h"

USING_NS_CC; 
USING_NS_CC_EXT; 
using namespace cocos2d::network; 

class  HTTPManagerDelegate { 
public:
    virtual void onHttpManagerRequestCompleted(HttpClient *sender, HttpResponse *response) = 0; 
};   

class HTTPManager:public Ref { 
public:     
    typedef std::function<void(HttpClient *sender,HttpResponse *response)> ccHttpManagerCallback;     

    HTTPManager();     
    ~HTTPManager();     
    CC_SYNTHESIZE(HTTPManagerDelegate*, _httpManagerDelegate, HttpDelegate);     

    //Http Response Callback     
    void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);     

    void getFormData(std::string parameter);     
    void sendGetRequest(std::string url,std::string requestTag);     
    void addHttpListener(ccHttpManagerCallback& callback);   
private:
    int _CheckResponse(HttpResponse *response);

    ccHttpManagerCallback _eventCallback; 

    Logger *_logger;
}; 

#endif

