
#ifndef _DS_MSG_PARSER_
#define _DS_MSG_PARSER_

#include "./../utils/UtilBasic.h"
#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

#include "KwxRequestTypes.h"

class KwxDsMsg;
class DsMsgParser {
public:
    static int _load(Card_t *cards,INT8U &num,const KwxDsMsg &msg,int itemIdx);
    static int _load(ActionId_t *actions,INT8U &num,const KwxDsMsg &msg,int itemIdx);
    static int _load(MsgTingInfo_t &info,const INT8U *inMsg);
    static int _load(MingInfo_t &info,const KwxDsMsg &msg,int itemIdx);
    static int _load(Reminds_t &remind,const KwxDsMsg &msg,int itemIdx);
};

#endif