
#include "Coordinate.h"
#include "GameLayout.h"

GameLayout::GameLayout(cocos2d::Layer *employer) {
    _employer = employer;
}

void GameLayout::init(Coordinate *coordinate) {
    _coordinate = coordinate;

    int   s_no=1;
    _addTable(s_no);

	_cache("gameprepareImage.plist");
	_cache("tools.plist");
	_cache("userhead.plist");
	_cache("xzdd_prepare_pop_res.plist");
    
    _addBackground();
}

void GameLayout::_addTable(int option) {
    float s_scale=1.189;

	Sprite *sprite = Sprite::create((option==1)?"racetable.png":"racetable2.png");
	sprite->setPosition(_coordinate->Center());
	sprite->setScaleX(s_scale);
	sprite->setScaleY(s_scale);
	_employer->addChild(sprite, 0);
}

void GameLayout::_addBackground() {
	auto center_bkg = Sprite::createWithSpriteFrameName("kawuxing.png");
	center_bkg->setPosition(_coordinate->Center());
	_employer->addChild(center_bkg,1,CENTER_BKG_TAG_ID);
}

void GameLayout::_cache(const char *file) {
	SpriteFrame_cache::getInstance()->addSpriteFramesWithFile(file);
}

/*************************************
    singleton
*************************************/
GameLayout *GameLayout::_instance = NULL;

GameLayout *GameLayout::getInstance(cocos2d::Layer *employer) {
    if (_instance==NULL) {
        _instance = new GameLayout(employer);
    }

    return _instance;
}

void GameLayout::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

