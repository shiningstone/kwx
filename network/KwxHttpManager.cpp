
#include <IOSTREAM>     

#include "./../utils/UtilBasic.h"
#include "KwxHttpManager.h"
//#include "../UnZip/DJZipArchive.h" 

#define HTTPHeader "http://www.epicc.com.cn/QrDmzBastionNB/BastionServlet"  

HTTPManager::HTTPManager() { 
    _logger = LOGGER_REGISTER("HTTPManager");
} 

HTTPManager::~HTTPManager() { 
    LOGGER_DEREGISTER(_logger);
} 

void HTTPManager::sendGetRequest(std::string url,std::string requestTag) {     
    HttpRequest* request = new HttpRequest();

    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(url.c_str());
    request->setResponseCallback(CC_CALLBACK_2(HTTPManager::onHttpRequestCompleted, this));
    request->setTag(requestTag.c_str());
    HttpClient::getInstance()->send(request);
    request->release();
}   

void HTTPManager::getFormData(std::string parameter) {
    HttpRequest* request = new HttpRequest();   
    
    request->setUrl(HTTPHeader);     
    request->setRequestType(HttpRequest::Type::POST);           
    request->setResponseCallback(CC_CALLBACK_2(HTTPManager::onHttpRequestCompleted, this));                 //write the post data     

    const char* postData = StringUtils::format("parameter=%s",parameter.c_str()).c_str();     
    LOGGER_WRITE("postData=%s.",postData);     
    request->setRequestData(postData, strlen(postData));     
    request->setTag("getFormData");     
    LOGGER_WRITE("url=%s.",request->getUrl());
    
    HttpClient::getInstance()->send(request);     
    request->release(); 
}   

int HTTPManager::_CheckResponse(HttpResponse *response) {
    LOGGER_WRITE("HTTP Status Code: %ld,tag=%s.",response->getResponseCode(),response->getHttpRequest()->getTag());                 

    if (!response->isSucceed()) {         
        LOGGER_WRITE("Error buffer:%s",response->getErrorBuffer());
        return RET_ERROR;
    } else {
        return RET_OK;
    }
}

void HTTPManager::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
    if (!response) {
        LOGGER_WRITE("response is null");
        return;
    } else if(_CheckResponse(response)==RET_ERROR) {
        return;
    } else {
        if (_httpManagerDelegate) {
            _httpManagerDelegate->onHttpManagerRequestCompleted(sender, response);     
        }                    
        
        const char* resTag = response->getHttpRequest()->getTag(); 
    }
}   

void HTTPManager::addHttpListener(ccHttpManagerCallback &callback) {
    _eventCallback = callback;       
} 

