
#include "Coordinate.h"
#include "GameLayout.h"

void GameLayout::init(Coordinate *coordinate) {
    _coordinate = coordinate;

    float s_scale=1.189;
    int s_no=1;

	Sprite *sprite = Sprite::create((s_no==1)?"racetable.png":"racetable2.png");
	sprite->setPosition(_coordinate->Center());
	sprite->setScaleX(s_scale);
	sprite->setScaleY(s_scale);
	this->addChild(sprite, 0);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameprepareImage.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tools.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("userhead.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("xzdd_prepare_pop_res.plist");
    
	auto center_bkg = Sprite::createWithSpriteFrameName("kawuxing.png");
	center_bkg->setPosition(_coordinate->Center());
	this->addChild(center_bkg,1,CENTER_BKG_TAG_ID);
}
/*************************************
    singleton
*************************************/
GameLayout *GameLayout::_instance = NULL;

GameLayout *GameLayout::getInstance() {
    if (_instance==NULL) {
        _instance = new GameLayout();
    }

    return _instance;
}

void GameLayout::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

