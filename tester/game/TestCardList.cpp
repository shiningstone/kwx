
#include "./../../game/CardList.h"

void test_card_list() {
	Card_t cards[] = {TIAO_1,TONG_1,ZHONG,FA,BAI};
    CardInHand cardsInHand;

    cardsInHand.init(cards,sizeof(cards)/sizeof(cards[0]));
	//test show
	cardsInHand.show();
	//test get
	CardList::iterator it;
	for(it=cardsInHand.begin();it!=cardsInHand.end();it++) {
		printf("%d ",(*it)->kind);
	}
    printf("\n");
	//test push_back
	cardsInHand.push_back(TIAO_2);
	cardsInHand.show();
	//test erase
	cardsInHand.erase(cardsInHand.begin()+1,cardsInHand.begin()+2);
	cardsInHand.show();
    //test delete_card
	cardsInHand.init(cards,sizeof(cards)/sizeof(cards[0]));
	cardsInHand.show();
    cardsInHand.delete_card(0,2);
	cardsInHand.show();
    //test insert_card
	cardsInHand.init(cards,sizeof(cards)/sizeof(cards[0]));
	cardsInHand.show();
    CardNode_t card;
    card.kind = TIAO_2;
    card.status = sFREE;
    cardsInHand.insert_card(card,1);
	cardsInHand.show();


}