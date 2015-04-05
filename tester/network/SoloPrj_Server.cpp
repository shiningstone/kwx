
#include <assert.h>
#include <stdio.h>

#include "./../../utils/UtilBasic.h"
#include "./../../network/CSockets.h"

typedef enum {
    STRING,
    HEX,
};

int ctoi(char c) {
    if( c>='0' && c <= '9' ) {
        return c-'0';
    } else if( c>='a' && c<='f' ) {
        return 10+c-'a';
    } else if( c>='A' &&c<='F' ) {
        return 10+c-'A';
    } else {
        return -1;
    }
}

int StringToHex(char *buf) {
    char Hex[128] = {0};

    int i=0;
    for(i=0;3*i<strlen(buf);i++) {
        Hex[i] = ctoi(*(buf+3*i))*16 + ctoi(*(buf+3*i+1));
    }

    memset(buf,0,128);
    memcpy(buf,Hex,i);

    return i;
}

void test_basic_recv_and_send() {
    ServerSocket SERVER;
    bool start = false;
    int  choice = STRING;

    while(1) {
        if(start) {
            char buf[128] = {0};
            char recvBuf[128] = {0};
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
#define SEND_DATA    "D:\\kwx\\kwx\\Classes\\tester\\network\\DATA\\send.txt"

#include <stdio.h>

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
    FILE * fsend = fopen(SEND_DATA,"r");
    assert(fsend!=NULL);

    int i = 1;
    while(i<=line) {
        memset(buf,0,512);
        if(fgets(buf,512,fsend)==NULL) {
            return -1;
        } else {
            i++;
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
    FILE * fsend = fopen(SEND_DATA,"r");
    assert(fsend!=NULL);

    if(GetLine(buf,line)==-1) {
        printf("read line %d from %s fail\n",line,SEND_DATA);
        return -1;
    } else {
        ExtractHeader(buf);
        return StringToHex(buf);
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


        char sendBuf[512] = {0};
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
            char sendBuf[512] = {0};
            int  sendLen = 0;
            char recvBuf[512] = {0};
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
void test_smart_game_server() {
    FILE * fmonitor = fopen(WORKING_PATH"monitor.txt","w+");
    assert(fmonitor!=NULL);

    ServerSocket SERVER;
    bool start = false;
    int  choice = STRING;

    while(1) {
        if(start) {
            char recvBuf[512] = {0};
            int  recvLen = 0;

        	SERVER.Recv(recvBuf,&recvLen);
            handle_requests(SERVER,recvBuf,recvLen);
        } else {
        	SERVER.Start();
            printf("server started\n");
            start = true;
        }
    }

	SERVER.Stop();
}

void handle_requests(ServerSocket SERVER,char *recvBuf,int len) {
    char sendBuf[512] = {0};
    int  sendLen = 0;

    if(recvBuf[16]==REQ_GAME_SEND_START) {
        sendLen = GetSendData(sendBuf,1);
        SERVER.Send(sendBuf,sendLen);
    
        sendLen = GetSendData(sendBuf,2);
        SERVER.Send(sendBuf,sendLen);
        
        sendLen = GetSendData(sendBuf,3);
        SERVER.Send(sendBuf,sendLen);
    }
}

void test_server_console() {
#if 0
    test_basic();
    test_read_send_data();
#endif

    #if 0
    test_game_server()();
    #else
    test_smart_game_server();
    #endif
}

