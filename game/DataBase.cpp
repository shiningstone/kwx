#include "cocos2d.h"
USING_NS_CC;

#include "DataBase.h"

static UserDefault *sCocos2dUserDefault = NULL;

char *Database::ROBOTS_NAME[16] = {
    "十堰冰冰",
    "随州青霞",
    "武汉丽缇",
    "襄阳韦彤",
	"孝感歆艺",
	"郧西子怡",
	"郧县若瑄",
	"竹山紫琪",
	"房县老李",
	"十堰小李",
	"随州小张",
	"武汉关哥",
	"襄阳小刘",
	"孝感小王",
	"郧西老杨",
	"郧县小陈",
};

Database::Database() {
    sCocos2dUserDefault = UserDefault::getInstance();
    
    if( sCocos2dUserDefault->getBoolForKey("userDefault")==false ) {
        _setDefaults();
    }
}

Database::~Database() {
    sCocos2dUserDefault = NULL;
}

void Database::SetProperty(int id,int property) {
    char title[32] = {0};
    
    if (id<17) {
        sprintf(title,"PropertyOfRobot%d",id);
    } else {
        sprintf(title,"MyProperty");
    }
    
    sCocos2dUserDefault->setIntegerForKey(title,property);
}

void Database::SetVersion(int code,const char * name) {
	sCocos2dUserDefault->setIntegerForKey("VersionCode",code);
	sCocos2dUserDefault->setStringForKey( "VersionName",name);
}

void Database::GetVersion(int &code,char *name) {
    code = sCocos2dUserDefault->getIntegerForKey("VersionCode");
    strcpy(name, sCocos2dUserDefault->getStringForKey("VersionName").c_str());
}

void Database::get_local_image(char *buf,const char *id) {
    char photo[32]    = {0};

    sprintf(photo,"PhotoOfRobot");
    strcat(photo,id);

    strcpy(buf, sCocos2dUserDefault->getStringForKey(photo).c_str());
}

bool Database::IsYinYueOn() const {
    return sCocos2dUserDefault->getBoolForKey("ifYinYueOn");
}

bool Database::IsYinXiaoOn() const {
    return sCocos2dUserDefault->getBoolForKey("ifYinXiaoOn");
}

void Database::GetUserProfile(int id,UserProfile_t &profile) {
    if (id<17) {
        return _getRobotProfile(id,profile);
    } else {
        return _getMyProfile(id,profile);
    }
}

void Database::_getRobotProfile(int id,UserProfile_t &profile) {
    char name[32]     = {0};
    char property[32] = {0};
    char level[32]    = {0};
    char photo[32]    = {0};
    char sex[32]      = {0};
    char language[32] = "Language0";
    
    sprintf(name,"Robot%d",id);
    sprintf(property,"PropertyOfRobot%d",id);
    sprintf(level,"LevelOfRobot%d",id);
    sprintf(photo,"PhotoOfRobot%d",id);
    sprintf(sex,"SexOfRobot%d",id);

    strcpy(profile.name, sCocos2dUserDefault->getStringForKey(name).c_str());
    profile.property = sCocos2dUserDefault->getIntegerForKey(property);
    strcpy(profile.level, sCocos2dUserDefault->getStringForKey(level).c_str());
    strcpy(profile.photo, sCocos2dUserDefault->getStringForKey(photo).c_str());
    strcpy(profile.sex, sCocos2dUserDefault->getStringForKey(sex).c_str());
    strcpy(profile.language, sCocos2dUserDefault->getStringForKey(language).c_str());
}

void Database::_getMyProfile(int id,UserProfile_t &profile) {
    char name[32]     = "MyNickName";
    char property[32] = "MyProperty";
    char level[32]    = "MyLevel";
    char photo[32]    = "MyPhoto";
    char sex[32]      = "MySex";
    char language[32] = "Language0";

    strcpy(profile.name, sCocos2dUserDefault->getStringForKey(name).c_str());
    profile.property = sCocos2dUserDefault->getIntegerForKey(property);
    strcpy(profile.level, sCocos2dUserDefault->getStringForKey(level).c_str());
    strcpy(profile.photo, sCocos2dUserDefault->getStringForKey(photo).c_str());
    strcpy(profile.sex, sCocos2dUserDefault->getStringForKey(sex).c_str());
    strcpy(profile.language, sCocos2dUserDefault->getStringForKey(language).c_str());
}

void Database::_setDefaults() {
	sCocos2dUserDefault->setBoolForKey(   "userDefault",true);
	sCocos2dUserDefault->setIntegerForKey("load_days",0);
	sCocos2dUserDefault->setIntegerForKey("cur_sta",  1);
	sCocos2dUserDefault->setIntegerForKey("load_time",7352);

	sCocos2dUserDefault->setStringForKey("MyNickName","雀友");
	sCocos2dUserDefault->setIntegerForKey("MyProperty",50000);
	sCocos2dUserDefault->setStringForKey( "MyLevel",   "菜鸟");
	sCocos2dUserDefault->setStringForKey( "MyPhoto",   "Head17.png");
	sCocos2dUserDefault->setStringForKey( "MySex",     "Boy");
    
    for(int i=0;i<16;i++)
	{
        char name[32]     = {0};
        char property[32] = {0};
        char level[32]    = {0};
        char photo[32]    = {0};
        char sex[32]      = {0};

        char photoPath[32] = {0};
    
		sprintf(name,"Robot%d",i);
		sprintf(property,"PropertyOfRobot%d",i);
		sprintf(level,"LevelOfRobot%d",i);
		sprintf(photo,"PhotoOfRobot%d",i);
		sprintf(sex,"SexOfRobot%d",i);
        
		sprintf(photoPath,"Head%d.png",i+1);

		sCocos2dUserDefault->setStringForKey(name,ROBOTS_NAME[i]);
		sCocos2dUserDefault->setIntegerForKey(property,50000);
		sCocos2dUserDefault->setStringForKey(level,"菜鸟");
		sCocos2dUserDefault->setStringForKey(photo,photoPath);
		if(i<9) {
			sCocos2dUserDefault->setStringForKey(sex,"Girl");
        }
		else{
			sCocos2dUserDefault->setStringForKey(sex,"Boy");
        }
	}

    char language[32] = "Language0";
    sCocos2dUserDefault->setStringForKey(language,"PuTongBan");

    sCocos2dUserDefault->setIntegerForKey("VersionCode",KWX_VERSION_CODE);
    sCocos2dUserDefault->setStringForKey("VersionName",KWX_VERSION_NAME);

	sCocos2dUserDefault->setBoolForKey("ifYinYueOn",true);
	sCocos2dUserDefault->setBoolForKey("ifYinXiaoOn",true);
	sCocos2dUserDefault->setBoolForKey("ifZhenDongOn",false);
	sCocos2dUserDefault->setBoolForKey("ifFangYanOn",false);

	sCocos2dUserDefault->flush();
}

/******************************************
        SINGLETON
******************************************/
Database *Database::_instance = NULL;

Database *Database::getInstance() {
    if (_instance==NULL) {
        _instance = new Database();
    }

    return _instance;
}

void Database::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

