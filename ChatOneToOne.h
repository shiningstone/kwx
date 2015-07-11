#ifndef CHAT_ONE_TO_ONE_H
#define CHAT_ONE_TO_ONE_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

#include "LoginType.h"

#include "network/KwxEnv.h"
#include "EnterRoomScene.h"

class ChatOneToOne : public Layer
{
public:
#define CHATRECORD_BTN				1
#define COMMON_BTN					2
#define CHAT_EDIT_BOX				3
#define CHAT_LOG_VIEW				4
#define KEY_MIDDLE_WORD				"_MES_"
#define LOCAL_MY_MES				"LoCaL_mYmEs"
#define LOCAL_OTHER_MES				"LoCaL_tOmEs"
	ChatOneToOne(EnterRoom* p,Friend_Info curFriend);
	~ChatOneToOne(void);

	Size visibleSize;
	Vec2 origin;
	virtual bool init();
	void getLocalChatLog();//获取本地聊天记录
	std::string Message_local_save(std::string curMes,bool ifMySend);//消息本地存储
	void ChatBtnCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type);//聊天窗口切换按钮
	void MessageSendCallBack(cocos2d::Ref* pSender,Widget::TouchEventType type);//消息发送

	void receiveChatMessage(std::string receiveStr);//收到消息
	Layout* Message_cell_create(std::string message);
	void RowAndColumn(Size labelSize,int &row,int &column);
private:
	EnterRoom* parentNode;
	Friend_Info chatFriend;
	std::vector<string> chatLog;
	Size chatSize;
};

#endif