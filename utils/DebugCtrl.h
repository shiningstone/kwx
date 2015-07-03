
#ifndef __DEBUG_CTRL__
#define __DEBUG_CTRL__

#define DBG_LAN_SERVER "192.168.1.101"
#define DBG_LOGIN_PORT 60905
/**********************************************************************************
    网络版调试版本总开关,控制"单机版"入口
    0 单机版
    1 网络版
    2 其他功能入口
**********************************************************************************/
#define DEBUG_ENTRANCE 2

/* 不发送RequestLogin - 本地服务器仅支持一个sockect连接 */
//#define IGNORE_LOGIN_REQUEST

/* 使用预先设置好的牌局 - 单机版测试 */
//#define USE_TEST_ROUND

/* 倒计时超时不强制出牌 */
#define TIMER_FREE

/* 不发送心跳 */
#define NO_HEART_BEAT

#endif

