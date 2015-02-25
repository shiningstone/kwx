
#ifndef __DATABASE__
#define __DATABASE__

#include "cocos2d.h"
USING_NS_CC;

typedef struct {
    int  id;
    char name[32];
    int  property;
    char level[32];
    char photo[32];
    char sex[32];
    char language[32];
}UserProfile_t;

class Database {
public:
    static Database *getInstance();
    static void destroyInstance();

    void GetUserProfile(int id,UserProfile_t &profile);
    void SetProperty(int id,int property);
protected:
    Database();
    ~Database();

    static Database *_instance;
private:
    static char  *ROBOTS_NAME[16];
    UserDefault *_userDefault;

    void _getRobotProfile(int id,UserProfile_t &profile);
    void _getMyProfile(int id,UserProfile_t &profile);
    void _setDefaults();
};

#endif

