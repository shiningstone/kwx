
#ifndef __DATABASE__
#define __DATABASE__

#define KWX_VERSION_CODE 0
#define KWX_VERSION_NAME "0.0.0.0"

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

    static void get_local_image(char *buf,const char *id);

    bool IsYinYueOn() const;
    bool IsYinXiaoOn() const;
    
    void GetUserProfile(int id,UserProfile_t &profile);
    void SetProperty(int id,int property);
    void GetVersion(int &code,char *name);
    void SetVersion(int code,const char *name);
    
protected:
    Database();
    ~Database();

    static Database *_instance;
private:
    static char  *ROBOTS_NAME[16];

    void _getRobotProfile(int id,UserProfile_t &profile);
    void _getMyProfile(int id,UserProfile_t &profile);
    void _setDefaults();
};

#endif

