
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
    
****************************************************/
#define WORKING_PATH "D:\\kwx\\kwx\\Classes\\tester\\network\\DATA\\"

#include <stdio.h>

void show(char *buf,int len) {
    for (int i=0;i<len;i++) {
        printf("0x%02x ",buf[i]);
        if((i+1)%8==0) {
            printf("\n");
        }
    }
    
    printf("\n");
}

/* the string format should be 4B,57,58,10,01,02,03,04,05,06,07,08,09,0a,0b,00,2b,00,36,00,00,00,00,00,00,00,00,00,00,00 */
int GetSendData(char *buf) {
    FILE * fsend = fopen(WORKING_PATH"send.txt","r");
    assert(fsend!=NULL);

    fgets(buf,512,fsend);
    return StringToHex(buf);
}

int PressAnyKeyToSend(char *sendBuf) {
    char c;
    printf("\nSEND:");
    scanf("%c",&c);
    if(c=='q') {
        return -1;
    }
        
    int  len = GetSendData(sendBuf);
    show(sendBuf,len);

    return 0;
}

void test_server_console() {
#if 0
    test_basic();
#endif
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

            sendLen = GetSendData(sendBuf);
            show(sendBuf,sendLen);
            SERVER.Send(sendBuf,sendLen);
        } else {
        	SERVER.Start();
            printf("server started\n");
            start = true;
        }
    }

	SERVER.Stop();
}

