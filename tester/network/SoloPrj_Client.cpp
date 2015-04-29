
#include <assert.h>
#include <stdio.h>
#include "./../../network/CSockets.h"

typedef enum {
    HEX,
    STRING,
	IGNORE_RESPONSE,
}MessageType_t;

void GetArray(char *buf, int line);

static int ctoi(char c) {
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

int ArrayToSend[15][128] = {
	/* enter room*/
    { 0x4b,0x57,0x58,0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x00,0x2c,0x00,0x2d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x84,0x00,0x04,0x01,0x02,0x03,0x04,0x83,0x00,0x04,0x00,0x00,0x00,0x0b },
	/* start game*/
	{ 0x4b,0x57,0x58,0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x00,0x2b,0x00,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x83,0x00,0x04,0x00,0x00,0x00,0x01,0x84,0x00,0x04,0x10,0x01,0x00,0x00,0x85,0x00,0x04,0x00,0x00,0x00,0x01,0x3c,0x01 },
};

void test_client_console() {
#ifdef REQUIRE_SERVER_IP
	char serverIp[32] = {0};
#else
	char serverIp[32] = "10.101.104.116";
#endif

    ClientSocket CLIENT;
    bool start = false;
    int  choice = STRING;

	if(strlen(serverIp)==0) {
		printf("please input the IP address of the server:\n");
		scanf("%s",&serverIp);
	}
    printf("please select the message format(0-HEX, 1-String, 2-String and only send):\n");
    scanf_s("%d",&choice);

    int lineToSend = 0;

    while(1) {
        if(start) {
            char buf[256]         = {0};
            char _clientBuff[256] = {0};
            int  _clientBuffLen    = 0;

            if(choice==HEX) {
                printf("please input a Hex array(use '-' as the seperator):\n");
                printf("or select a constant array with a 'G', range 'G0-Gf':\n");
                scanf_s("%s",buf,256);

                if( !strcmp(buf,"switch") ) {
                    choice = 1-choice;
                } else if( buf[0]=='G' ){
                    lineToSend = ctoi(buf[1]);
                    GetArray(buf,lineToSend);
                } else {
                    StringToHex(buf);            
                }
            } else {
                printf("please input a message:\n");
                scanf_s("%s",buf,256);
                if( !strcmp(buf,"switch")  && choice<2  ) {
                    choice = 1-choice;
                }
            }

            if(choice!=HEX) {
                CLIENT.Send(buf,strlen(buf));
            } else {
                CLIENT.Send(buf,128);
            }

            if(choice!=IGNORE_RESPONSE) {
            	CLIENT.Recv(_clientBuff,&_clientBuffLen);
            }
        } else {
        	CLIENT.Start(serverIp);
            printf("client started\n");
            start = true;
        }
    }

	CLIENT.Stop();
}

void GetArray(char *buf, int line) {
    memset(buf,0,128);
    
    for(int i=0;i<128;i++) {
        buf[i] = ArrayToSend[line][i];
    }
}

