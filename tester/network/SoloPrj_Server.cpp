
#include <assert.h>
#include <stdio.h>

#include "./../../utils/UtilBasic.h"
#include "./../../network/CSockets.h"
#include "./../../utils/DebugCtrl.h"

#define DEF_SOCKET_PORT  60905

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
        	SERVER.Start(DBG_LAN_SERVER,DEF_SOCKET_PORT);
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
        	SERVER.Start(DBG_LAN_SERVER,DEF_SOCKET_PORT);
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
#define WORKING_PATH "D:\\kwx\\svn\\trunk\\kwx\\Classes\\tester\\network\\DATA\\"

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

int ExtractLineNumber(char *buf) {
    return atoi(buf);
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
            } else if(line==ExtractLineNumber(buf)) {
                return 0;
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
        	SERVER.Start(DBG_LAN_SERVER,DEF_SOCKET_PORT);
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
    printf("\n%d---->",lineNo);
    SERVER.Send(sendBuf,sendLen);
    SaveLog(fmonitor,"SEND",sendBuf,sendLen);
}

static void SendLines(ServerSocket SERVER,int start,int end) {
    for(int i=start;i<=end;i++) {
        SendLine(SERVER,i);
    }
}

void round1_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
        SendLine(SERVER,5);
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        SendLine(SERVER,6);
        SendLine(SERVER,7);
    } else if(handout==0) {
        handout++;
    
        SendLine(SERVER,8);
        SendLine(SERVER,9);
    } else if(handout==1) {
        handout++;

        for(int i=10;i<=15;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==2) {
        handout++;

        for(int i=16;i<=21;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==3) {
        handout++;

        for(int i=22;i<=23;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==4) {
        handout++;
    
        SendLine(SERVER,24);
        SendLine(SERVER,25);
    } else if(handout==5) {
        handout++;

        for(int i=26;i<=32;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==6) {
        handout++;
    
        SendLine(SERVER,33);
        SendLine(SERVER,34);
    } else if(handout==7) {
        handout++;

        for(int i=35;i<=40;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==8) {
        handout++;
    
        SendLine(SERVER,41);
        SendLine(SERVER,42);
    } else if(handout==9) {
        handout++;

        for(int i=43;i<=47;i++) {
            SendLine(SERVER,i);
        }    
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

void no_gang_remind_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        SendLine(SERVER,1);
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {

        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
        SendLine(SERVER,5);
        SendLine(SERVER,6);
        SendLine(SERVER,7);
        SendLine(SERVER,8);
        SendLine(SERVER,9);
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==0) {
        handout++;
        
        SendLine(SERVER,10);
        SendLine(SERVER,11);
        SendLine(SERVER,12);
        SendLine(SERVER,13);
        SendLine(SERVER,14);
        SendLine(SERVER,15);
        SendLine(SERVER,16);
        SendLine(SERVER,17);
        SendLine(SERVER,18);
        SendLine(SERVER,19);
    }
}


void Exception_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
        SendLine(SERVER,5);
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        SendLine(SERVER,6);
        SendLine(SERVER,7);
        SendLine(SERVER,8);
        SendLine(SERVER,9);
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==0) {
        handout++;

        for(int i=10;i<22;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==1) {
        handout++;
        
        for(int i=22;i<30;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==2) {
        handout++;
        
        for(int i=30;i<42;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==3) {
        handout++;
        
        for(int i=42;i<54;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==4) {
        handout++;
        
        for(int i=54;i<61;i++) {
            SendLine(SERVER,i);
        }
    }
}

void E15051202_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        for(int i=1;i<6;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        for(int i=6;i<9;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_ACTION) {
        SendLine(SERVER,10);
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==0) {
        handout++;
    
        SendLine(SERVER,11);
        SendLine(SERVER,12);
    }
}

void E15051701_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        for(int i=1;i<6;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        for(int i=6;i<9;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_ACTION && handout==0) {
        handout++;
    
        for(int i=9;i<13;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_ACTION && handout==1) {
        handout++;
    
        for(int i=13;i<17;i++) {
            SendLine(SERVER,i);
        }
    }
}

void E15051704_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        for(int i=1;i<6;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        for(int i=6;i<9;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_ACTION && handout==0) {
        handout++;
    
        for(int i=9;i<11;i++) {
            SendLine(SERVER,i);
        }
    } else if(recvBuf[16]==REQ_GAME_SEND_SHOWCARD && handout==1) {
        handout++;
    
        for(int i=11;i<34;i++) {
            SendLine(SERVER,i);
        }
    }
}

void E15051801_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(handout==0) {
        handout++;
        for(int i=1;i<6;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==1) {
        handout++;
        for(int i=6;i<10;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==2) {
        handout++;
        for(int i=10;i<13;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==3) {
        handout++;
        for(int i=13;i<16;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==4) {
        handout++;
        for(int i=16;i<19;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==5) {
        handout++;
        for(int i=19;i<21;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==6) {
        handout++;
        for(int i=21;i<32;i++) {
            SendLine(SERVER,i);
        }
    }
}

void E15051901_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(handout==0) {
        handout++;
        for(int i=1;i<6;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==1) {
        handout++;
        for(int i=6;i<10;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==2) {
        handout++;
        for(int i=10;i<13;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==3) {
        handout++;
        for(int i=13;i<16;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==4) {
        handout++;
        for(int i=16;i<19;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==5) {
        handout++;
        for(int i=19;i<21;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==6) {
        handout++;
        for(int i=21;i<32;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==7) {
        handout++;
        for(int i=32;i<34;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==8) {
        handout++;
        for(int i=34;i<36;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==9) {
        handout++;
        for(int i=36;i<54;i++) {
            SendLine(SERVER,i);
        }
    }
}

void E15051902_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    static int handout = 0;

    if(handout==0) {
        handout++;
        for(int i=1;i<6;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==1) {
        handout++;
        for(int i=6;i<10;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==2) {
        handout++;
        for(int i=10;i<13;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==3) {
        handout++;
        for(int i=13;i<16;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==4) {
        handout++;
        for(int i=16;i<19;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==5) {
        handout++;
        for(int i=19;i<21;i++) {
            SendLine(SERVER,i);
        }
    }
}

void round5_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 1;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
        SendLine(SERVER,5);
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        SendLine(SERVER,6);
        SendLine(SERVER,7);
    } else if(handout==1) {
        handout++;
        for(int i=8;i<10;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==2) {
        handout++;
        for(int i=10;i<14;i++) {
            SendLine(SERVER,i);
        }
    } else if(handout==3) {
        SendLine(SERVER,14);
        SendLine(SERVER,15);
        SendLine(SERVER,16);
	}
}

void E0605_gang_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 1;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
        SendLine(SERVER,5);
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        SendLine(SERVER,6);
        SendLine(SERVER,7);
    } else if(handout==1) {
        handout++;
        
        SendLine(SERVER,8);
        SendLine(SERVER,9);
        SendLine(SERVER,10);
    } else if(handout==2) {
        handout++;
        
        SendLine(SERVER,11);
        SendLine(SERVER,12);
        SendLine(SERVER,13);
    } else if(handout==3) {
        handout++;
        
        SendLine(SERVER,14);
        SendLine(SERVER,15);
        SendLine(SERVER,16);
    } else if(handout==4) {
        handout++;
        
        SendLine(SERVER,17);
        SendLine(SERVER,18);
        SendLine(SERVER,19);
        SendLine(SERVER,20);
    } 
}

void round6_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 1;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
        SendLine(SERVER,5);
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        SendLine(SERVER,6);
        SendLine(SERVER,7);
    } else if(handout==1) {
        handout++;
        
        SendLine(SERVER,8);
        SendLine(SERVER,9);
    } else if(handout==2) {
        handout++;
        
        SendLine(SERVER,10);
        SendLine(SERVER,11);
        SendLine(SERVER,12);
    } else if(handout==3) {
        handout++;
        
        SendLine(SERVER,13);
    }
}

void right_actions_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 1;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
        SendLine(SERVER,5);
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        SendLine(SERVER,6);
        SendLine(SERVER,7);
    } else if(handout==1) {
        handout++;
        SendLine(SERVER,8);
        SendLine(SERVER,9);
    } else if(handout==2) {
        handout++;
        SendLine(SERVER,10);
        SendLine(SERVER,11);
        SendLine(SERVER,12);
    } else if(handout==3) {
        handout++;
        for(int i=13;i<=23;i++) {
            SendLine(SERVER,i);
        }
    } 
}

void reconnect_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 1;

    if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
        SendLine(SERVER,5);
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        SendLine(SERVER,6);
        SendLine(SERVER,7);
    } else if(handout==1) {
        handout++;
        SendLine(SERVER,8);
    } else if(handout==2) {
        handout++;
        SendLine(SERVER,9);
    } 
}

void lde_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_LOGIN) {
        handout++;
        SendLine(SERVER,1);
    } else if(recvBuf[16]==REQ_GAME_SEND_RECONNECT) {
        handout++;
        SendLine(SERVER,2);
    } else if(recvBuf[16]==REQ_DAILY_LOGIN) {
        handout++;
        SendLine(SERVER,1);
    } else if(recvBuf[16]==REQ_GET_DAILY_PRIZE) {
        handout++;
        SendLine(SERVER,2);
    }
}

void le_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_LOGIN) {
        return;
    } else if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
        SendLine(SERVER,5);
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        handout++;
        SendLine(SERVER,6);
        SendLine(SERVER,7);
    } else if(handout==1) {
        handout++;
        SendLine(SERVER,8);
        SendLine(SERVER,9);
    } else if(handout==2) {
        handout++;
        SendLine(SERVER,10);
        SendLine(SERVER,11);
        SendLine(SERVER,12);
    } else if(handout==3) {
        handout++;
        for(int i=13;i<=23;i++) {
            SendLine(SERVER,i);
        }
    } 
}

void lr_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_LOGIN) {
        return;
    } else if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
        SendLine(SERVER,5);
    } else if(recvBuf[16]==REQ_GAME_SEND_START) {
        handout++;
        SendLine(SERVER,6);
        SendLine(SERVER,7);
    } else if(handout==1) {
        handout++;
        SendLine(SERVER,8);
        SendLine(SERVER,9);
    } else if(handout==2) {
        handout++;
        SendLine(SERVER,10);
        SendLine(SERVER,11);
        SendLine(SERVER,12);
    } else if(handout==3) {
        handout++;
        for(int i=13;i<=23;i++) {
            SendLine(SERVER,i);
        }
    } 
}

void temp_handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[BUF_LEN] = {0};
    int  sendLen = 0;

    static int handout = 0;

    if(recvBuf[16]==REQ_LOGIN) {
        return;
    } else if(recvBuf[16]==REQ_GAME_SEND_ENTER) {
        SendLine(SERVER,1);
        SendLine(SERVER,2);
        SendLine(SERVER,3);
        SendLine(SERVER,4);
        SendLine(SERVER,5);
    } else if(handout==0 && recvBuf[16]==REQ_GAME_SEND_START) {
        handout++;
        SendLine(SERVER,6);
        SendLine(SERVER,134);
        SendLine(SERVER,135);
    } else if(handout==1) {
        handout++;
        SendLines(SERVER,136,140);
    } else if(handout==2) {
        handout++;
        SendLines(SERVER,141,142);
    } else if(handout==3) {
        handout++;
        SendLines(SERVER,143,152);
    } else if(handout==4) {
        handout++;
        SendLines(SERVER,153,159);
    } else if(handout==5) {
        handout++;
        SendLines(SERVER,162,166);
    } else if(handout==6) {
        handout++;
        SendLines(SERVER,167,170);
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
        	SERVER.Start(DBG_LAN_SERVER,DEF_SOCKET_PORT);
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

    /* 本机非庄家 */
    SetFile("Round3");
    gHandle = round3_handle_requests;

    #endif
    /*没有杠提示*/
    SetFile("E15051202");
    gHandle = E15051202_handle_requests;

    SetFile("E15051701");
    gHandle = E15051701_handle_requests;

    SetFile("E15051704");
    gHandle = E15051704_handle_requests;

    SetFile("E15051801");
    gHandle = E15051801_handle_requests;

    SetFile("E15051901");
    gHandle = E15051901_handle_requests;

    SetFile("E15051902");
    gHandle = E15051902_handle_requests;

    SetFile("E0605_gang");
    gHandle = E0605_gang_handle_requests;

    SetFile("Round2");/*胡牌*/
    gHandle = round2_handle_requests;

    SetFile("Round5");/*竞争动作*/
    gHandle = round5_handle_requests;

    SetFile("reconnect");
    gHandle = reconnect_handle_requests;

    /*******************************************/
    /* login test 需要取消IGNORE_LOGIN_REQUEST */
    /*******************************************/
    SetFile("login_enterroom");
    gHandle = le_handle_requests;

    SetFile("login_recoonect");
    gHandle = lr_handle_requests;

    SetFile("right_actions");/*明牌右碰杠胡*/
    gHandle = right_actions_handle_requests;

    SetFile("Round6");/*明牌*/
    gHandle = round6_handle_requests;

    SetFile("login_dailylogin_enterroom");
    gHandle = lde_handle_requests;

    SetFile("Round1");/*基本操作*/
    gHandle = round1_handle_requests;

    SetFile("temp");
    gHandle = temp_handle_requests;

    test_smart_game_round_x();
#endif
}

