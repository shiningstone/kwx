
#ifndef __DEBUG_CTRL__
#define __DEBUG_CTRL__

/**********************************************************************************
    this macro provides a shortcut to start a network game by Button "DanJiBan" 
**********************************************************************************/
#define NETWORK_GAME_DEBUG

/********************************************************************************** 
    this macro is used to control the client is connected to 
        local server(127.0.0.1) OR 
        remote server(the ip address is stored in E:/server_ip.txt)
**********************************************************************************/
#define USE_REMOTE_SERVER

/********************************************************************************** 
    this macro is used to control the timer limitation
        round manager will ignore the count if this macro is enabled
**********************************************************************************/
#define TIMER_FREE

#define NO_HEART_BEAT

#endif
