
#ifndef __ROUND_MANAGER__
#define __ROUND_MANAGER__

#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

#include "GameType.h"

#include "./../utils/LogManager.h"
#include "CardCollection.h"

extern void load_test_round(int idx,Card_t *output);

class Player;
class StrategyRm;

class RoundManager {
    friend class RaceLayer;
    friend class StrategyLocalRM;
	friend class StrategyPlayer;
public:
    void update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive);
    void get_ending_gold(int gold[PLAYER_NUM]);
    
    bool player_can_gang();
protected:
    void UpdateGold(int gold[PLAYER_NUM]);
    
private:
    ActionMask_t GetPlayerChoices(PlayerDir_t dir,bool prevMingStatus);
    void _HandleCardNewDistributed(PlayerDir_t dir);
    void _HandleCardFrom(PlayerDir_t dir);
    int TIME_LIMIT;

public:
    /*****************************
        context
    *****************************/
    Card_t LastHandout() const;
    Card_t NewDistribute() const;

    typedef struct _ActCtrl_t {
        bool         handoutAllow;
        
        ActionMask_t choices;
        Card_t       target;
        ActionId_t   decision;
    }ActCtrl_t;

    ActCtrl_t   _actCtrl;
    PlayerDir_t _prevPlayer;
    int         _curPlayer;
    ActionId_t  _lastActionWithGold;
    int         _lastActionSource;
    
    static RoundManager *getInstance();
    static void  destroyInstance();

    RoundManager(RaceLayer *uiManager);
    ~RoundManager();

    virtual Card_t RecvPeng(PlayerDir_t dir);
    virtual void RecvHu(PlayerDir_t dir);
    virtual Card_t RecvGang(PlayerDir_t dir);
    virtual void RecvQi();
    virtual void RecvHandout(int idx,Vec2 touch,int mode);
    /*internal action*/
    virtual void RecvKouCancel();
    virtual void RecvKouConfirm();
    virtual void RecvMingCancel();
	virtual void RecvMing(bool isFromKouStatus=false);

    virtual void WaitForFirstAction(PlayerDir_t zhuang);
    virtual void WaitForMyAction();
    virtual void WaitForMyChoose();
    virtual void WaitForOthersAction(PlayerDir_t dir);
    virtual void WaitForOthersChoose();
    virtual void WaitForResponse(PlayerDir_t dir);
    virtual void WaitForTuoGuanHandle();
    virtual void DistributeTo(PlayerDir_t dir,Card_t card);
    virtual void ActionAfterGang(PlayerDir_t dir);

    virtual void UpdateCards(PlayerDir_t dir,ARRAY_ACTION action,Card_t actKind = CARD_UNKNOWN);
    void SetDecision(PlayerDir_t dir,ActionId_t act);
    PlayerDir_t RightOf(PlayerDir_t dir)const;
    PlayerDir_t TurnTo(PlayerDir_t dir = NEXT);
    void RecordOutCard( Card_t kind );
    bool IsMing(int player) const;

    PlayerDir_t GetLastWinner();
    void SetWin(WinKind_t kind,int player);
    const WinInfo_t &GetWin();
    bool IsWinner(int no);

    virtual void CreateRace(RaceLayer *uiManager);
    virtual void StartGame();
    virtual void InitPlayers();

    void QiangGangHuJudge(PlayerDir_t target,Card_t kind);
    int  Shuffle();

	void set_aims_sequence(const int p_aim[]);

    GameMode_t _MODE;
protected:

    StrategyRm *_strategy;

    void ForceHandout();
    
    RaceLayer  *_uiManager;

    int         aim[PLAYER_NUM];

    WinInfo_t   _lastWin;
    Player     *_players[PLAYER_NUM];

    CardList   *_gRiver;
    Card_t      _unDistributedCards[TOTAL_CARD_NUM];
    int         _distributedNum;

    bool _isGameStart;

    bool _isNewDistributed;

    bool _isQiangGangAsking;
    bool _isDoubleHuAsking;
    
    int  _firstMingNo;
    int  _continue_gang_times;

    int  _qiangGangTargetNo;

    bool _isGangHua;
    int  _otherOneForDouble;//this could be calculated via GetWin.

    Card _curEffectCard;
    void CancelEffectCard();
    void SetEffectCard(int kind,int status);
    bool IsCurEffectCard(const CardNode_t *card);

    bool _isMingTime;

    bool _isTuoGuan;
    
    Logger *_logger;
private:
    void _LogRoundCards();
protected:
    static RoundManager *_instance;
};

#endif
