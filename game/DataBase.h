
#ifndef __DATABASE__
#define __DATABASE__

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

    void getUserProfile(int id,UserProfile_t &profile);
protected:
    Database();
    ~Database();

    static Database *_instance;
private:
    const char  *ROBOTS_NAME[16];
    UserDefault *_userDefault;

    void _getRobotProfile(int id,UserProfile_t &profile);
    void _getMyProfile(int id,UserProfile_t &profile);
};

#endif

