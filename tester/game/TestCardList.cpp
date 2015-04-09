
#include "./../../game/CardList.h"

void test_card_list() {
	Card_t cards[] = {TIAO_1,TONG_1,ZHONG,FA,BAI};
	CardList list(cards,sizeof(cards)/sizeof(cards[0]));

	//test show
	list.show();
	//test get
	CardList::iterator it;
	for(it=list.begin();it!=list.end();it++) {
		printf("%d ",(*it)->kind);
	}
	//test push_back
	list.push_back(TIAO_2);
	list.show();
}