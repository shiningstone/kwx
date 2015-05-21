
#ifndef _DS_MSG_PARSER_
#define _DS_MSG_PARSER_

#include "./../utils/UtilBasic.h"
#include "./../game/GameType.h"
#include "./../utils/LogManager.h"
#include "./../game/CardCollection.h"

#include "RequestStructs.h"

class DsMsg;

class DsMsgParser {
public:
    static int _load(INT8U *status,INT8U &num,const DsMsg &msg,int itemIdx);
    static int _load(INT32U *fan, const bool *status,const DsMsg &msg,int itemIdx);
    static int _load(INT32U *score, INT8U &num, const DsMsg &msg,int itemIdx);
    static int _load(Card_t cards[3][18],INT8U num[3],const DsMsg &msg,int itemIdx);
    static int _load(Card_t *cards,INT8U &num,const DsMsg &msg,int itemIdx);
    static int _load(CardNode_t cards[18],INT8U &num,const DsMsg &msg,int itemIdx);
    static int _load(TingInfo_t    &info,const INT8U *inMsg);
    static int _load(MingInfo_t &info,const DsMsg &msg,int itemIdx);
    static int _load(Reminds_t &remind,const DsMsg &msg,int itemIdx);

    static int _unload(TingInfo_t    &ting);
    static int _unload(MingInfo_t &ming);
    static int _unload(Reminds_t &remind);
};

#endif
