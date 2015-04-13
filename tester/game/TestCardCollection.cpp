
#include "./../../game/CardCollection.h"

void test_card_list() {
	Card_t cards[] = {TIAO_1,TONG_1,ZHONG,FA,BAI};
	CardInHand list;

	list.init(cards,sizeof(cards)/sizeof(cards[0]));
	//test show
	list.show();
	//test get
	CardInHand::iterator it;
	for(it=list.begin();it!=list.end();it++) {
		printf("%d ",(*it)->kind);
	}
	//test push_back
	list.push_back(TIAO_2);
	list.show();
}