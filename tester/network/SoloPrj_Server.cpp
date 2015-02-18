
#include <assert.h>
#include <stdio.h>
#include "./../../network/CSockets.h"

typedef enum {
    STRING,
    HEX,
};

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

void test_server_console() {
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
