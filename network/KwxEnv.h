
#ifndef __KWX_ENV_VARIABLES_H__
#define __KWX_ENV_VARIABLES_H__

#include "./../utils/UtilBasic.h"
#include "./../utils/UtilPlatform.h"

#include "./../protocol/KwxMsgBasic.h"

#include <string>
#include "./../game/GameType.h"
#include <vector>
using namespace std;
/*******************************************************************************
	NOTE:
		the value of environment variables should either 
		get from OTHER MODULES or set to a CONST VALUE, 
		while they are set to keep accordance with test cases during unit test
********************************************************************************/
typedef enum					//账户类型		
{
	Tourist,
	QQAccount,
	KWXAccount,
}Account_Type;

typedef struct {
    int  id;					//id
	std::string account;		//账号
	Account_Type accountType;	//账号类型
	std::string Nickname;		//昵称
	Sex_t PlayerSex;			//性别
    int Winrate;				//胜率
	int maxTime;				//最大番型
	std::string PhotoName;		//头像
	float PlayerLv;				//整数部分是等级，小数部分是当前等级经验占升级经验的多少
	int GoldNum;				//金币
}Personal_Information;

typedef struct {
	std::string IconPhoto;				//图片名称
	std::string IconName;				//物品名称
	std::string DetailedInformation;	//物品详细信息
	std::string ItemFragment[4];		//碎片   为空则此物品没有碎片，否则又四个，内容为按序图片名
	int ItemFragmentNum[4];				//碎片数量
	int Amount;							//数量
}Backpack_Item;

typedef struct {						//好友信息
	std::string photoFileName;
	std::string NikeName;
	Sex_t friendSex;
	int FriendGold;
	int _id;
}Friend_Info;

typedef struct {						//陌生人&&附近玩家
	std::string photoFileName;
	std::string NikeName;
	Sex_t friendSex;
	int _id;
}OtherPlayers_Info;

typedef struct {						//主页面需要的数据
	std::string photoFileName;
	std::string NikeName;
	int GoldNum;
	int QuanNum;
}MainLayer_myInfo;

typedef struct {						//排行榜
	std::string photoFileName;
	std::string NikeName;
	int RankingNum;
}TheCharts;

typedef struct
{											//商城物品
	std::string photoFileName;				//图片
	std::string GoodsName;					//物品名称
	std::string GoodsIntroduct;				//物品简介
	int GooldPrice;							//物品售价
	bool CanBuy;							//能否购买
}Mall_Items;
    
typedef struct {
    bool        hasReward;                          //是否已领取
    INT32U      continuousDays;						//连续登陆天数
    
    INT8U       image[128];							//这几个是做什么用的
    INT8U       name[128];
    INT32U      gold;
    INT32U      coupon;
    vector<Friend_Info>       friends;
    vector<OtherPlayers_Info> strangers;
    vector<OtherPlayers_Info> nearbys;
}DataDailyLogin_t;

class EnvVariable {
public:
	static EnvVariable *getInstance();

    void       SetReconnect(bool flag);
    bool       IsReconnectRequired();
    void       SetRoomServer(int roomId,const char *ip,int port,int voicePort);
    void       SetKey(Key_t key);
    Key_t      GetKey();
    void       SetUserId(INT32U id);
    INT32U     GetUserId();
    void       SetRoomPath(RoomPath_t id);
    RoomPath_t GetRoomPath();

    Key_t      _key;
    RoomPath_t _roomPath;
    
    INT8U    _protocol;
	INT32U   _userId;
    INT8U    _language;
    INT8U    _platform;
    INT8U    _buildNo;
    INT16U   _customerId;
    INT16U   _productId;

    DeviceInfo_t _device;
    
    Server_t     _loginServer;
    Server_t     _roomServer;

	DataDailyLogin_t _dailyLogin;

	/**************************/
	void set__dailyLogin();

	void set_personalSimple();		//主页面个人信息
	MainLayer_myInfo get_personalSimple();

	void set_MyFriendList();		//好友列表
	std::vector <Friend_Info> get_MyFriendList();

	void set_NearyPeopleList();		//附近玩家列表
	std::vector <OtherPlayers_Info> get_NearyPeopleList();

	void set_StrangersList();		//陌生人列表
	std::vector <OtherPlayers_Info> get_StrangersList();

	void set_personalDetailed();	//个人信息
	Personal_Information get_personalDetailed();

	void set_myBackpack();			//背包
	std::vector <Backpack_Item> get_myBackpack();

	void set_RaningList_DayGain();	//每日盈利
	std::vector <TheCharts> get_RaningList_DayGain();

	void set_RaningList_Riches();	//财富排行
	std::vector <TheCharts> get_RaningList_Riches();

	void set_RaningList_MaxType();	//最大番型
	std::vector <TheCharts> get_RaningList_MaxType();

	void set_RaningList_Record();	//战绩排行
	std::vector <TheCharts> get_RaningList_Record();

	void set_Mall_GoodList();	//商城物品
	std::vector <Mall_Items> get_Mall_GoodList();

	void TestDataFunction();									//测试数据初始化
	/**************************/
private:
	EnvVariable();
	static EnvVariable *_instance;

    bool       _reconnectRequired;

    static void SetServerIp(Server_t &server,const char *ip);
    void LoginServerInit();
    void RoomServerInit(int roomId);

	/**************************/
	Personal_Information personalDetailed;						//个人信息..
	std::vector <Friend_Info> MyFriendList;						//好友列表..
	std::vector <OtherPlayers_Info> NearyPeopleList;			//附近玩家..
	std::vector <OtherPlayers_Info> StrangersList;				//陌生人..
	std::vector <Backpack_Item> myBackpack;						//我的背包..
	MainLayer_myInfo personalSimple;							//主页面个人数据..
	std::vector <TheCharts> RaningList_DayGain;					//日盈利..
	std::vector <TheCharts> RaningList_Riches;					//财富榜..
	std::vector <TheCharts> RaningList_MaxType;					//最大番型..
	std::vector <TheCharts> RaningList_Record;					//战绩榜..
	std::vector <Mall_Items> Mall_GoodList;						//商城
	/**************************/
};

#endif
