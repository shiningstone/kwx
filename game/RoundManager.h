
#ifndef __ROUND_MANAGER__
#define __ROUND_MANAGER__

#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

#include "RaceType.h"

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"
#include "CardCollection.h"

extern void load_test_round(int idx,int *output);

class Player;
class RmStrategy;

class RoundManager {
    friend class RaceLayer;
	friend class PlayerOthers;
public:
    void update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive);
protected:
    void UpdateGold(int gold[3]);
private:
    void CalcAnGangGold(int winner,int gold[3],int continueGang = 1);
    void CalcMingGangGold(int winner,int giver,int gold[3],int continueGang = 1);
    void CalcSingleWinGold(int gold[3], int winner,int whoGive);
    void CalcDoubleWinGold(int gold[3], int giver);
    void CalcNoneWinGold(int gold[3], int giver);
    void CalcHuGold(int gold[3],const WinInfo_t &win);
    void CalculateGold(int gold[3],PlayerDir_t GoldWinner,GoldKind_t goldKind,PlayerDir_t whoGive);

    int PREMIUM_LEAST;

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
    CartApperance_t GetCardApperance(PlayerDir_t dir,int idx);
    
    int _GroupIdx(int idx,CARD_ARRAY *cards);

    PlayerDir_t GetLastWinner();
    void SetWin(WinKind_t kind,int player);
    void GetWin(WinInfo_t &info);
    bool IsWinner(int no);

    PlayerDir_t TurnToNext();

    void RecordOutCard( Card_t kind );

    virtual void InitPlayers();
    bool IsMing(int player) const;

    
    int  Shuffle();

	void set_aims_sequence(const int p_aim[]);

    /* main process */
    GameMode_t _MODE;

    virtual void CreateRace(Scene *scene);
    virtual void StartGame();

    RmStrategy *_strategy;
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

    static RoundManager *_instance;
};

#endif
