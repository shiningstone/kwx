#ifndef _PLAYER_SEARCH_H
#define _PLAYER_SEARCH_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

#include "EnterRoomScene.h"
#include "network/KwxEnv.h"

class PlayerSearch : public Layer
{
public:
#define FRIEND_SEARCH_BKG					1
#define ID_EDIT_BOX							1
	PlayerSearch(EnterRoom* p);
	~PlayerSearch(void);

	Size visibleSize;
	Vec2 origin;
	virtual bool init();
	void SearchResult(OtherPlayers_Info searchedFriend);
	void Friend_Add_Call(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);
	void Friend_Search_Call(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);
private:
	EnterRoom* parentNode;
	//OtherPlayers_Info searchedFriend;
};
#endif