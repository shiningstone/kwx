
#include "DataBase.h"

char *Database::ROBOTS_NAME[16] = {
    "ʮ�߱���",
    "������ϼ",
    "�人���",
    "����Τͮ",
	"Т�����",
	"��������",
	"�������u",
	"��ɽ����",
	"��������",
	"ʮ��С��",
	"����С��",
	"�人�ظ�",
	"����С��",
	"Т��С��",
	"��������",
	"����С��",
};

Database::Database() {
    _userDefault = UserDefault::getInstance();
    
    if( _userDefault->getBoolForKey("userDefault")==false ) {
        _setDefaults();
    }
}

Database::~Database() {

}

void Database::SetProperty(int id,int property) {
    char title[32] = {0};
    
    if (id<17) {
        sprintf(title,"PropertyOfRobot%d",id);
    } else {
        sprintf(title,"MyProperty");
    }
    
    _userDefault->setIntegerForKey(title,property);
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

    strcpy(profile.name, _userDefault->getStringForKey(name).c_str());
    profile.property = _userDefault->getIntegerForKey(property);
    strcpy(profile.level, _userDefault->getStringForKey(level).c_str());
    strcpy(profile.photo, _userDefault->getStringForKey(photo).c_str());
    strcpy(profile.sex, _userDefault->getStringForKey(sex).c_str());
    strcpy(profile.language, _userDefault->getStringForKey(language).c_str());
}

void Database::_getMyProfile(int id,UserProfile_t &profile) {
    char name[32]     = "MyNickName";
    char property[32] = "MyProperty";
    char level[32]    = "MyLevel";
    char photo[32]    = "MyPhoto";
    char sex[32]      = "MySex";
    char language[32] = "Language0";

    strcpy(profile.name, _userDefault->getStringForKey(name).c_str());
    profile.property = _userDefault->getIntegerForKey(property);
    strcpy(profile.level, _userDefault->getStringForKey(level).c_str());
    strcpy(profile.photo, _userDefault->getStringForKey(photo).c_str());
    strcpy(profile.sex, _userDefault->getStringForKey(sex).c_str());
    strcpy(profile.language, _userDefault->getStringForKey(language).c_str());
}

void Database::_setDefaults() {
	_userDefault->setBoolForKey(   "userDefault",true);
	_userDefault->setIntegerForKey("load_days",0);
	_userDefault->setIntegerForKey("cur_sta",  1);
	_userDefault->setIntegerForKey("load_time",7352);

	_userDefault->setStringForKey("MyNickName","ȸ��");
	_userDefault->setIntegerForKey("MyProperty",50000);
	_userDefault->setStringForKey( "MyLevel",   "����");
	_userDefault->setStringForKey( "MyPhoto",   "Head17.png");
	_userDefault->setStringForKey( "MySex",     "Boy");
    
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

		_userDefault->setStringForKey(name,ROBOTS_NAME[i]);
		_userDefault->setIntegerForKey(property,50000);
		_userDefault->setStringForKey(level,"����");
		_userDefault->setStringForKey(photo,photoPath);
		if(i<9) {
			_userDefault->setStringForKey(sex,"Girl");
        }
		else{
			_userDefault->setStringForKey(sex,"Boy");
        }
	}

    char language[32] = "Language0";
    _userDefault->setStringForKey(language,"PuTongBan");
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

