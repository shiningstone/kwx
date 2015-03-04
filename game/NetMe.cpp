#include "NetMe.h"

USING_NS_CC;
NetMe::NetMe()
{
    _logger = LOGGER_REGISTER("NetMe");
}

NetMe::~NetMe()
{
    LOGGER_DEREGISTER(_logger);
}

void NetMe::set_robot_hu_target(ROBOT_TARGET par_target)
{

}
ROBOT_TARGET NetMe::get_robot_hu_target()
{
	return PI_HU_TARGET;
}

void NetMe::set_player_id(int player_id)
{
	idOfPlayer=player_id;
}
bool NetMe::get_player_id(int & player_id)
{
	player_id=idOfPlayer;
	return true;
}
void NetMe::set_photo(const std::string pho)
{
	photo=pho;
}
bool NetMe::get_photo(std::string & pho)
{
	pho=photo;
	return true;
}
void NetMe::set_nick_name(const std::string name)
{
	nickName=name;
}
bool NetMe::get_nick_name(std::string & name)
{
	name=nickName;
	return true;
}

unsigned char NetMe::init(int card_array[],int len,int aim)
{
    LOGGER_WRITE("NETWORK : %s should send the result",__FUNCTION__);
	return parter->init(card_array,len,aim);
}

Raction* NetMe::get_parter()
{
	return parter;
}
void NetMe::set_parter(Raction* p_parter)
{
	parter=p_parter;
}
void NetMe::set_property(int pro)
{
	property=pro;
}

bool NetMe::get_property(int & pro)
{
	pro=property;
	return true;
}
void NetMe::set_sex(std::string sex)
{
	playerSex=sex;
}
bool NetMe::get_sex(std::string & sex)
{
	sex=playerSex;
	return true;
}
void NetMe::set_language(std::string language)
{
	playerLanguage=language;
}
bool NetMe::get_language(std::string & language)
{
	language=playerLanguage;
	return true;
}
