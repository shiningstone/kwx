
#ifndef __VERSION_MANAGER__
#define __VERSION_MANAGER__

#include <string>
using namespace std;

#include "./../utils/UtilBasic.h"
#include "./../utils/LogManager.h"

#include "./../network/KwxHttpManager.h"

#define VERSION_PAGE "localhost:8080/qwe.json"
#define VERSION_CODE "update_time"

class VersionManager : public HTTPManagerDelegate {
public:
    static VersionManager *getInstance();
    static void  destroyInstance();

    virtual void onHttpManagerRequestCompleted(HttpClient *sender, HttpResponse *response); 

    string          getCurrentVerName() const;
    VersionCode_t   getNewVerCode() const;
    string          getNewVerName() const;
    int             upgrade();
private:
    friend void test_download_version();
    friend void test_update_version();
    
    VersionManager();
    ~VersionManager();

    int _requestUpdate();
    int _download();
    int _update();
    int _recordVersion();

    friend class VersionUpdateResponse;
    VerInfo_t       _newVer;
    HTTPManager     *_hm;

    Logger *_logger;
    static VersionManager *_instance;
};

#endif

