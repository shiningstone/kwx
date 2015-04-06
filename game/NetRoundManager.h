
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
#include "OutCardList.h"

#include "CardHolder.h"

class NetRole;
class RoundManager;
class KwxMessenger;

class NetRoundManager : public RoundManager {
    friend class NetRaceLayer;
    friend class Ai;
public:
    static NetRoundManager *getInstance();
    static void  destroyInstance();
    Ai             *_ai;

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
    void _DiRecv(DistCardNotif *info);
    void _DiRecv(DistCardInfo *info);
    void _DiRecv(ShowCardResponse *info);
    void _DiRecv(ShowCardNotif *info);
/* networks end   */
/******************/

    NetRoundManager(NetRaceLayer *uiManager);
    ~NetRoundManager();

    virtual void RecvPeng(PlayerDir_t dir);
    virtual void RecvHu(PlayerDir_t dir);
    virtual void RecvGang(PlayerDir_t dir);
    virtual void RecvQi();
    virtual void RecvHandout(int idx,Vec2 touch,int mode);
    /*internal action*/
    virtual void RecvKouCancel();
    virtual void RecvKouConfirm();
    virtual void RecvMingCancel();
	virtual void RecvMing();

    virtual void WaitForMyAction();
    virtual void WaitForMyChoose();
    virtual void WaitForOthersAction(PlayerDir_t dir);
    virtual void WaitForOthersChoose();
    virtual void WaitForResponse(PlayerDir_t dir);
    virtual void DistributeTo(PlayerDir_t dir,Card_t card);
    virtual void ActionAfterGang(PlayerDir_t dir);

    virtual void Init();
    virtual void InitPlayers();
    virtual void LoadPlayerInfo();

    /* main process */
    virtual void CreateRace(Scene *scene);
    virtual void StartGame();
private:
    static NetRoundManager *_instance;
};

#endif
