
#ifndef __AI__
#define __AI__

#include "RaceType.h"

class Ai {
public:
    void collect_resources(HAH *res,CARD_KIND target1[],CARD_KIND target2[],int *len1,int *len2);
    int  ChooseWorstCard(bool &kouRequest);

private:
    RoundManager *_roundManager;
    
    void _CollectResouce(HAH *res);

/***************************************
        singleton
***************************************/
public:
    static Ai *getInstance(RoundManager *roundManager);
    static void  destroyInstance();
protected:
    Ai(RoundManager *roundManager);
    ~Ai();

    static Ai *_instance;
};

#endif
