
#ifndef __PLAYER_OTHERS__
#define __PLAYER_OTHERS__

#include "Player.h"

class Logger;
class PlayerOthers:public Player
{
	public:
		PlayerOthers(PlayerDir_t dir);
        PlayerOthers(int id,PlayerDir_t dir);

        int  PickupForMing(ActionId_t &ming,bool &canKou);

		virtual ~PlayerOthers();
    private:
        Logger      *_logger;
        
        typedef struct _KindPosition {
            int           num;
            int           position[4];
        }KindPosition;
        
        typedef struct _Context_t {
            /* global */
            CardList     *river;
            int           remain;
            /* others */
            TingInfo_t   *OthersTing[2];
            /* self */
            KindPosition  cards[TOTAL_CARD_KIND];
            int           huNum;
            RobotTarget_t aim;
        }Context_t;

        Context_t    _ctx;
	private:
        bool IsInStrictSequences(Card_t kind,int seqIdx) const;
        bool IsInSequences(Card_t kind) const;
        bool IsStable(Card_t kind) const;
        int  PickupForSameColor(int reservedColor);
        int  PickupForSevenCouples();
        int  PickupForFourPeng();
        int  PickupForPiHu();

        int  _FindSingleChar();
        int  _FindSingleAndNonSequence(Card_t HeadKind,Card_t TailKind);
        int  _FindSingleAndUnstable(Card_t HeadKind,Card_t TailKind);

        void _CollectPosition(KindPosition *info);
        void _SetContext(RoundManager &context);
        
        bool OthersCanHu(Card_t kind) const;
        int  AvailNum(Card_t kind) const;

		int Robot_check_pickup_card(CARD_KIND kind,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int river_reserved_card(HAH *hash_table,int card);//
		int Robot_check_card_stable(HAH *card_array,CARD_KIND card);//
		int Robot_picup_single_for_samecolor(int color,HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_for_sevencouples(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_for_fourpeng(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_for_lowwin(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
		int Robot_pickup_single(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2);//
	public:
        int  choose_card(RoundManager &context,ActionId_t &ming,bool &canKou);
};

#endif 

