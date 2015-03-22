
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

#define PLAYER_NUMBER 3

class RoundManager {
    friend class NetRaceLayer;
    friend class Ai;
public:
    static RoundManager *getInstance();
    static void  destroyInstance();

    RoundManager(NetRaceLayer *uiManager);
    ~RoundManager();

    void RecvPeng();
    void RecvHu();
    void RecvGang();
    void RecvQi();
    void RecvHandout(int idx,Vec2 touch,int mode);
    /*internal action*/
    void RecvKouCancel();
    void RecvKouConfirm();
    void RecvMingCancel();

    void QiangGangHuJudge();
    CartApperance_t GetCardApperance(PlayerDir_t dir,int idx);
    TingInfo_t GetTingInfo(PlayerDir_t dir,int outCarIdx);
    
    int _GroupIdx(int idx,CARD_ARRAY *cards);

    NetRaceLayer *_uiManager;
    
    PlayerDir_t GetLastWinner();
    void SetWin(WinKind_t kind,int player);
    void GetWin(WinInfo_t &info);
    bool IsWinner(int no, int curPlayer, int FirstMingPlayer);

    void RecordOutCard( Card card );
    void RenewOutCard();
    void RecordHandOut(int cardIdx);

    void Init();
    void InitPlayers();
    void LoadPlayerInfo();
    bool IsTing(int player);

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
    
    int  Shuffle();
    int  NotifyStart();
    bool GetReadyStatus(int tableId);
    bool WaitUntilAllReady();
    int  AllowChooseCard();
    bool WaitForDistribute();
    bool WaitForAction();

    int FindGangCards(int dir,int cards[4],Card_t target=CARD_UNKNONW);

    /* main process */
    void StartGame(Scene *scene);
private:
    static RoundManager *_instance;
    Logger *_logger;

    WinInfo_t   _lastWin;
    NetRole     *_players[PLAYER_NUMBER];
    CardHolder  *_cardHolders[PLAYER_NUMBER];

    outCardList *_river;
    int         _unDistributedCards[TOTAL_CARD_NUM];
    int         _distributedNum;

    void _GenerateIds(int ids[]);
};

#endif
