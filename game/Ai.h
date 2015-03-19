
#ifndef __AI__
#define __AI__

#include "RaceType.h"

class RoundManager;
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

    void ClearKouCardInfo();
    void AddKouCardGroup(Card_t kind,int *idx);
    bool IsKouCardInclude(Card_t kind);
    int  KouCardGroupNum();
    int  KouCardIndex(int group,int idx);
    CARD_STATUS  KouCardStatus(int gIdx);
    void SetKouCardStatus(int gIdx,CARD_STATUS status);
    Card_t KouCardKind(int gIdx);
protected:
    Ai(RoundManager *roundManager);
    ~Ai();

    static Ai *_instance;
private:
    /*************************************
            kou card info
    *************************************/
    typedef struct {
        CARD card;
        int  idxInHand[3];
    }CardGroup_t;
    
    typedef struct {
        int num;
        CardGroup_t group[4];
    }KouCardInfo_t;

    KouCardInfo_t _bufKouCards;
};

#endif
