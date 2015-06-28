
#include <IOSTREAM>     

#include "KwxHttpManager.h"
//#include "../UnZip/DJZipArchive.h" 

#define HTTPHeader "http://www.epicc.com.cn/QrDmzBastionNB/BastionServlet"  

HTTPManager::HTTPManager() { 
} 

HTTPManager::~HTTPManager() { 
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
    log("postData = %s",postData);     
    request->setRequestData(postData, strlen(postData));     
    request->setTag("getFormData");     
    log("url = %s",request->getUrl());
    
    HttpClient::getInstance()->send(request);     
    request->release(); 
}   

void HTTPManager::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
    if (!response) {
        return;     
    }
    
    if (0 != strlen(response->getHttpRequest()->getTag())) {
        log("%s compeled",response->getHttpRequest()->getTag());     
    }           

    long statusCode = response->getResponseCode();           
    char statusString[64] = {};           
    sprintf(statusString, "HTTP Status Code:%ld , tag = %s",statusCode,response->getHttpRequest()->getTag());     
    log("response code:%s",statusString);                 

    if (!response->isSucceed()) {         
        log("response failed");         
        log("error buffer:%s",response->getErrorBuffer());        
        return;     
    }             

    if (_httpManagerDelegate) {
        _httpManagerDelegate->onHttpManagerRequestCompleted(sender, response);     
    }                          

    const char* resTag = response->getHttpRequest()->getTag(); 
}   

void HTTPManager::writeFileFromRequest(HttpResponse *response,std::string filename) {
    std::vector<char>* buffer = response->getResponseData();     
    printf("Http Test,dump data:"); 
    
    std::string path     = FileUtils::getInstance()->getWritablePath();     
    std::string fullPath =  path + filename;     

    FILE* fp = fopen(fullPath.c_str(), "wb");             
    log("将文件写入本地 %s",fullPath.c_str());
    
    unsigned char bf;
    for (unsigned int i  = 0; i < buffer->size(); i++) {
        bf = buffer->at(i);         
        fwrite(&bf, 1, 1, fp);               
    }     
    fclose(fp);       
}   

void HTTPManager::addHttpListener(ccHttpManagerCallback &callback) {
    _eventCallback = callback;       
} 


