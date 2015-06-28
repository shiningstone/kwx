
#ifndef __DEBUG_CTRL__
#define __DEBUG_CTRL__

#define LAN_SERVER "192.168.1.101"
/**********************************************************************************
    �������԰汾�ܿ��أ�"������" ֱ�ӿ���������ƾ�
**********************************************************************************/
#define NETWORK_GAME_DEBUG

/********************************************************************************** 
    this macro is used to control the client is connected to 
        1 : local server(127.0.0.1);                                      window���԰汾���뱾����ģ�������ͨѶ
        2 : LAN_SERVER;                                                   android���԰汾�����ص��ֻ����뱾��ģ�������ͨѶ
        3 : remote server(the ip address is stored in E:/server_ip.txt)   window���԰汾������ʵ������ͨѶ
**********************************************************************************/
#define DBG_REMOTE_SERVER 2

#if (DBG_REMOTE_SERVER!=3)
/* ������RequestLogin - ���ط�������֧��һ��sockect���� */
//#define IGNORE_LOGIN_REQUEST
#endif

/* ʹ��Ԥ�����úõ��ƾ� - ��������� */
//#define USE_TEST_ROUND

/* ����ʱ��ʱ��ǿ�Ƴ��� */
#define TIMER_FREE

/* ���������� */
#define NO_HEART_BEAT

#endif

