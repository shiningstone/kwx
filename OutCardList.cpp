#include "OutCardList.h"

outCardNode::outCardNode()
{
	pNext=NULL;
}

outCardNode::outCardNode(const Card item,outCardNode* link)
{
	data.kind=item.kind;
	data.status=item.status;
	pNext=link;
}

outCardList::outCardList()
{
	inital();
}

outCardList::~outCardList()
{
	delete head;
	head=NULL;
}

bool outCardList::inital()
{
	head=new outCardNode;
	length=0;
	return true;
}

bool outCardList::clear()
{
	delete head;
	head=NULL;
	length=0;
	return true;
}

bool outCardList::insertItem(const Card item)
{
	outCardNode *currentPtr=head;
	while(currentPtr->pNext!=NULL)
	{
		currentPtr=currentPtr->pNext;
	}
	currentPtr->pNext=new outCardNode(item,NULL);
	length=length+1;
	return true;
}

bool outCardList::deleteItem()
{
	outCardNode *currentPtr=head;
	outCardNode *nextPtr=head;
	while(nextPtr->pNext!=NULL)
	{
		currentPtr=nextPtr;
		nextPtr=nextPtr->pNext;
	}
	delete currentPtr->pNext;
	currentPtr->pNext=NULL;
	length-=1;
	return true;
}

bool outCardList::getCard(Card& r,int i) const
{
	int position=1;
	outCardNode* curNode=head;
	if(curNode->pNext==NULL||i>length)
	{
		return false;
	}
	for(position;position<=length && position<=i;position++)
	{
		curNode=curNode->pNext;
		if(curNode==NULL)
		{
			return false;
		}
	}
	r.kind=curNode->data.kind;
	r.status=curNode->data.status;
	return true;
}
