
#include "NetRole.h"

NetRole::NetRole() {
    _logger = LOGGER_REGISTER("NetRole");
}

NetRole::NetRole(int id) {//this is for default settings ( robot ) 
    _logger = LOGGER_REGISTER("NetRole_%d");
}

NetRole::~NetRole() {
    LOGGER_DEREGISTER(_logger);
}

void NetRole::set_sex(std::string sex) {
    _sex = (sex=="Boy")? BOY : GIRL;
}

Sex_t NetRole::get_sex() {
    return _sex;
}

void NetRole::set_robot_hu_target(ROBOT_TARGET par_target)
{

}
ROBOT_TARGET NetRole::get_robot_hu_target()
{
	return PI_HU_TARGET;
}

unsigned char NetRole::init(int card_array[],int len,int aim)
{
	return parter->init(card_array,len,aim);
}

void NetRole::set_player_id(int player_id)
{
	idOfPlayer=player_id;
}
bool NetRole::get_player_id(int & player_id)
{
	player_id=idOfPlayer;
	return true;
}
void NetRole::set_photo(const std::string pho)
{
	photo=pho;
}
bool NetRole::get_photo(std::string & pho)
{
	pho=photo;
	return true;
}
void NetRole::set_nick_name(const std::string name)
{
	nickName=name;
}
bool NetRole::get_nick_name(std::string & name)
{
	name=nickName;
	return true;
}

NetRRound* NetRole::get_parter()
{
	return parter;
}
void NetRole::set_parter(NetRRound* p_parter)
{
	parter=p_parter;
}
void NetRole::set_property(int pro)
{
	property=pro;
}

bool NetRole::get_property(int & pro)
{
	pro=property;
	return true;
}
void NetRole::set_language(std::string language)
{
	playerLanguage=language;
}
bool NetRole::get_language(std::string & language)
{
	language=playerLanguage;
	return true;
}

