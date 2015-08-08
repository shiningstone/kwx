
#include <stdio.h>
#include <string.h>

#include "./../utils/UtilPlatform.h"
#include "KwxEnv.h"

#include "./../utils/DebugCtrl.h"

#ifdef DBG_LAN_SERVER
#define LOGIN_SERVER_IP   DBG_LAN_SERVER
#define DEF_SERVER_IP     DBG_LAN_SERVER
#else
#define LOGIN_SERVER_IP   "127.0.0.1"
#define DEF_SERVER_IP     "127.0.0.1"
#endif

#ifdef DBG_LOGIN_PORT
#define LOGIN_SOCKET_PORT DBG_LOGIN_PORT
#else
#define LOGIN_SOCKET_PORT 20000
#endif

#define DEF_SOCKET_PORT   60905


EnvVariable *EnvVariable::_instance = 0;

EnvVariable *EnvVariable::getInstance() {
	if(_instance==0) {
		_instance = new EnvVariable();
	}

	return _instance;
}

void EnvVariable::SetServerIp(Server_t &server,const char *ip) {
    sprintf(server.ipaddr,"%s",ip);
}

void EnvVariable::LoginServerInit() {
    _loginServer.id = 0;
    sprintf(_loginServer.name,"loginServer");
    strcpy(_loginServer.ipaddr,LOGIN_SERVER_IP);
    _loginServer.port = LOGIN_SOCKET_PORT;
}

void EnvVariable::RoomServerInit(int roomId) {
    SetRoomServer(roomId,DEF_SERVER_IP,DEF_SOCKET_PORT,0);
}

void EnvVariable::SetRoomServer(int roomId,const char *ip,int port,int voicePort) {
    _roomServer.id = roomId;
    sprintf(_roomServer.name,"roomServer%d",roomId);
    SetServerIp(_roomServer,ip);
    _roomServer.port = port;
    _roomServer.voicePort = voicePort;
}

EnvVariable::EnvVariable() {
#ifdef WIN32
	_protocol = 16;                
	_userId   = 0x01020304;        
	_language = 5;                 
	_platform = 6;                 
	_buildNo  = BUILD_NUMBER;                 
	_customerId = 0x0809;          
	_productId  = 0x0a0b;
#else
	_protocol = 16;                //const value in terms of version
	_userId   = 0x01020304;        //set by device Id
	_language = 5;                 //set by user configuration
	_platform = 6;                 //set by device type
	_buildNo  = BUILD_NUMBER;      //const value in terms of version
	_customerId = 0x0809;          //???
	_productId  = 0x0a0b;          //???
#endif
    _reconnectRequired = false;
    _key      = 0x01020304;
    _roomPath = 11;

    LoginServerInit();
    RoomServerInit(0);
    
    _get_device_info(_device);

    memset(&_dailyLogin,0,sizeof(_dailyLogin));

	TestDataFunction();
}

void EnvVariable::SetReconnect(bool flag) {
    _reconnectRequired = flag;
}

bool EnvVariable::IsReconnectRequired() {
    return _reconnectRequired;
}

void EnvVariable::SetKey(Key_t key) {
    _key = key;
}

Key_t EnvVariable::GetKey() {
    return _key;
}

void EnvVariable::SetUserId(INT32U id) {
    _userId = id;
}

INT32U EnvVariable::GetUserId() {
    return _userId;
}

void EnvVariable::SetRoomPath(RoomPath_t id) {
    _roomPath = id;
}

RoomPath_t EnvVariable::GetRoomPath() {
	return _roomPath;
}
/*****************************************************/
//void EnvVariable::RoomServerInit(int roomId)
//{
//}






/******************”‡Àº*******************************/
void EnvVariable::set__dailyLogin()
{
	_dailyLogin.hasReward=true;
	_dailyLogin.continuousDays=1;

    _dailyLogin.gold = 5000;
    _dailyLogin.coupon = 5000;
    strcpy((char *)_dailyLogin.name,"tempName");
    strcpy((char *)_dailyLogin.image,"Head17.png");
}
void EnvVariable::set_MyFriendList()
{
	Friend_Info tempInfo={"Head17.png","tempName",BOY,5000,12345678};
	for(int a=0;a<20;a++)
		MyFriendList.push_back(tempInfo);
}
void EnvVariable::set_NearyPeopleList()
{
	OtherPlayers_Info tempInfo={"Head17.png","tempName",BOY,12345678};
	for(int a=0;a<20;a++)
		NearyPeopleList.push_back(tempInfo);
}
void EnvVariable::set_StrangersList()
{
	OtherPlayers_Info tempInfo={"Head17.png","tempName",BOY,12345678};
	for(int a=0;a<20;a++)
		StrangersList.push_back(tempInfo);
}
void EnvVariable::set_personalDetailed()
{
	Personal_Information tempInfo={12345678,"ye10203",Tourist,"tempName",BOY,50,128,"Head17.png",3.254,5000};
	personalDetailed=tempInfo;
}
void EnvVariable::set_myBackpack()
{
	Backpack_Item tempItem={"Cheats-dgbf.png","¥Úπ∑∞Ù∑®","∫«∫«°£°£",{"Cheats-da.png","Cheats-gou.png","Cheats-bang.png","Cheats-fa.png"},{2,5,4,3},1};
	for(int a=0;a<6;a++)
		myBackpack.push_back(tempItem);
}
void EnvVariable::set_RaningList_DayGain()
{
	TheCharts tempItem={"Head17.png","tempName",5000};
	for(int a=0;a<20;a++)
		RaningList_DayGain.push_back(tempItem);
}
void EnvVariable::set_RaningList_Riches()
{
	TheCharts tempItem={"Head17.png","tempName",5000};
	for(int a=0;a<20;a++)
		RaningList_Riches.push_back(tempItem);
}
void EnvVariable::set_RaningList_MaxType()
{
	TheCharts tempItem={"Head17.png","tempName",5000};
	for(int a=0;a<20;a++)
		RaningList_MaxType.push_back(tempItem);
}
void EnvVariable::set_RaningList_Record()
{
	TheCharts tempItem={"Head17.png","tempName",5000};
	for(int a=0;a<20;a++)
		RaningList_Record.push_back(tempItem);
}
void EnvVariable::set_Mall_GoodList()
{
	Mall_Items tempItem={"Head17.png","±Ÿ–∞Ω£∆◊","∫«∫«°£°£°£",500,true};
	for(int a=0;a<10;a++)
		Mall_GoodList.push_back(tempItem);
}
void EnvVariable::set_Searched_Friend()
{
	OtherPlayers_Info temp={"Head17.png","tempName",BOY,12346789};
	searchedFriend=temp;
}
OtherPlayers_Info EnvVariable::get_Searched_Friend()
{
	return searchedFriend;
}
std::vector <Mall_Items> EnvVariable::get_Mall_GoodList()
{
	return Mall_GoodList;
}
std::vector <Friend_Info> EnvVariable::get_MyFriendList()
{
	return MyFriendList;
}
std::vector <OtherPlayers_Info> EnvVariable::get_NearyPeopleList()
{
	return NearyPeopleList;
}
std::vector <OtherPlayers_Info> EnvVariable::get_StrangersList()
{
	return StrangersList;
}
Personal_Information EnvVariable::get_personalDetailed()
{
	return personalDetailed;
}
std::vector <Backpack_Item> EnvVariable::get_myBackpack()
{
	return myBackpack;
}
std::vector <TheCharts> EnvVariable::get_RaningList_DayGain()
{
	return RaningList_DayGain;
}
std::vector <TheCharts> EnvVariable::get_RaningList_Riches()
{
	return RaningList_Riches;
}
std::vector <TheCharts> EnvVariable::get_RaningList_MaxType()
{
	return RaningList_MaxType;
}
std::vector <TheCharts> EnvVariable::get_RaningList_Record()
{
	return RaningList_Record;
}
void EnvVariable::TestDataFunction()
{
	set__dailyLogin();
	set_MyFriendList();
	set_NearyPeopleList();
	set_StrangersList();
	set_personalDetailed();
	set_myBackpack();
	set_RaningList_DayGain();
	set_RaningList_Riches();
	set_RaningList_MaxType();
	set_RaningList_Record();
	set_Mall_GoodList();
	set_Searched_Friend();
}


