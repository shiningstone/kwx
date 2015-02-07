#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class ActivityLayer :public cocos2d::Layer, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
	ActivityLayer(void);
	~ActivityLayer(void);
	virtual bool init();
	CREATE_FUNC(ActivityLayer);
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
};

