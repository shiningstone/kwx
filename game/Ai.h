
#ifndef __AI__
#define __AI__

#include "RaceType.h"

#include "./../utils/BasicType.h"

class CardInHand;
class RoundManager;
class Ai {
public:
    void collect_resources(HAH *res,CARD_KIND target1[],CARD_KIND target2[],int *len1,int *len2);
    int  ChooseWorstCard(bool &kouRequest);
    Card_t FindGangCards(int cardIdx[4],CardInHand *cards,Card_t target,int gangType,bool isTing,bool isNewDistributed);
    int  ReChooseAfterGang(int chosen,int gangIdx[3],bool isNewDistributed,int gang4=INVALID/* set to INVALID if MingGang*/);
    int  ReChooseAfterPeng(int chosen,int pengIdx[2]);
private:
    RoundManager *_roundManager;
    
    void _CollectResouce(HAH *res);

/*************************************
        kou card info
*************************************/
public:
    void KouCardCheck(PlayerDir_t dir);
    void MingKouChoose(PlayerDir_t dir);
/*************************************
        round info
*************************************/
    void UpdateAtFirstRound(int &actionToDo);

/*************************************
        game calculation
*************************************/
    long sum_up_score(unsigned int fan);

/***************************************
        singleton
***************************************/
public:
    static Ai *getInstance(RoundManager *roundManager = NULL);
    static void  destroyInstance();
protected:
    Ai(RoundManager *roundManager);
    ~Ai();

    static Ai *_instance;
};

#endif
