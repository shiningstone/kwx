
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

#include "RaceType.h"

class Player;
class RoundManager;
class KwxMessenger;

class NetRoundManager : public RoundManager {
    friend class RaceLayer;
public:
    static NetRoundManager *getInstance();
    static void  destroyInstance();

/******************/
/* networks start */
    KwxMessenger   *_messenger;
    MsgQueue       *_msgQueue;

    void HandleMsg(void * aMsg);
    void ListenToMessenger();
    void RecvMsg(void* val);

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
    void _UpdateWin(HuInfo_t *player);
    void _DiRecv(HuInfoNotif *info);

    void _DiRecv(EnterRoomResponse *info);
    void _DiRecv(EnterRoomNotif *info);
    void _DiRecv(CounterNotif *info);
    void _DiRecv(ScoreNotif *info);

    void ServerWaitForMyAction();
    void ServerDistributeTo(PlayerDir_t dir,Card_t card);
    
    virtual void UpdateCards(PlayerDir_t dir,ARRAY_ACTION action,Card_t actKind = CARD_UNKNOWN);

    bool _HandoutNotify;
    void _NotifyHandout();
/* networks end   */
/******************/

    NetRoundManager(RaceLayer *uiManager);
    ~NetRoundManager();

    virtual Card_t RecvPeng(PlayerDir_t dir);
    virtual void RecvHu(PlayerDir_t dir);
    virtual Card_t RecvGang(PlayerDir_t dir);
    virtual void RecvQi();
    virtual void RecvHandout(int idx,Vec2 touch,int mode);
    /*internal action*/
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

    virtual void InitPlayers();

    /* main process */
    virtual void CreateRace(Scene *scene);
    virtual void StartGame();
private:
    static NetRoundManager *_instance;
};

#endif
