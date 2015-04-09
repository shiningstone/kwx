
#ifndef __AI__
#define __AI__

#include "RaceType.h"
#include "CardList.h"

class RoundManager;
class Ai {
public:
    void init_target(CardInHand *list,ROBOT_TARGET *target,int hu_len1,int hu_len2);
    void collect_resources(HAH *res,CARD_KIND target1[],CARD_KIND target2[],int *len1,int *len2);
    int  ChooseWorstCard(bool &kouRequest);
    Card_t FindGangCards(int cardIdx[4],CARD_ARRAY *cards,Card_t target,int gangType,bool isTing,bool isCardFromOthers);
    Card_t FindPengCards(int cardIdx[2],CARD_ARRAY *cards,Card_t target);
    int  ReChooseAfterGang(int chosen,int gangIdx[3],bool isCardFromOthers,int gang4=INVALID/* set to INVALID if MingGang*/);
    int  ReChooseAfterPeng(int chosen,int pengIdx[2]);
private:
    RoundManager *_roundManager;
    
    void _CollectResouce(HAH *res);

/*************************************
        kou card info
*************************************/
public:
    void ClearKouCardInfo();
    void AddKouCardGroup(Card_t kind,int *idx);
    bool IsKouCardInclude(Card_t kind);
    int  KouCardGroupNum();
    int  KouCardIndex(int group,int idx);
    CARD_STATUS  KouCardStatus(int gIdx);
    void SetKouCardStatus(int gIdx,CARD_STATUS status);
    void SwitchGroupStatus(int group,CARD_ARRAY *cards);
    void Refresh(CARD_ARRAY *cards);
    Card_t KouCardKind(int gIdx);
    void KouCardCheck(PlayerDir_t dir);
    int  _FindCards(int idx[],CardInHand *list,CARD_KIND kind);

    void MingKouChoose(PlayerDir_t dir);
/*************************************
        round info
*************************************/
    void UpdateAtFirstRound(int &actionToDo);
    
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
/*************************************
        kou card info
*************************************/
private:
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
