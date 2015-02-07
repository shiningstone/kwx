#include "MyInfo.h"
USING_NS_CC;
USING_NS_CC_EXT;

MyInfo::MyInfo(void)
{
}


MyInfo::~MyInfo(void)
{
}

bool MyInfo::init()
{
	if(!Layer::create())
	{
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

    TableView* tableView = TableView::create(this, Size(1070,503));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(Vec2(origin.x+visibleSize.width*0.065,origin.y+visibleSize.height*0.1));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tableView);
	tableView->reloadData();

	return true;
}

void MyInfo::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size MyInfo::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(100, 100);
}

TableViewCell* MyInfo::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto string = String::createWithFormat("%ld", idx);
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new TableViewCell();
        cell->autorelease();
        auto sprite = Sprite::create("fengexian5.png");
        sprite->setAnchorPoint(Vec2::ZERO);
        sprite->setPosition(Vec2(0, 0));
        cell->addChild(sprite);
    }
    return cell;
}

ssize_t MyInfo::numberOfCellsInTableView(TableView *table)
{
    return 10;
}
