
#ifndef __DEBUG_CTRL__
#define __DEBUG_CTRL__

/**********************************************************************************
    编手机版本统一定义(无需针对单个宏进行调整)
**********************************************************************************/
//#define FORMAL_ANDROID

/**********************************************************************************
    网络版调试版本总开关,控制"单机版"入口
    0 单机版
    1 网络版
    2 其他功能入口
    3 版本升级UI测试
    4 网络通信测试
    5 错误提示
**********************************************************************************/
#define DEBUG_ENTRANCE 0

/**********************************************************************************
    网络版SERVER IP
        本机windows调试，   127.0.0.1
        手机调试版本，      本机局域网ip
        windows联调版本     实际的server ip
**********************************************************************************/
#if 1
#define DBG_LAN_SERVER "120.25.169.221"
#define DBG_LOGIN_PORT 20000
#else
#define DBG_LAN_SERVER "127.0.0.1"
#define DBG_LOGIN_PORT 60905
#endif

/**********************************************************************************
    本机服务器测试控制
        由于本机服务器只支持一个socket连接，只能模拟登陆服务器和房间服务器中的一个
**********************************************************************************/
#if (DBG_LOGIN_PORT==60905)
/* 不发送RequestLogin - 使用本机服务器测试时设置 */
#define IGNORE_LOGIN_REQUEST
/* 不发送DailyLogin   - 使用本机服务器测试LoginRequest时设置*/
#define IGNORE_DAILY_LOGIN_REQUEST
#endif



/* 单机版使用预先设置好的牌局 - 单机版测试 */
//#define USE_TEST_ROUND

/* 倒计时超时不强制出牌 */
#define TIMER_FREE

/* 不发送心跳 */
#define NO_HEART_BEAT

/* log带时间 */
#define LOG_TIME_ENABLE

/**********************************************************************************
    编手机版本统一定义(无需针对单个宏进行调整)
**********************************************************************************/
#ifdef FORMAL_ANDROID
    #define DEBUG_ENTRANCE 0

    #define DBG_LAN_SERVER "120.25.169.221"
    #define DBG_LOGIN_PORT 20000

    #undef IGNORE_LOGIN_REQUEST
    #undef IGNORE_DAILY_LOGIN_REQUEST

    #undef  USE_TEST_ROUND
    #undef  TIMER_FREE
    #define NO_HEART_BEAT
#endif

#endif

