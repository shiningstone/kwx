
#ifndef __DEBUG_CTRL__
#define __DEBUG_CTRL__

/**********************************************************************************
    网络版调试版本总开关,控制"单机版"入口
    0 单机版
    1 网络版
    2 其他功能入口
    3 版本升级UI测试
**********************************************************************************/
#define DEBUG_ENTRANCE 3

/**********************************************************************************
    网络版SERVER IP
        本机windows调试，   127.0.0.1
        手机调试版本，      本机局域网ip
        windows联调版本     实际的server ip
**********************************************************************************/
#if 0
#define DBG_LAN_SERVER "10.101.104.116"
#define DBG_LOGIN_PORT 20000
#else
#define DBG_LAN_SERVER "127.0.0.1"
#define DBG_LOGIN_PORT 60905
#endif

/**********************************************************************************
    本机服务器测试控制
        由于本机服务器只支持一个socket连接，只能模拟登陆服务器和房间服务器中的一个
**********************************************************************************/
/* 不发送RequestLogin - 使用本机服务器测试时设置 */
//#define IGNORE_LOGIN_REQUEST
/* 不发送DailyLogin   - 使用本机服务器测试LoginRequest时设置*/
#define IGNORE_DAILY_LOGIN_REQUEST

/* 使用预先设置好的牌局 - 单机版测试 */
//#define USE_TEST_ROUND

/* 倒计时超时不强制出牌 */
#define TIMER_FREE

/* 不发送心跳 */
#define NO_HEART_BEAT

#endif

