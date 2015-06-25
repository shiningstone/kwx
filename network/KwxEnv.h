
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
    
typedef struct {
    bool        hasReward;                          //�Ƿ�����ȡ
    INT32U      continuousDays;						//������½����
    INT32U      dailyReward;						//��ǰ��������
    
    INT8U       image[128];							//�⼸������ʲô�õ�
    INT8U       name[128];
    INT32U      gold;
    INT32U      coupon;
    INT8U       friends;
    INT8U       strangers;
}DataDailyLogin_t;

typedef enum					//�˻�����		
{
	Tourist,
	QQAccount,
	KWXAccount,
}Account_Type;

typedef struct {
    int  id;					//id
	std::string account;		//�˺�
	Account_Type accountType;	//�˺�����
	std::string Nickname;		//�ǳ�
	Sex_t PlayerSex;			//�Ա�
    int Winrate;				//ʤ��
	int maxTime;				//�����
	std::string PhotoName;		//ͷ��
	float PlayerLv;				//���������ǵȼ���С�������ǵ�ǰ�ȼ�����ռ��������Ķ���
	int GoldNum;				//���
}Personal_Information;

typedef struct {
	std::string IconPhoto;				//ͼƬ����
	std::string IconName;				//��Ʒ����
	std::string DetailedInformation;	//��Ʒ��ϸ��Ϣ
	std::string ItemFragment[4];		//��Ƭ   Ϊ�������Ʒû����Ƭ���������ĸ�������Ϊ����ͼƬ��
	int ItemFragmentNum[4];				//��Ƭ����
	int Amount;							//����
}Backpack_Item;

typedef struct {						//������Ϣ
	std::string photoFileName;
	std::string NikeName;
	Sex_t friendSex;
	int FriendGold;
	int _id;
}Friend_Info;

typedef struct {						//İ����&&�������
	std::string photoFileName;
	std::string NikeName;
	Sex_t friendSex;
	int _id;
}OtherPlayers_Info;

typedef struct {						//��ҳ����Ҫ������
	std::string photoFileName;
	std::string NikeName;
	int GoldNum;
	int QuanNum;
}MainLayer_myInfo;

typedef struct {						//���а�
	std::string photoFileName;
	std::string NikeName;
	int RankingNum;
}TheCharts;

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

	void set_personalSimple();		//��ҳ�������Ϣ
	MainLayer_myInfo get_personalSimple();

	void set_MyFriendList();		//�����б�
	std::vector <Friend_Info> get_MyFriendList();

	void set_NearyPeopleList();		//��������б�
	std::vector <OtherPlayers_Info> get_NearyPeopleList();

	void set_StrangersList();		//İ�����б�
	std::vector <OtherPlayers_Info> get_StrangersList();

	void set_personalDetailed();	//������Ϣ
	Personal_Information get_personalDetailed();

	void set_myBackpack();			//����
	std::vector <Backpack_Item> get_myBackpack();

	void TestDataFunction();									//�������ݳ�ʼ��
	/**************************/
private:
	EnvVariable();
	static EnvVariable *_instance;

    bool       _reconnectRequired;

    static void SetServerIp(Server_t &server,const char *ip);
    void LoginServerInit();
    void RoomServerInit(int roomId);

	/**************************/
	Personal_Information personalDetailed;						//������Ϣ
	std::vector <Friend_Info> MyFriendList;						//�����б�
	std::vector <OtherPlayers_Info> NearyPeopleList;			//�������
	std::vector <OtherPlayers_Info> StrangersList;				//İ����
	std::vector <Backpack_Item> myBackpack;						//�ҵı���
	MainLayer_myInfo personalSimple;							//��ҳ���������
	std::vector <TheCharts> RaningList_DayGain;					//��ӯ��
	std::vector <TheCharts> RaningList_Riches;					//�Ƹ���
	std::vector <TheCharts> RaningList_MaxType;					//�����
	std::vector <TheCharts> RaningList_Record;					//ս����
	/**************************/
};

#endif
