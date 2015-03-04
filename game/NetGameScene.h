#ifndef NET_GAMESCENE_H
#define NET_GAMESCENE_H

#include "NetRaceLayer.h"
#include "cocos2d.h"
#include "ui\CocosGUI.h"

#include "./../utils/LogManager.h"

USING_NS_CC;
using namespace cocos2d::ui;

class  NetGameScene : public NetRaceLayer
{
public:
	NetGameScene();
	virtual ~NetGameScene();
private:
    Logger        *_logger;

	int card_seq[TOTAL_CARD_NUM];
	int dist_card_no;
	int aim[3];
	Role *race_role[3];
	Raction *race_action[3];
public:
	virtual bool init();
	void race_start_again();
	void distribute_card_event();
	void init_race_sequence();

	CREATE_FUNC(NetGameScene);
};

#endif