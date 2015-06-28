
#ifndef _DS_MSG_PARSER_
#define _DS_MSG_PARSER_

#include "./../utils/UtilBasic.h"
#include "./../utils/LogManager.h"
#include "./../game/GameType.h"
#include "./../game/CardCollection.h"
#include "./../network/KwxEnv.h"

#include "RequestStructs.h"

class DsMsg;

class DsMsgParser {
public:
    static int _load(INT8U *values,const DsMsg &msg,int itemIdx);
    static int _load(INT32U *values, const DsMsg &msg,int itemIdx);
    static int _load(Card_t *cards,INT8U &num,const DsMsg &msg,int itemIdx);
    static int _load(CardNode_t cards[18],INT8U &num,const DsMsg &msg,int itemIdx);
    static int _load(CardNode_t cards[3][18],INT8U num[3],const DsMsg &msg,int itemIdx);
    static int _load_ting_info(TingInfo_t &info,const INT8U *inMsg);
    static int _load_ting_remind(TingInfo_t &ting,const INT8U *inMsg);
    static int _load(MingInfo_t &info,const DsMsg &msg,int itemIdx);
    static int _load(Reminds_t &remind,const DsMsg &msg,int itemIdx);
    static int _load(bool isMing[3],INT8U tingNum[3],Card_t tingCards[3][18],const DsMsg &msg,int itemIdx);

    static int _unload(TingInfo_t    &ting);
    static int _unload(MingInfo_t &ming);
    static int _unload(Reminds_t &remind);

    static int _load_seat_info(SeatInfo_t &info,const DsMsg &msg,int itemIdx);
    static int _load_friends_info(vector<Friend_Info> &info,const DsMsg &msg,int itemIdx);
    static int _load_others_info(vector<OtherPlayers_Info> &info,const DsMsg &msg,int itemIdx);
};

#endif
