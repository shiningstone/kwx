
#ifndef __DEBUG_CTRL__
#define __DEBUG_CTRL__

#define LAN_SERVER "192.168.1.101"
/**********************************************************************************
    网络版调试版本总开关，"单机版" 直接开启网络版牌局
**********************************************************************************/
#define NETWORK_GAME_DEBUG

/********************************************************************************** 
    this macro is used to control the client is connected to 
        1 : local server(127.0.0.1);                                      window调试版本，与本机的模拟服务器通讯
        2 : LAN_SERVER;                                                   android调试版本，下载到手机，与本地模拟服务器通讯
        3 : remote server(the ip address is stored in E:/server_ip.txt)   window调试版本，与真实服务器通讯
**********************************************************************************/
#define DBG_REMOTE_SERVER 2

#if (DBG_REMOTE_SERVER!=3)
/* 不发送RequestLogin - 本地服务器仅支持一个sockect连接 */
//#define IGNORE_LOGIN_REQUEST
#endif

/* 使用预先设置好的牌局 - 单机版测试 */
//#define USE_TEST_ROUND

/* 倒计时超时不强制出牌 */
#define TIMER_FREE

/* 不发送心跳 */
#define NO_HEART_BEAT

#endif

