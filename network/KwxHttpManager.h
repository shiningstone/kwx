
#ifndef __KWX_HTTP_MANAGER__
#define __KWX_HTTP_MANAGER__

#include "cocos2d.h"
#include "../cocos2d/extensions/cocos-ext.h"
#include "network/HttpClient.h"

USING_NS_CC; 
USING_NS_CC_EXT; 
using namespace network; 

class  HTTPManagerDelegate { 
public:    
    virtual void onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response) = 0; 
};   

class HTTPManager:public Ref { 
public:     
    typedef std::function<VOID(cocos2d::network::HttpClient *sender,cocos2d::network::HttpResponse *response)> ccHttpManagerCallback;     

    HTTPManager();     
    ~HTTPManager();     
    CC_SYNTHESIZE(HTTPManagerDelegate*, _httpManagerDelegate, HttpDelegate);     

    //Http Response Callback     
    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);     

    void getFormData(std::string parameter);     
    void sendGetRequest(std::string url,std::string requestTag);     
    void writeFileFromRequest(cocos2d::network::HttpResponse *response,std::string filename);     
    void addHttpListener(ccHttpManagerCallback& callback);   
private:     
    ccHttpManagerCallback _eventCallback; 
}; 

#endif

