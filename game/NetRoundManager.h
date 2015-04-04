
#ifndef __NET_ROUND_MANAGER__
#define __NET_ROUND_MANAGER__

#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

#include "RaceType.h"
#include "OutCardList.h"

#include "CardHolder.h"
#include "./../utils/LogManager.h"

class NetRole;
class RoundManager;

class NetRoundManager : public RoundManager {
    friend class NetRaceLayer;
    friend class Ai;
public:
    static NetRoundManager *getInstance();
    static void  destroyInstance();
    Ai             *_ai;

    NetRoundManager(NetRaceLayer *uiManager);
    ~NetRoundManager();

    void RecvPeng(PlayerDir_t dir);
    void RecvHu(PlayerDir_t dir);
    void RecvGang(PlayerDir_t dir);
    void RecvQi();
    void RecvHandout(int idx,Vec2 touch,int mode);
    /*internal action*/
    void RecvKouCancel();
    void RecvKouConfirm();
    void RecvMingCancel();
	void RecvMing();

    void WaitForFirstAction(PlayerDir_t zhuang);
    void WaitForMyAction();
    void WaitForMyChoose();
    void WaitForOthersAction(PlayerDir_t dir);
    void WaitForOthersChoose();
    void WaitForResponse(PlayerDir_t dir);
    void DistributeTo(PlayerDir_t dir);
    void ActionAfterGang(PlayerDir_t dir);

    void Init();
    void InitPlayers();
    void LoadPlayerInfo();

    bool GetReadyStatus(PlayerDir_t dir);
    bool WaitUntilAllReady();

    /* main process */
    virtual void CreateRace(Scene *scene);
    virtual void StartGame();
private:
    static NetRoundManager *_instance;
};

#endif
