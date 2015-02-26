
#ifndef __ROUND_MANAGER__
#define __ROUND_MANAGER__

#include "./../utils/LogManager.h"

class Role;

#define PLAYER_NUMBER 3

typedef enum {
    SINGLE_WIN,
    DOUBLE_WIN,
    NONE_WIN,
}WinKind_t;

typedef struct {
    WinKind_t  kind;
    int        player;
}WinInfo_t;

class RoundManager {
    friend class NetRaceLayer;
public:
    RoundManager();
    ~RoundManager();
    
    int GetLastWinner();
    void SetWin(WinKind_t kind,int player);
    void GetWin(WinInfo_t &info);
    bool IsWinner(int no, int curPlayer, int FirstMingPlayer);

    void RecordOutCard( Card card );
    void RenewOutCard();


    void InitPlayers();
    bool IsTing(int player);

    bool _isGangAsking;
    bool _isQiangGangAsking;
    bool _isDoubleHuAsking;
    int  _firstMingNo;
    int  _qiangGangTargetNo;
    int  _curPlayer;
    bool _isGangHua;
    int  _otherOneForDouble;//this could be calculated via GetWin.
    bool _isWaitDecision;
    bool _isCardFromOthers;

    int  _curEffectCardKind;
    int  _curEffectCardStatus;
    
    int  _tempActionToDo;
    int  _actionToDo;
        
    CARD_KIND  _lastHandedOutCard;
    int  _lastAction;
    int  _lastActionWithGold;
    int  _lastActionSource;

    int  Shuffle();
    int  NotifyStart();
    bool GetReadyStatus(int tableId);
    bool WaitUntilAllReady();
    int  AllowMovement();
    bool WaitForDistribute();
    bool WaitForAction();
    
private:
    Logger *_logger;

    WinInfo_t   _lastWin;
    Role        *_players[PLAYER_NUMBER];
    bool        _ready[PLAYER_NUMBER];

    outCardList *_river;
    int         _unDistributedCards[TOTAL_CARD_NUM];
    int         _distributedNum;
};

#endif
