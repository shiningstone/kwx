
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

extern void load_test_round(int idx,int *output);

class Player;
class StrategyRm;

class RoundManager {
    friend class RaceLayer;
	friend class PlayerOthers;
    friend class StrategyLocalRM;
	friend class StrategyPlayer;
protected:
    void update_gold(int gold[3]);
private:
    unsigned int _GetPlayerReaction(PlayerDir_t dir,bool prevTingStatus);
    void _HandleCardNewDistributed(PlayerDir_t dir);
    void _HandleCardFrom(PlayerDir_t dir);
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
    int  _curPlayer;
    ActionId_t  _lastActionWithGold;
    int  _lastActionSource;
    
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

    void QiangGangHuJudge(PlayerDir_t dir);
    
    PlayerDir_t GetLastWinner();
    void SetWin(WinKind_t kind,int player);
    const WinInfo_t &GetWin();
    bool IsWinner(int no);

    PlayerDir_t TurnTo(PlayerDir_t dir = NEXT);

    void RecordOutCard( Card_t kind );

    virtual void InitPlayers();
    bool IsMing(int player) const;

    
    int  Shuffle();

	void set_aims_sequence(const int p_aim[]);

    /* main process */
    GameMode_t _MODE;

    virtual void CreateRace(Scene *scene);
    virtual void StartGame();

    StrategyRm *_strategy;
protected:
    typedef struct _Context_t {
    }Context_t;

    Context_t  _ctx;
    
    RaceLayer  *_uiManager;

    int         aim[PLAYER_NUM];

    WinInfo_t   _lastWin;
    Player     *_players[PLAYER_NUM];

    CardList   *_gRiver;
    int         _unDistributedCards[TOTAL_CARD_NUM];
    int         _distributedNum;

    void        _GenerateIds(int ids[]);

    bool _isGameStart;

    bool _isWaitForMyDecision;
    
    bool _isNewDistributed;

    bool _isGangAsking;
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
    void _LoadRandomCardSequence();
protected:
    static RoundManager *_instance;
};

#endif
