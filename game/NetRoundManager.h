
#ifndef __NET_ROUND_MANAGER__
#define __NET_ROUND_MANAGER__

#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

#include "./../utils/LogManager.h"
#include "./../utils/MsgQueue.h"
#include "./../protocol/DsInstruction.h"
#include "./../protocol/KwxMsgLogin.h"

class Player;
class RoundManager;
class KwxMessenger;

class NetRoundManager : public RoundManager, public MsgHandle {
    friend class RaceLayer;
    friend class StrategyPlayer;
public:
    static RoundManager *getInstance();
    static void  destroyInstance();

    std::mutex          _uiLocker;

/******************/
/* networks start */
    virtual void HandleMsg(void * aMsg);
    void RecordError(void *aError);
    void HandleError();

    KwxMessenger   *_messenger;

    bool _isAfterMing;

    void _DiRecv(GameStartResponse *info);
    void _DiRecv(GameStartNotif *info);
    void _DiRecv(FirstDistZhuang *info);
    void _DiRecv(FirstDistNonZhuang *info);
    void _DiRecv(DistCardNotif *info);
    void _DiRecv(DistCardInfo *info);
    void _DiRecv(ShowCardResponse *info);
    void _DiRecv(ShowCardNotif *info);
    void _DiRecv(RemindInfo *info);
    void _DiRecv(ActionResponse *info);
    void _DiRecv(ActionNotif *info);
    bool _UpdateWin(HuInfo_t *player);
    void _DiRecv(HuInfoNotif *info);
    void _DiRecv(TuoGuanResponse *info);
    void _DiRecv(TuoGuanNotif *info);
    void _DiRecv(TingInfoResponse *info);

    void _DiRecv(EnterRoomResponse *info);
    void _DiRecv(EnterRoomNotif *info);
    void _DiRecv(CounterNotif *info);
    void _DiRecv(ScoreNotif *info);
    void _DiRecv(ReconnectResponse *info);
    void _DiRecv(ReconnectNotif *info);

    bool ServerWaitForMyAction();
    void ServerDistributeTo(PlayerDir_t dir,Card_t card);
    
    virtual void UpdateCards(PlayerDir_t dir,ARRAY_ACTION action,Card_t actKind = CARD_UNKNOWN);

    /* handout request only can be sent after DAOJISHI received.
       postpone this process to abolish ui-effect-lagging */
    bool _HandoutNotify;
    void _NotifyHandout();

    bool _permited;    /* this flag is used for judging conflicting actions from different players */
    bool _needConfirm;
    bool Wait(RequestId_t req);

    Reminds_t  _serverReminds;
    void _restoreRemindInfo(const Reminds_t &remind);
    void _order_small_to_big(Card_t kinds[],int num);

    void ForceAction();

    typedef struct _MingDecisionBuf_t {
        CardNode_t       cards[PLAYER_NUM][18]; /* this struct is used for buffering the cards received by ming decision */
        int              num[PLAYER_NUM];
    }MingDecisionBuf;
    MingDecisionBuf _mingBuf;

    void SendReconnect();
    void query_ting_info(TingInfo_t *ting);
/* networks end   */
/******************/

    virtual Card_t RecvPeng(PlayerDir_t dir);
    virtual void RecvHu(PlayerDir_t dir);
    virtual void RecvQi();
    virtual void RecvHandout(int idx,Vec2 touch,int mode);
    /*internal action*/
    virtual Card_t RecvGang(PlayerDir_t dir);
    virtual Card_t RecvGangConfirm(PlayerDir_t dir);
    virtual void RecvKouCancel();
    virtual void RecvKouConfirm();
    virtual void RecvMingCancel();
	virtual void RecvMing(bool isFromKou = false);

    virtual void WaitForFirstAction(PlayerDir_t zhuang);
    virtual void WaitForMyAction();
    virtual void WaitForOthersAction(PlayerDir_t dir);
    virtual void WaitForOthersChoose();
    virtual void WaitForResponse(PlayerDir_t dir);
    virtual void DistributeTo(PlayerDir_t dir,Card_t card);
    virtual void ActionAfterGang(PlayerDir_t dir);
    virtual ActionMask_t judge_action_again(PlayerDir_t dir);

    virtual void InitPlayers();

    /* main process */
    virtual void CreateRace(RaceLayer *uiManager);
    virtual void StartGame();
    virtual void StopGame();
private:
    NetRoundManager(RaceLayer *uiManager);
    ~NetRoundManager();
};

#endif
