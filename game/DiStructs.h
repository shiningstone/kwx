
#ifndef __DS_INSTRUCTION_STRUCTS_H__
#define __DS_INSTRUCTION_STRUCTS_H__

/*****************************************
    the following structs are used as messages between NetRoundManager & DsInstruction
*****************************************/
#include "./../utils/BasicType.h"
#include "./../protocol/KwxMsgBasic.h"

typedef struct _EventMsg_t {
    RequestId_t  request;
    void         *data;
}EventMsg_t;

typedef struct _DiScoreInfo_t {
    PlayerDir_t  dir;
    int          score;
}DiScoreInfo_t;

#endif

