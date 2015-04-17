
#include <assert.h>
#include <stdarg.h>

#include "./../../game/CardCollection.h"

/**********************************
    support
**********************************/
class TestSmartList : public SmartList {
public:
    void Load(Card_t first,...);
    bool Match();
	bool Equals(const SmartList &exp);
};

#define CARDS_END CARD_UNKNOWN

void TestSmartList::Load(Card_t first,...) {
    kind[0] = first;
    len = 1;
    
    Card_t card;

    va_list args;
    va_start(args,first);
    while ((card = va_arg(args, Card_t)) != CARDS_END) {
        kind[len] = card;
        len++;
    }
    va_end(args);
}

bool TestSmartList::Match() {
    bool result = PatternMatch();
    if(result) {
        printf("match    : ");
    } else {
        printf("dismatch : ");
    }

	for(int i=0;i<len;i++) {
		printf("%02d ",kind[i]);
	}

    printf("\n");

    return result;
}

bool TestSmartList::Equals(const SmartList &exp) {
	if(len==exp.len) {
		for(int i=0;i<exp.len;i++) {
			if(kind[i]!=exp.kind[i]) {
				return false;
			}	
		}

		return true;
	}

	return false;
}

/**********************************
    testcases
**********************************/
void test_remove() {
    TestSmartList cards;
    TestSmartList exp;

	int i = 0;
	int deletes[] = {i,i+2,i+4};

	exp.Load(TIAO_2,TIAO_4,TIAO_6,CARDS_END);
    cards.Load(TIAO_1,TIAO_2,TIAO_3,TIAO_4,TIAO_5,TIAO_6,CARDS_END);

    cards._Remove(3,deletes);
    assert( cards.Equals(exp) );

	exp.Load(TIAO_1,TIAO_5,TIAO_6,CARDS_END);
    cards.Load(TIAO_1,TIAO_2,TIAO_3,TIAO_4,TIAO_5,TIAO_6,CARDS_END);

	deletes[0] = 1;
	deletes[1] = 2;
	deletes[2] = 3;
    cards._Remove(3,deletes);
    assert( cards.Equals(exp) );

}

void test_pattern_match() {
    TestSmartList cards;

    cards.Load(TIAO_1,TIAO_1,TIAO_1,CARDS_END);
    assert( cards.Match() );

    cards.Load(TIAO_1,TIAO_2,TIAO_3,CARDS_END);
    assert( cards.Match() );

    cards.Load(TIAO_1,TIAO_2,TIAO_1,CARDS_END);
    assert( !cards.Match() );

    cards.Load(TIAO_8,TIAO_9,TONG_1,CARDS_END);
    assert( !cards.Match() );

    cards.Load(TIAO_1,TIAO_2,TIAO_3,TIAO_4,CARDS_END);
    assert( !cards.Match() );

    

    cards.Load(TIAO_1,TIAO_2,TIAO_2,TIAO_3,TIAO_3,TIAO_4,CARDS_END);
    assert( cards.Match() );

    cards.Load(TIAO_1,TIAO_2,TIAO_2,TIAO_2,TIAO_2,TIAO_3,CARDS_END);
    assert( cards.Match() );

    cards.Load(TIAO_1,TIAO_1,TIAO_2,TIAO_2,TIAO_3,TIAO_3,CARDS_END);
    assert( cards.Match() );



    cards.Load(TIAO_1,TIAO_2,TIAO_2,TIAO_2,TIAO_3,TIAO_3,TIAO_3,TIAO_4,TIAO_4,CARDS_END);
    assert( cards.Match() );

    cards.Load(TIAO_1,TIAO_1,TIAO_2,TIAO_2,TIAO_2,TIAO_3,TIAO_3,TIAO_3,TIAO_4,CARDS_END);
    assert( cards.Match() );



    cards.Load(TIAO_1,TIAO_1,TIAO_2,TIAO_2,TIAO_2,TIAO_2,TIAO_3,TIAO_3,TIAO_3,TIAO_3,TIAO_4,TIAO_4,CARDS_END);
    assert( cards.Match() );



    cards.Load(TIAO_1,TIAO_1,TIAO_2,TIAO_2,TIAO_3,TIAO_3,TIAO_4,TIAO_4,TIAO_5,TIAO_5,TIAO_6,TIAO_6,CARDS_END);
    assert( cards.Match() );
}

/**********************************
    main
**********************************/
void test_card_list() {
	Card_t cards[] = {TIAO_1,TONG_1,ZHONG,FA,BAI};
	CardInHand list;

	list.init(cards,sizeof(cards)/sizeof(cards[0]));
	//test index
	Card_t kind0 = list[0]->kind;
	Card_t kind1 = list[2]->kind;
	Card_t kind2 = list[4]->kind;
	printf("%d %d %d\n", kind0, kind1, kind2);
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

	test_remove();
    test_pattern_match();
}
