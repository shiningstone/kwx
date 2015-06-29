
#ifndef __VERSION_MANAGER__
#define __VERSION_MANAGER__

#include <string>
using namespace std;

#include "./../utils/UtilBasic.h"
#include "./../utils/LogManager.h"

#include "./../network/KwxHttpManager.h"

typedef INT32U VersionCode_t;

typedef struct _VerInfo_t {
    VersionCode_t   code;
    string          name;
    string          content;
    string          size;
    string          url;
}VerInfo_t;

class VersionManager : public HTTPManagerDelegate {
public:
    static VersionManager *getInstance();
    static void  destroyInstance();

    virtual void onHttpManagerRequestCompleted(HttpClient *sender, HttpResponse *response); 

    VersionCode_t   getCurrentVerCode() const;
    string          getCurrentVerName() const;
    int             update();
private:
    VersionManager();

    int _requestUpdate();
    int _download();
    int _upgrade();

    friend class VersionUpdateResponse;
    VerInfo_t       _newVer;

    Logger *_logger;
    static VersionManager *_instance;
};

#endif

