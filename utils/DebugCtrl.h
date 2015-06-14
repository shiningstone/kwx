
#ifndef __DEBUG_CTRL__
#define __DEBUG_CTRL__

/**********************************************************************************
    this macro provides a shortcut to start a network game by Button "DanJiBan" 
**********************************************************************************/
#define NETWORK_GAME_DEBUG

/********************************************************************************** 
    this macro is used to control the client is connected to 
        1 : local server(127.0.0.1);          Vs virtual phone, test with Project TestServer
        2 : LAN server(192.168.1.102);        Real phone, test with Project TestServer
        3 : remote server(the ip address is stored in E:/server_ip.txt)   Vs virtual phone, test with real server
**********************************************************************************/
#define DBG_REMOTE_SERVER 1
#define LAN_SERVER "192.168.1.102"
/********************************************************************************** 
    this macro is used to control the timer limitation
        round manager will ignore the count if this macro is enabled
**********************************************************************************/
#define TIMER_FREE
//#define USE_LOCAL_PROFILE
#define NO_HEART_BEAT
//#define USE_TEST_ROUND

#endif
