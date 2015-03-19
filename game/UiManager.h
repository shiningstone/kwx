
#ifndef __UI_MANAGER__
#define __UI_MANAGER__

#include "RoundManager.h"

class UiManager {
/***************************************
        singleton
***************************************/
public:
    static UiManager *getInstance(RoundManager *roundManager);
    static void  destroyInstance();
private:
    RoundManager *_roundManager;
protected:
    UiManager(RoundManager *roundManager);
    ~UiManager();

    static UiManager *_instance;
};

#endif
