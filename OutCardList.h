#ifndef  OUT_CARD_LIST_H_
#define  OUT_CARD_LIST_H_
#include "RaceType.h"
#include "cocos2d.h"

struct outCardNode
{
public:
	outCardNode* pNext;
	Card data;
	outCardNode();
	outCardNode(const Card item,outCardNode* link);
};

class outCardList
{
public:
	outCardNode *head;
	int length;
public:
	outCardList();
	~outCardList();
	bool inital();
	bool clear();
	bool insertItem(const Card item);
	bool deleteItem();
	bool getCard(Card& r,int i) const;
};

#endif // OUT_CARD_LIST_H_