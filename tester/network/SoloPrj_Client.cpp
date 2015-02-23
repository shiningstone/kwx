
#include <assert.h>
#include <stdio.h>
#include "./../../network/CSockets.h"

typedef enum {
    HEX,
    STRING,
}MessageType_t;

int ctoi(char c) {
    if( c>='0' && c <= '9' ) {
        return c-'0';
    } else if( (c>='a' && c<='f') || (c>='A' &&c<='F' ) ) {
        return 10+c-'a';
    } else {
        return -1;
    }
}

void StringToHex(char *buf) {
    char Hex[128] = {0};

    int i=0;
    for(i=0;3*i<strlen(buf);i++) {
        Hex[i] = ctoi(*(buf+3*i))*16 + ctoi(*(buf+3*i+1));
    }

    memset(buf,0,128);
    memcpy(buf,Hex,i);
}

void test_client_console() {
    ClientSocket CLIENT;
    bool start = false;
    int  choice = STRING;
    char serverIp[32] = {0};

    printf("please input the IP address of the server:\n");
    scanf("%s",&serverIp);
    printf("please select the message format(0-HEX, 1-String):\n");
    scanf_s("%d",&choice);

    while(1) {
        if(start) {
            char buf[256]         = {0};
            char _clientBuff[256] = {0};
            int  _clientBuffLen    = 0;

            if(choice==HEX) {
                printf("please input a Hex array(use '-' as the seperator):\n");
                scanf_s("%s",buf,256);
                if( !strcmp(buf,"switch") ) {
                    choice = 1-choice;
                } else {
                    StringToHex(buf);            
                }
            } else {
                printf("please input a message:\n");
                scanf_s("%s",buf,256);
                if( !strcmp(buf,"switch") ) {
                    choice = 1-choice;
                }
            }

            CLIENT.Send(buf,strlen(buf));

        	CLIENT.Recv(_clientBuff,&_clientBuffLen);
        } else {
        	CLIENT.Start(serverIp);
            printf("client started\n");
            start = true;
        }
    }

	CLIENT.Stop();
}