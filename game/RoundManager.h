
#ifndef __ROUND_MANAGER__
#define __ROUND_MANAGER__

class Logger;
class RoundManager {
public:
    RoundManager();
    ~RoundManager();

    int GetLastWinner();
    int Shuffle(int *cardSeq);
    int NotifyStart();
    bool GetReadyStatus(int tableId);
    bool WaitUntilAllReady();
    int AllowMovement();

private:
    Logger *_logger;

    int _lastWinner;
};

#endif