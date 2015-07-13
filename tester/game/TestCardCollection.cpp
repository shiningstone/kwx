
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
    bool result = _PatternMatch();
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

    cards.remove(3,deletes);
    assert( cards.Equals(exp) );

	exp.Load(TIAO_1,TIAO_5,TIAO_6,CARDS_END);
    cards.Load(TIAO_1,TIAO_2,TIAO_3,TIAO_4,TIAO_5,TIAO_6,CARDS_END);

	deletes[0] = 1;
	deletes[1] = 2;
	deletes[2] = 3;
    cards.remove(3,deletes);
    assert( cards.Equals(exp) );

    cards.Load(TIAO_4,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1,CARDS_END);
    cards.insert(TIAO_3);
    exp.Load(TIAO_3,TIAO_4,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1,CARDS_END);
    assert( cards.Equals(exp) );

    cards.Load(TIAO_4,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1,CARDS_END);
    cards.insert(TIAO_5);
    exp.Load(TIAO_4,TIAO_5,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1,CARDS_END);
    assert( cards.Equals(exp) );

    cards.Load(TIAO_4,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1,CARDS_END);
    cards.insert(TIAO_6);
    exp.Load(TIAO_4,TIAO_6,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1,CARDS_END);
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


    cards.Load(TONG_1,TONG_1,TONG_1,TONG_1,TONG_2,TONG_2,TONG_3,TONG_3,TONG_4,TONG_6,TONG_7,TONG_9,CARDS_END);
    assert( !cards.Match() );
}

void test_can_hu() {
    TestSmartList cards;
    cards.Load(TIAO_4,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1,CARDS_END);
    cards.insert(TIAO_5);
    assert(cards.can_hu());

    cards.Load(TIAO_3,TIAO_3,TIAO_3,TIAO_4,TIAO_5,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1,CARDS_END);
    cards.insert(TIAO_1);
    assert(!cards.can_hu());

    cards.Load(TIAO_3,TIAO_3,TIAO_3,TIAO_4,TIAO_5,TIAO_6,TIAO_7,TIAO_8,TIAO_9,TONG_1,TONG_1,CARDS_END);
    cards.len--;                    
    cards.insert(TIAO_1);       
    assert(!cards.can_hu());

    TestSmartList card2;
    card2.Load(TIAO_3,TIAO_3,TONG_1,TONG_1,TONG_1,TONG_4,TONG_5,TONG_8,TONG_8,TONG_8,CARDS_END);
    card2.insert(TONG_6);
    assert(card2.can_hu());
}

void test_refresh_empty() {
    CardInHand result;
    CardInHand realCards;
    CardNode_t nodes[14];

    Card_t     cards[14] = {
        TIAO_1,TIAO_1,TIAO_1,TIAO_1,TIAO_2,
        TIAO_2,TIAO_3,TIAO_3,TIAO_3,TIAO_3,
        TIAO_4,TIAO_4,TIAO_4,BAI};
    realCards.init(cards,14);

    for(int i=0;i<realCards.size();i++) {
        nodes[i] = *(realCards.at(i));
    }
    result.refresh(nodes,14);

    for(int i=0;i<result.size();i++) {
        assert(cards[i]==result.at(i)->kind);
    }
}

void test_refresh_different() {
    CardInHand result;
    CardInHand realCards;
    CardNode_t nodes[14];

    Card_t     cards1[14] = {
        TIAO_1,TIAO_1,TIAO_1,TIAO_1,TIAO_2,
        TIAO_2,TIAO_3,TIAO_3,TIAO_3,TIAO_3,
        TIAO_4,TIAO_4,TIAO_4,BAI};
    realCards.init(cards1,14);

    Card_t     cards2[14] = {
        TONG_1,TONG_1,TONG_1,TONG_1,TONG_2,
        TONG_2,TONG_3,TONG_3,TONG_3,TONG_3,
        TONG_4,TONG_4,TONG_4,ZHONG};
    result.init(cards2,14);

    for(int i=0;i<realCards.size();i++) {
        nodes[i] = *(realCards.at(i));
    }
    result.refresh(nodes,14);

    for(int i=0;i<result.size();i++) {
        assert(cards1[i]==result.at(i)->kind);
    }
}

void test_refresh_peng() {
    CardInHand result;
    CardInHand realCards;
    CardNode_t nodes[14];

    Card_t     cards[14] = {
        TIAO_1,TIAO_1,TIAO_1,TIAO_1,TIAO_2,
        TIAO_2,TIAO_3,TIAO_3,TIAO_3,TIAO_3,
        TIAO_4,TIAO_4,TIAO_4,TIAO_2};
    realCards.init(cards,14);
    realCards.perform(aPENG,TIAO_2);

    CardNode_t pengcard = {
        TIAO_2,
        sPENG,
        false,
    };
    result.push_back(&pengcard);
    result.push_back(&pengcard);
    result.push_back(&pengcard);
    result.FreeStart += 3;

    for(int i=0;i<realCards.size();i++) {
        nodes[i] = *(realCards.at(i));
    }
    result.refresh(nodes,14);

    for(int i=0;i<result.size();i++) {
        assert(realCards.at(i)->kind==result.at(i)->kind);
        assert(realCards.at(i)->status==result.at(i)->status);
    }
}

void test_refresh_an_gang() {
    CardInHand result;
    CardInHand realCards;
    CardNode_t nodes[14];

    Card_t     cards[14] = {
        TIAO_1,TIAO_1,TIAO_1,TIAO_1,TIAO_2,
        TIAO_2,TIAO_3,TIAO_3,TIAO_3,TIAO_3,
        TIAO_4,TIAO_4,TIAO_4,TIAO_2};
    realCards.init(cards,14);
    realCards.perform(aAN_GANG,TIAO_1);

    CardNode_t gangcard = {
        CARD_UNKNOWN,
        sAN_GANG,
        false,
    };
    result.push_back(&gangcard);
    result.push_back(&gangcard);
    result.push_back(&gangcard);
    result.push_back(&gangcard);
    result.FreeStart += 4;

    for(int i=0;i<realCards.size();i++) {
        nodes[i] = *(realCards.at(i));
    }
    result.refresh(nodes,14);

    for(int i=0;i<result.size();i++) {
        assert(realCards.at(i)->kind==result.at(i)->kind);
        assert(realCards.at(i)->status==result.at(i)->status);
    }
}

void test_refresh_mixed() {
    CardInHand result;
    CardInHand realCards;
    CardNode_t nodes[14];

    Card_t     cards[14] = {
        TIAO_1,TIAO_1,TIAO_1,TIAO_1,TIAO_2,
        TIAO_2,TIAO_2,TIAO_3,TIAO_3,TIAO_3,
        TIAO_4,TIAO_4,TIAO_3,TIAO_2};
    realCards.init(cards,14);
    realCards.perform(aAN_GANG,TIAO_1);
    realCards.perform(aMING_GANG,TIAO_3);
    realCards.pop_back();
    realCards.push_back(TIAO_2);
    realCards.perform(aPENG,TIAO_2);

    CardNode_t angangcard = {
        CARD_UNKNOWN,
        sAN_GANG,
        false,
    };
    result.push_back(&angangcard);
    result.push_back(&angangcard);
    result.push_back(&angangcard);
    result.push_back(&angangcard);
    result.FreeStart += 4;

    CardNode_t mgangcard = {
        TIAO_3,
        sMING_GANG,
        false,
    };
    result.push_back(&mgangcard);
    result.push_back(&mgangcard);
    result.push_back(&mgangcard);
    result.push_back(&mgangcard);
    result.FreeStart += 4;

    CardNode_t pengcard = {
        TIAO_2,
        sPENG,
        false,
    };
    result.push_back(&pengcard);
    result.push_back(&pengcard);
    result.push_back(&pengcard);
    result.FreeStart += 3;

    for(int i=0;i<realCards.size();i++) {
        nodes[i] = *(realCards.at(i));
    }
    result.refresh(nodes,14);

    for(int i=0;i<result.size();i++) {
        assert(realCards.at(i)->kind==result.at(i)->kind);
        assert(realCards.at(i)->status==result.at(i)->status);
    }
}

void test_refresh() {
    test_refresh_empty();
    test_refresh_different();
    test_refresh_peng();
    test_refresh_an_gang();
    test_refresh_mixed();
}

void test_basic() {
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
}

void test_judge_long_qi_dui() {
	int seq[] = {
        TIAO_3,TIAO_3,TIAO_8,TIAO_8,TONG_2,TONG_2,ZHONG,ZHONG,ZHONG,FA,FA,BAI,BAI,ZHONG,
    };
    Card_t cards[18];
    for(int i=0;i<sizeof(seq)/sizeof(seq[0]);i++) {
        cards[i] = (Card_t)seq[i];
    }

	CardInHand list;
	list.init(cards,sizeof(seq)/sizeof(seq[0]));

    list.update_statistics((Card_t)ZHONG);
    assert(_is_active(list.statHuFanMask,RH_HAOHUAQIDUI));
    assert(!_is_active(list.statHuFanMask,RH_ANSIGUI));
}

void test_judge_peng_peng_hu() {
	int seq[] = {
        0x0d,0x0d,0x0d,0x00,0x00,0x00,0x01,0x01,0x01,0x06,0x06,0x10,0x10,0x10,
    };
    Card_t cards[18];
    for(int i=0;i<sizeof(seq)/sizeof(seq[0]);i++) {
        cards[i] = (Card_t)seq[i];
    }

	CardInHand list;
	list.init(cards,sizeof(seq)/sizeof(seq[0]));

    list.update_statistics((Card_t)0x10);
    assert(_is_active(list.statHuFanMask,RH_SIPENG));
}

void test_judge_no_peng_peng_hu() {
	int seq[] = {
        TONG_3,TONG_3,TONG_3,TONG_6,TONG_6,TONG_6,TIAO_3,TIAO_3,TIAO_3,TIAO_4,TIAO_4,TIAO_4,TIAO_5,TIAO_3,
    };
    Card_t cards[18];
    for(int i=0;i<sizeof(seq)/sizeof(seq[0]);i++) {
        cards[i] = (Card_t)seq[i];
    }

	CardInHand list;
	list.init(cards,sizeof(seq)/sizeof(seq[0]));

    list.update_statistics((Card_t)0x10);
    assert(!_is_active(list.statHuFanMask,RH_SIPENG));
}

void test_judge_ming_si_gui() {
	int seq[] = {
        TONG_4,TONG_4,TONG_8,TONG_8,TIAO_9,TIAO_9,TIAO_7,TIAO_7,TONG_3,TONG_5,TONG_4,TIAO_9,TONG_8,TONG_4,
    };
    Card_t cards[18];
    for(int i=0;i<sizeof(seq)/sizeof(seq[0]);i++) {
        cards[i] = (Card_t)seq[i];
    }

	CardInHand list;
	list.init(cards,sizeof(seq)/sizeof(seq[0]));
    list.perform(aPENG,TONG_4,false);
    list.perform(aPENG,TONG_8,false);
    list.perform(aPENG,TIAO_9,false);

    list.update_statistics(TONG_4);
    assert(_is_active(list.statHuFanMask,RH_MINGSIGUI));
}

void test_judge_an_si_gui() {
	int seq[] = {
        TONG_1,TONG_2,TONG_2,TONG_2,TONG_2,TONG_3,TONG_6,
        TONG_7,TONG_8,TONG_8,TONG_8,TONG_9,TONG_9,TONG_5,
    };
    Card_t cards[18];
    for(int i=0;i<sizeof(seq)/sizeof(seq[0]);i++) {
        cards[i] = (Card_t)seq[i];
    }

	CardInHand list;
	list.init(cards,sizeof(seq)/sizeof(seq[0]));

    list.update_statistics(TONG_5);
    assert(_is_active(list.statHuFanMask,RH_QINYISE));
    assert(_is_active(list.statHuFanMask,RH_ANSIGUI));
}

void test_judge_ming() {
	int seq[] = {
        FA,FA,FA, TIAO_1,TIAO_2,TIAO_3,TIAO_3,TIAO_4,TIAO_5,TIAO_6,TONG_6,TONG_7,TONG_8,
        TIAO_2,
    };
    Card_t cards[18];
    for(int i=0;i<sizeof(seq)/sizeof(seq[0]);i++) {
        cards[i] = (Card_t)seq[i];
    }

	CardInHand list;
	list.init(cards,sizeof(seq)/sizeof(seq[0]));
    assert(list.scan_ming());
}

void test_judge_ming2() {
	int seq[] = {
        FA,FA,FA, TIAO_5,TIAO_6,TIAO_7,TONG_3,TONG_4,TONG_5,TONG_5,TONG_6,TONG_7,ZHONG,
        ZHONG,
    };
    Card_t cards[18];
    for(int i=0;i<sizeof(seq)/sizeof(seq[0]);i++) {
        cards[i] = (Card_t)seq[i];
    }

	CardInHand list;
	list.init(cards,sizeof(seq)/sizeof(seq[0]));
    assert(list.scan_ming());
}

void test_lian_gang() {
	int seq[] = {
        TONG_3,TONG_3,TONG_7,TONG_7,TIAO_4,TIAO_4,TIAO_4,ZHONG,ZHONG,ZHONG,TONG_8,ZHONG,TIAO_4,TONG_7,TONG_3,
    };
    Card_t cards[18];
    for(int i=0;i<sizeof(seq)/sizeof(seq[0]);i++) {
        cards[i] = (Card_t)seq[i];
    }

	CardInHand list;
	list.init(cards,sizeof(seq)/sizeof(seq[0]));
    list.perform(aPENG,TONG_3,false);
    list.perform(aPENG,TONG_7,false);
    list.perform(aMING_GANG,TIAO_4,false);
    list.perform(aAN_GANG,ZHONG,true);
}
/**********************************
    main
**********************************/
void test_smart_list() {
	test_remove();
    test_pattern_match();
    test_can_hu();
}

void test_card_list() {
    test_smart_list();
    test_basic();
    test_refresh();

    test_lian_gang();

    test_judge_long_qi_dui();
    test_judge_an_si_gui();
    test_judge_ming();
    test_judge_ming2();
    test_judge_no_peng_peng_hu();
    test_judge_peng_peng_hu();
    test_judge_ming_si_gui();
}
