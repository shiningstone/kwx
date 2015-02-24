
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

class RoundManager {
public:
    RoundManager();
    ~RoundManager();

    void SetPlayers(Role *players[]);
    int GetLastWinner();
    void SetWin(WinKind_t kind,int player);
    int Shuffle(int *cardSeq);
    int NotifyStart();
    bool GetReadyStatus(int tableId);
    bool WaitUntilAllReady();
    int AllowMovement();
    bool WaitForDistribute();
    bool WaitForAction();
    
private:
    Logger *_logger;

    Role *_players[PLAYER_NUMBER];
    int  _lastWinner;
};

#endif