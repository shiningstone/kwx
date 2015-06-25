
#ifndef __LOGIN_TYPE__

#define __LOGIN_TYPE__

#include <string>
#include "utils/UtilBasic.h"
#include "game/GameType.h"

typedef struct _SeatInfo_t {
    INT32U   roomPath;
    INT32U   roomId;
    INT32U   tableId;
    INT32U   seatId;
}SeatInfo_t;

//typedef enum
//{
//	Tourist,
//	QQAccount,
//	KWXAccount,
//}Account_Type;
//
//typedef struct {
//    int  id;					//id
//	std::string account;		//账号
//	Account_Type accountType;	//账号类型
//	std::string Nickname;		//昵称
//	Sex_t PlayerSex;			//性别
//    int Winrate;				//胜率
//	int maxTime;				//最大番型
//	std::string PhotoName;		//头像
//	float PlayerLv;				//整数部分是等级，小数部分是当前等级经验占升级经验的多少
//	int GoldNum;				//金币
//}Personal_Information;
//
//typedef struct {
//	std::string IconPhoto;				//图片名称
//	std::string IconName;				//物品名称
//	std::string DetailedInformation;	//物品详细信息
//	std::string ItemFragment[4];		//碎片   为空则此物品没有碎片，否则又四个，内容为按序图片名
//	int ItemFragmentNum[4];				//碎片数量
//	int Amount;							//数量
//}Backpack_Item;
//
//typedef struct {
//	std::string photoFileName;
//	std::string NikeName;
//	Sex_t friendSex;
//	int FriendGold;
//}Friend_Info;
//
//typedef struct {
//	std::string photoFileName;
//	std::string NikeName;
//	Sex_t friendSex;
//}OtherPlayers_Info;
//
//typedef struct {
//	std::string photoFileName;
//	std::string NikeName;
//	int GoldNum;
//	int QuanNum;
//}MainLayer_myInfo;
//
//typedef struct {
//	std::string photoFileName;
//	std::string NikeName;
//	int RankingNum;
//}TheCharts;

#endif
