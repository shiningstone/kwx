
#ifndef __ROUND_MANAGER__
#define __ROUND_MANAGER__

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

class RoundManager {
    friend class NetRaceLayer;
    friend class Ai;
public:
    static RoundManager *getInstance();
    static void  destroyInstance();
    Ai             *_ai;

    RoundManager(NetRaceLayer *uiManager);
    ~RoundManager();

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

    void UpdateCards(PlayerDir_t dir,ARRAY_ACTION action);

    void QiangGangHuJudge(PlayerDir_t dir);
    CartApperance_t GetCardApperance(PlayerDir_t dir,int idx);
    
    int _GroupIdx(int idx,CARD_ARRAY *cards);

    PlayerDir_t GetLastWinner();
    void SetWin(WinKind_t kind,int player);
    void GetWin(WinInfo_t &info);
    bool IsWinner(int no);

    PlayerDir_t TurnToNext();

    void RecordOutCard( Card card );
    void RenewOutCard();
    void RecordHandOut(int cardIdx);

    void Init();
    void InitPlayers();
    void LoadPlayerInfo();
    bool IsTing(int player);

    
    int  Shuffle();
    bool GetReadyStatus(PlayerDir_t dir);
    bool WaitUntilAllReady();

	void set_aims_sequence(const int p_aim[]);

    /* main process */
    virtual void CreateRace(Scene *scene);
    virtual void StartGame();
protected:
    GameMode_t _MODE;
    
    NetRaceLayer *_uiManager;
	int         aim[PLAYER_NUM];

    WinInfo_t   _lastWin;
    NetRole     *_players[PLAYER_NUM];
    CardHolder  *_cardHolders[PLAYER_NUM];

    outCardList *_river;
    int         _unDistributedCards[TOTAL_CARD_NUM];
    int         _distributedNum;

    void        _GenerateIds(int ids[]);
    bool _isGameStart;
    bool _isGangAsking;
    bool _isQiangGangAsking;
    bool _isDoubleHuAsking;
    int  _firstMingNo;
    int  _continue_gang_times;
    int  _qiangGangTargetNo;
    int  _curPlayer;
    bool _isGangHua;
    int  _otherOneForDouble;//this could be calculated via GetWin.
    bool _isWaitDecision;
    bool _isCardFromOthers;

    Card _curEffectCard;
    void CancelEffectCard();
    void SetEffectCard(int kind,int status);
    bool IsCurEffectCard(Card card);

    bool _isMingTime;
    int  _tempActionToDo;
    int  _actionToDo;
        
    CARD_KIND  _lastHandedOutCard;
    int  _lastAction;
    int  _lastActionWithGold;
    int  _lastActionSource;

    bool _isMyShowTime;
    bool _isTuoGuan;
    Card_t _otherHandedOut;
    
    Logger *_logger;
private:
    static RoundManager *_instance;
};

#endif
