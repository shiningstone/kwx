
#ifndef __KWX_ENV_VARIABLES_H__
#define __KWX_ENV_VARIABLES_H__

#include "./../utils/UtilBasic.h"
#include "./../utils/UtilPlatform.h"

#include "./../protocol/KwxMsgBasic.h"
/*******************************************************************************
	NOTE:
		the value of environment variables should either 
		get from OTHER MODULES or set to a CONST VALUE, 
		while they are set to keep accordance with test cases during unit test
********************************************************************************/
    
typedef struct {
    bool        hasReward;
    INT32U      continuousDays;
    INT32U      dailyReward;
    
    INT8U       image[128];
    INT8U       name[128];
    INT32U      gold;
    INT32U      coupon;
    INT8U       friends;
    INT8U       strangers;
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
private:
	EnvVariable();
	static EnvVariable *_instance;

    bool       _reconnectRequired;

    static void SetServerIp(Server_t &server,const char *ip);
    void LoginServerInit();
    void RoomServerInit(int roomId);
};

#endif
