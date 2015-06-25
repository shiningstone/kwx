
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
//	std::string account;		//�˺�
//	Account_Type accountType;	//�˺�����
//	std::string Nickname;		//�ǳ�
//	Sex_t PlayerSex;			//�Ա�
//    int Winrate;				//ʤ��
//	int maxTime;				//�����
//	std::string PhotoName;		//ͷ��
//	float PlayerLv;				//���������ǵȼ���С�������ǵ�ǰ�ȼ�����ռ��������Ķ���
//	int GoldNum;				//���
//}Personal_Information;
//
//typedef struct {
//	std::string IconPhoto;				//ͼƬ����
//	std::string IconName;				//��Ʒ����
//	std::string DetailedInformation;	//��Ʒ��ϸ��Ϣ
//	std::string ItemFragment[4];		//��Ƭ   Ϊ�������Ʒû����Ƭ���������ĸ�������Ϊ����ͼƬ��
//	int ItemFragmentNum[4];				//��Ƭ����
//	int Amount;							//����
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
