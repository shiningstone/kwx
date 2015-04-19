
#include <assert.h>
#include <stdio.h>

#include "./../../utils/UtilBasic.h"
#include "./../../network/CSockets.h"

typedef enum {
    STRING,
    HEX,
};

#define BUF_LEN 2048

void test_string_to_hex() {
    char buf[BUF_LEN] = {0};

    char testcase1[] = "0x4b,0x57,0x58,0x00,0x2b,0x07,0x00,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x83,0x00,0x04,0x00,0x00,0x00,0x01";
    char expcase1[] = {
        0x4b,0x57,0x58,0x00,0x2b,0x07,0x00,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x83,0x00,0x04,0x00,0x00,0x00,0x01};

    _bytes(buf,testcase1);
    assert(!memcmp(buf,expcase1,sizeof(expcase1)));

    char testcase2[] = "0x4b";
    char expcase2[] = {0x4b};

    _bytes(buf,testcase2);
    assert(!memcmp(buf,expcase2,sizeof(expcase2)));

    char testcase3[] = "0xb";
    char expcase3[] = {0xb};

    _bytes(buf,testcase3);
    assert(!memcmp(buf,expcase3,sizeof(expcase3)));

    char testcase4[] = "10";
    char expcase4[] = {0xa};

    _bytes(buf,testcase4);
    assert(!memcmp(buf,expcase4,sizeof(expcase4)));

    char testcase5[] = "1";
    char expcase5[] = {1};

    _bytes(buf,testcase5);
    assert(!memcmp(buf,expcase5,sizeof(expcase5)));

    char testcase51[] = "5,1";
    char expcase51[] = {5,1};

    _bytes(buf,testcase51);
    assert(!memcmp(buf,expcase51,sizeof(expcase51)));

    char testcase6[] = "131";
    char expcase6[] = {0x83};

    _bytes(buf,testcase6);
    assert(!memcmp(buf,expcase6,sizeof(expcase6)));

    char testcase7[] = "0x00,43,7,0x00,28,0,0,0,0,0,0,0,0,0,0,0,0,1,131,0,4,0,0,0,1";
    char expcase7[] = {
        0x00,0x2b,0x07,0x00,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x83,0x00,0x04,0x00,0x00,0x00,0x01};

    char testcase8[] = "0x4b,0x57,0x58,0x00,52,7,0x00,29,0,0,0,0,0,0,0,0,0,0,0,0,4,60,2,61,1,62,2,63,3";
    char expcase8[] = {
        0x4b,0x57,0x58,0x00,52,7,0x00,29,0,0,0,0,0,0,0,0,0,0,0,0,4,60,2,61,1,62,2,63,3};

    _bytes(buf,testcase8);
    assert(!memcmp(buf,expcase8,sizeof(expcase8)));
}

void test_basic_recv_and_send() {
    ServerSocket SERVER;
    bool start = false;
    int  choice = STRING;

    while(1) {
        if(start) {
            char buf[BUF_LEN] = {0};
            char recvBuf[BUF_LEN] = {0};
            int  recvLen = 0;

        	SERVER.Recv(recvBuf,&recvLen);
            if( !strcmp(recvBuf,"switch") ) {
                choice = 1 - choice;
                sprintf_s( buf, "now is %s\n", (choice==HEX)?"HEX":"STRING");
            } else if( choice==STRING ) {
                sprintf_s( buf, "server got : %s\n", recvBuf );
            } else {
                sprintf_s( buf, "server got : %s\n", recvBuf );
            }

            SERVER.Send(buf,strlen(buf));
        } else {
        	SERVER.Start();
            printf("server started\n");
            start = true;
        }
    }

	SERVER.Stop();
}

void test_peng_effect() {
    ServerSocket SERVER;
    bool start = false;
    int  choice = STRING;

    while(1) {
        if(start) {
            char input = -1;
            printf("Enter 'c' to resend, 'q' to quit:\n");
            scanf("%c",&input);

            if(input=='q') {
                break;;
            } else {
                INT8U msgInNetwork[] = {
                    'K','W','X',           //KWX
                    0x00,76,               //request code(下发其他玩家反应)
                    7,                     //package level
                    0x00,27,               //package size
                    0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)
                
                    3,
                    60,1,                  //seatId
                    66,1,                  //reaction
                    67,3,                  //card kind
                };
                
                SERVER.Send((char *)msgInNetwork,sizeof(msgInNetwork));
            }

        } else {
        	SERVER.Start();
            printf("server started\n");
            start = true;
        }
    }

	SERVER.Stop();
}

void test_basic() {
    test_basic_recv_and_send();
    test_peng_effect();
}

/****************************************************
    发送消息实现
    消息格式 : lineNo:PACKAGE_INFO:4B,57,58,10,01,02,03,04,05,06,07,08,09,0a,0b,00,2b,00,36,00,00,00,00,00,00,00,00,00,00,00
****************************************************/
#define WORKING_PATH "D:\\kwx\\kwx\\Classes\\tester\\network\\DATA\\"

#include <stdio.h>

char MONITOR_FILE[128] = {0};
char SEND_DATA_FILE[128] = {0};

void SetFile(char *name) {
    memset(MONITOR_FILE,0,128);
    strcpy(MONITOR_FILE,WORKING_PATH);
    strcat(MONITOR_FILE,name);
    strcat(MONITOR_FILE,"_monitor.txt");
    //printf("monitor file is %s\n",MONITOR_FILE);

    memset(SEND_DATA_FILE,0,128);
    strcpy(SEND_DATA_FILE,WORKING_PATH);
    strcat(SEND_DATA_FILE,name);
    strcat(SEND_DATA_FILE,"_send.txt");
    //printf("send data is %s\n",SEND_DATA_FILE);
}

void show(char *buf,int len) {
    for (int i=0;i<len;i++) {
        printf("0x%02x ",(unsigned char)buf[i]);
        if((i+1)%8==0) {
            printf("\n");
        }
    }
    
    printf("\n");
}

int GetLine(char *buf,int line=1) {
    FILE * fsend = fopen(SEND_DATA_FILE,"r");
    assert(fsend!=NULL);

    int i = 1;
    while(i<=line) {
        memset(buf,0,BUF_LEN);
        if(fgets(buf,BUF_LEN,fsend)==NULL) {
            return -1;
        } else {
            if(!strncmp(buf,"//",2)) {
                continue;
            } else {
                i++;
            }
        }
    }

    return 0;
}

int ExtractHeader(char *buf) {
    char *p = strrchr(buf,':');
    int remain = strlen(buf)-(p+1-buf);
    memcpy(buf,p+1,remain);
    buf[remain] = 0;

    return remain;
}

int GetSendData(char *buf,int line=1) {
    FILE * fsend = fopen(SEND_DATA_FILE,"r");
    assert(fsend!=NULL);

    char str[BUF_LEN] = {0};
    if(GetLine(str,line)==-1) {
        printf("read line %d from %s fail\n",line,SEND_DATA_FILE);
        return -1;
    } else {
        ExtractHeader(str);
        return _bytes(buf,str);
    }
}

void test_read_send_data() {
    while(1) {
        int choice = 0;
        printf("choose a package:");
        scanf("%d",&choice);
        if(choice==0) {
            break;
        }


        char sendBuf[BUF_LEN] = {0};
        int  len = GetSendData(sendBuf,choice);
        show(sendBuf,len);
    }
}

/*************************************************************
    接收到一个包后，可以选择发送一个send.txt中预设的消息
*************************************************************/
void test_game_server() {
    FILE * fmonitor = fopen(WORKING_PATH"monitor.txt","w+");
    assert(fmonitor!=NULL);

    ServerSocket SERVER;
    bool start = false;
    int  choice = STRING;

    while(1) {
        if(start) {
            char sendBuf[BUF_LEN] = {0};
            int  sendLen = 0;
            char recvBuf[BUF_LEN] = {0};
            int  recvLen = 0;

        	SERVER.Recv(recvBuf,&recvLen);

            int line = 0;
            printf("choose a package:");
            scanf("%d",&line);
            if(line==0) {
                break;
            }

            sendLen = GetSendData(sendBuf,line);
            SERVER.Send(sendBuf,sendLen);
        } else {
        	SERVER.Start();
            printf("server started\n");
            start = true;
        }
    }

	SERVER.Stop();
}

/*************************************************************
    接收到一个指定类型的request后，自动发送预设的消息
*************************************************************/
#include "./../../protocol/KwxMsgBasic.h"
void handle_requests(ServerSocket SERVER,char *recvBuf,int len);
FILE *fmonitor = NULL;

static void SaveLog(FILE *fp,char *dir,const char *buf,int len) {
    char str[BUF_LEN] = {0};
    int strLen = 0;

    strcat(str,dir);    strLen += strlen(dir);
    strcat(str,":");    strLen += 1;
    for(int i=0;i<len;i++) {
        sprintf(str+strLen,"0x%02x ",(unsigned char)buf[i]);
        strLen += 5;
        
        if((i+1)%16==0) {
            sprintf(str+strLen,"\r     ",(unsigned char)buf[i]);
            strLen += strlen("\r     ");
        }
    }
    strcat(str+strLen,"\r\n");
    strLen += 2;

    fwrite(str,strLen,1,fp);
    fflush(fp);
}

#ifndef DELAY
#define DELAY 500
#endif
static void SendLine(ServerSocket SERVER,int lineNo) {
    char sendBuf[2048] = {0};
    int  sendLen = 0;
    
    Sleep(DELAY);
    sendLen = GetSendData(sendBuf,lineNo);
    SERVER.Send(sendBuf,sendLen);
    SaveLog(fmonitor,"SEND",sendBuf,sendLen);
}

void round1_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    static int handout = 0;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        char sendBuf[] = {
            'K','W','X',           //KWX
            0x00,44,               //request code
            7,                     //package level
            0x00,84,               //package size
            0,0,0,0,0,0,0,0,0,0,0,0, //reserved(12)

            9,
            131,0,4,0,1,2,3,         //roomPath:0x00010203
            132,0,4,4,5,6,7,         //roomId:  0x04050607
            133,0,4,8,9,10,11,       //tableId: 0x08090a0b
            60,1,                    //site:    1
            134,0,4,0,0,0,1,         //底分 base score
            135,0,3,1,1,1,           //player status
            136,0,12,                //player score
                0,0,0,100,
                0,0,0,200,
                0,0,0,300,
            137,0,3,0,1,2,           //player name, UTF-16
            138,0,3,0,1,2            //player image, UTF-16
        };
        
        SERVER.Send(sendBuf,sizeof(sendBuf));
        SaveLog(fmonitor,"SEND",sendBuf,sizeof(sendBuf));
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==0) {
        handout++;
    
        SendLine(SERVER,5);
        SendLine(SERVER,6);
        SendLine(SERVER,7);
        SendLine(SERVER,8);
        SendLine(SERVER,9);
        SendLine(SERVER,10);
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==1) {
        handout++;
    
        SendLine(SERVER,11);
        SendLine(SERVER,12);
        SendLine(SERVER,13);
        SendLine(SERVER,14);
        SendLine(SERVER,15);
        SendLine(SERVER,16);
    } else if(recvBuf[16]==REQ_GAME_SEND_ACTION && handout==2) {
        handout++;
    
        SendLine(SERVER,17);
        SendLine(SERVER,18);
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==3) {
        handout++;
    
        SendLine(SERVER,19);
        SendLine(SERVER,20);
        SendLine(SERVER,21);
        SendLine(SERVER,22);
        SendLine(SERVER,23);
        SendLine(SERVER,24);
        SendLine(SERVER,25);
    } else if(recvBuf[16]==REQ_GAME_SEND_ACTION && handout==4) {
        handout++;
    
        SendLine(SERVER,26);
        SendLine(SERVER,27);
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==5) {
        handout++;
    
        SendLine(SERVER,28);
        SendLine(SERVER,29);
        SendLine(SERVER,30);
        SendLine(SERVER,31);
        SendLine(SERVER,32);
        SendLine(SERVER,33);
    } else if(recvBuf[16]==REQ_GAME_SEND_ACTION && handout==6) {
        handout++;
    
        SendLine(SERVER,34);
        SendLine(SERVER,35);
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==7) {
        handout++;
    
        SendLine(SERVER,36);
        SendLine(SERVER,37);
        SendLine(SERVER,38);
        SendLine(SERVER,39);
    }
}

void round2_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_GAME_SEND_START) {
        handout++;
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
    } else if(recvBuf[16]==REQ_GAME_SEND_ACTION && handout==1) {
        SendLine(SERVER,5);
        SendLine(SERVER,6);
    }
}

void round3_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_GAME_SEND_START) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
    }
}

typedef void (*REQUEST_HANDLER)(ServerSocket SERVER,char *recvBuf,int len);
REQUEST_HANDLER gHandle = NULL;

void test_smart_game_round_x() {
    fmonitor = fopen(MONITOR_FILE,"w");
    assert(fmonitor!=NULL);

    ServerSocket SERVER;
    bool start = false;
    int  choice = STRING;

    while(1) {
        if(start) {
            char recvBuf[BUF_LEN] = {0};
            int  recvLen = 0;

        	SERVER.Recv(recvBuf,&recvLen);
            SaveLog(fmonitor,"RECV",recvBuf,recvLen);
            gHandle(SERVER,recvBuf,recvLen);
        } else {
        	SERVER.Start();
            printf("server started\n");
            start = true;
        }
    }

	SERVER.Stop();
    fclose(fmonitor);
}

//#define NETWORK_TRANSFER_ONLY
void test_server_console() {
    test_string_to_hex();

#ifdef NETWORK_TRANSFER_ONLY
    test_basic();

    SetFile("");
    test_read_send_data();
#else
    #if 0
    test_game_server()();

    SetFile("Round1");
    gHandle = round1_handle_requests;

    /* 本机非庄家 */
    SetFile("Round3");
    gHandle = round3_handle_requests;

    #endif

    SetFile("Round2");
    gHandle = round2_handle_requests;

    test_smart_game_round_x();
#endif
}

