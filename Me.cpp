#include "Me.h"

USING_NS_CC;
Me::Me()
{

}
Me::~Me()
{
}

void Me::set_robot_hu_target(ROBOT_TARGET par_target)
{

}
ROBOT_TARGET Me::get_robot_hu_target()
{
	return PI_HU_TARGET;
}

void Me::set_player_id(int player_id)
{
	idOfPlayer=player_id;
}
bool Me::get_player_id(int & player_id)
{
	player_id=idOfPlayer;
	return true;
}
void Me::set_photo(const std::string pho)
{
	photo=pho;
}
bool Me::get_photo(std::string & pho)
{
	pho=photo;
	return true;
}
void Me::set_nick_name(const std::string name)
{
	nickName=name;
}
bool Me::get_nick_name(std::string & name)
{
	name=nickName;
	return true;
}

unsigned char Me::init(int card_array[],int len,int aim)
{
	return parter->init(card_array,len,aim);
}

Raction* Me::get_parter()
{
	return parter;
}
void Me::set_parter(Raction* p_parter)
{
	parter=p_parter;
}
void Me::set_property(int pro)
{
	property=pro;
}

bool Me::get_property(int & pro)
{
	pro=property;
	return true;
}
void Me::set_sex(std::string sex)
{
	playerSex=sex;
}
bool Me::get_sex(std::string & sex)
{
	sex=playerSex;
	return true;
}
void Me::set_language(std::string language)
{
	playerLanguage=language;
}
bool Me::get_language(std::string & language)
{
	language=playerLanguage;
	return true;
}