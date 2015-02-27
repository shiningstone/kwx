#ifndef __NET_RACE_LAYER_H__
#define __NET_RACE_LAYER_H__

#include "NetRaceRound.h"
#include "ui/CocosGUI.h"
#include "account.h"
#include "Role.h"
#include "Resources.h"
using namespace CocosDenshion;

#include "RoundManager.h"
#include "DataBase.h"
#include "./../utils/LogManager.h"

typedef struct {
    PENG,
    GANG,
    TING,
    HU,
}ActionType_t;

class NetRaceLayer : public cocos2d::Layer
{
public:
	NetRaceLayer();
	virtual ~NetRaceLayer();
protected:
    RoundManager  *_roundManager;
private:
    Logger *_logger;
    void _LoadPlayerInfo();
    void _GenerateIds(int ids[3]);
    void _SpeakCard();
    CallFunc *_SpeakAction(ActionType_t id);
    void create_residue_cards();
    void refresh_residue_cards();
    std::string _NumToString( int number );
    void _CalcSingleWinGold(int goldOfPlayer[3], int winner);
    void _CalcDoubleWinGold(int goldOfPlayer[3], int loser);
    void _CalcNoneWinGold(int goldOfPlayer[3], int loser);
    void _CalcHuGold(int goldOfPlayer[3]);
    void CalculateGoldNum(int goldOfPlayer[3],int GoldWinner,int Gold_kind,int who_give);

    void _UpdateGouldAccount(int id,int gold);
    void UpdateGoldAccounts(int goldOfPlayer[3]);
    Vec2 _AnchorOfSign(int dir);
    Vec2 _PositionOfSign(int dir,int size,int origin,int xOffset = 0);
    Vec2 _DestOfSign(int dir,int size,int origin,int xOffset = 0);
    Vec2 _AnchorOfNumber(int dir);
    Vec2 _PositionOfNumber(int dir,int size,int origin);
    Vec2 _DestOfNumber(int dir,int size,int origin);
    Vec2 _AnchorOfGold(int dir);
    Vec2 _PositionOfGold(int dir,int size,int origin,int xOffset = 0);
    Vec2 _DestOfGold(int dir,int size,int origin,int xOffset = 0);
    void GuiJinBiShow(int dir, int gold);
    void GoldNumInsert(int GoldWinner,int Gold_kind,int who_give);

    void _ClockAddTime( Sprite *clock, int time );
    void _Remove(Sprite *parent, int childTag);
public:
	void UpdateClock(int time,int direction);
    void HideClock();


    
private:
    CARD_KIND RototHandOutIndex;
	float s_scale;
	int s_no;
	float residue_no_x,residue_no_y;
	int premiumLeast;//底
	/************************自己手牌全局变量**************************/
	/************************自己手牌全局变量**************************/
	bool ifGameStart;
	bool ifResourcePrepared;

	bool ifTuoGuan;
	int MyCardChoosedNum;
	int cardInList;  //only used in waitfor_MyTouchShowCard
	bool ifChosed;   //only used in waitfor_MyTouchShowCard

	CARD_KIND cur_effect_cardKind;
	CARD_STATUS cur_effect_cardStatus;

	bool ifInsertCardsTime;
	bool ifInsertStopped;
	bool ifEndGameChoose;//是否退出游戏
	bool ifMyShowCardTime;//出牌阶段
	bool ifEffectTime;
	bool ifUpdateDuringEffect;
	bool ifMingTime;
	bool ifMingMybeError;
	//bool ifMyTime;
	//bool ifTriangleHintEnable;
	bool ifTingSignBarVisible;
	unsigned int continue_gang_times;

	Vec2 distributeCardPos;
    
	cocos2d::Point base_point[3];
	cocos2d::Point river_point[3];
	cocos2d::Point lastwinner_point[3];
	cocos2d::Sprite *last_winner;
	cocos2d::Sprite *residue_card_bkg;
    
	cocos2d::Sprite *_playerBkg[3];

	cocos2d::Sprite *left_player_bkg;
	cocos2d::Sprite *right_player_bkg;
	cocos2d::Sprite *my_bkg;

    CCSpriteBatchNode *g_my_free;
	CCSpriteBatchNode *g_my_peng;
	CCSpriteBatchNode *g_my_ming_canPlay;
	CCSpriteBatchNode *g_my_ming_ting;
	CCSpriteBatchNode *g_my_angang;
	CCSpriteBatchNode *g_my_out;
	CCSpriteBatchNode *g_my_in;
	CCSpriteBatchNode *g_my_mask;
	CCSpriteBatchNode *g_my_kou;
	CCSpriteBatchNode *g_left_free;
	CCSpriteBatchNode *g_right_free;
	CCSpriteBatchNode *g_left_right_peng_out;
	CCSpriteBatchNode *g_left_right_in;
	CCSpriteBatchNode *g_left_right_angang;
	CCSpriteBatchNode *g_small_card_kind[21];
	CCSpriteBatchNode *g_small_cardKind_black[21];
	CCSpriteBatchNode *g_mid_card_kind[21];
	CCSpriteBatchNode *g_card_kind[21];
	cocos2d::Size visibleSize;
	cocos2d::Point origin;
	int aim[3];
	CARD_KIND dist_card;
private:
	void AccountShows(LayerColor* BarOfPlayer,int no);
	void AccountHuKind(LayerColor* BarOfPlayer,int num);
	//void show_win_card(account *lastLayer,int no,cocos2d::Vec2 pos,CARD_ARRAY list);
	bool resource_prepare();//码牌准备
	void race_begin_prepare();//牌局开始效果
	Spawn* simple_tip_effect(Vec2 v,std::string act_name);//机器人碰杠胡效果=-=
	void display_callback(cocos2d::Ref* pSender);//功能--看（空）
	void start_callback();//功能--开始
	void restart_touchCallBack(Ref* pSender,ui::Widget::TouchEventType type);
	void start_touchCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void backPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//功能--返回
	void tuoGuanPressed(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);//功能--机器人
	/*######################
	diretion:
				0 left
				1 middle(me)
				2 right
	#######################*/
	void GuiUpdateScore(int direction,int score);//更新分数
	void update_nickname(int direction,std::string str_Nick);//更新昵称
	void update_headimage(int direction,std::string head_photo);//更新头像
    /*###################################*/
	void waitfor_ShowCardWithoutTouch();
	void waitfor_MyTouchShowCard();
	void waitfor_MyShowCardInstruct();
	void start_dealCardsDelete();
	BezierTo* BizerMove1(outCardList* outCard,Vec2 location);
	BezierTo* BizerMove2(outCardList* outCard,Vec2 location,int time);
	BezierTo* OthersBizerMove(int no,outCardList* outCard);
	void collect_resources(HAH *res,CARD_KIND target1[],CARD_KIND target2[],int *len1,int *len2);
	void call_distribute_card();
	void distribute_card_effect();
	void distribute_card();
	void waitfor_otheraction(int no);
	void distribute_event(const std::string event_type,void* val);
	void waitfor_response(Node* sender);
	void first_response(int no);
	void waitfor_myaction(int no);
	void hu_effect_tip(int no);
	void update_outcard(Node *myframe,Vec2 location,int time);
	void choose_and_insert_cards(Node *myframe,CARD_ARRAY *list,int cardInList,Touch* touch,int time);
public:
	int GoldAccountImmediate[3];
	CARD kouCards_kind[4];
	int Kou_kindLen;
	int KouCardsPlace[4][3];

	unsigned int VoiceId;
	virtual void init_race_sequence(){};
	/*##############################
	   everytime when enter game,first call init_role(player) 
	   function,then create_race();
	##############################*/
	void init_role(Role *s_p[],Raction *p_parter[]);//玩家对象初始化
	void create_race();//创建比赛场景
	void update_residue_cards(int no);//更新剩余牌数
	void effect_Distribute_Card(int zhuang);//发牌效果
	/***********callback function***********************/
	void PengPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--碰牌
	void HuPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--胡牌
	void GangPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--杠牌
	void QiPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--弃牌
	void ming_callback(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--名牌
	/***********callback function***********************/
	/*########################
	diretion:
		0 show left indicator
		1 show middle indicator(me)
		2 show right indicator
		-1 show none indicator
	#########################*/
	void delete_act_tip();//删除效果残留
	void delete_ActionRemind();
	void delete_ActionEffect();
	void angang_update(Node *psender);
	void minggang_update(Node *psender);
	void peng_update(Node *psender);
	void PengEffect(Node *psender);//me--碰牌效果
	void an_gang_tip_effect(Node *psender);//me--暗杠效果
	void ming_gang_tip_effect(Node *psender);//me--明杠效果
	void ming_tip_effect(Node *psender);//me--名牌效果
	void hu_tip_effect(Node *psender);//me--胡牌效果
	void qi_tip_effect(Node *psender);//me--弃牌效果
	void startParticleSystem(float delta);//me--例子效果
	void card_list_update(int no);
	void update_card_list(Node *psender);//更新玩家手牌
	void update_card_in_river_list(Node* sender);
	void GoldNumInsert(int GoldWinner,int Gold_kind,int who_give);//金币计算
	void showall();
	void GuiShowReady(int direction);
	//int get_cur_player_no();
	//void set_cur_player_no(int no);
	void raceAccount(float delta);
	void set_aims_sequence(const int p_aim[]);

	void ming_kou_Choose(int no);
	void KouCancle(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void KouEnsure(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void MingCancle(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void KouCardsCheck(int no);
	void GuiUpdateGoldAccounts(int GoldNum[3]);
	void ming_winCards_Hint(Point curPosition);
	void update_residue_TingCards(int no);
	void tingHintCreate(Point curPos,int CardPlace);
	void OtherTingHintBar(int curNo,int CardPlace);
	virtual bool init();
	int Hu_cardOut_place;
	void Back();
	void BackConfirmPressed(Ref* pSender,ui::Widget::TouchEventType type);
	void BackCancelPressed(Ref* pSender,ui::Widget::TouchEventType type);
	void tuoGuanCancelPressed(Ref* pSender,ui::Widget::TouchEventType type);
	void QiangGangHuJudge();
	void TingHintBarListener();

    void distribute_card_event();
    void race_start_again();
public:
	CREATE_FUNC(NetRaceLayer);
};
#endif //	 __RACE_LAYER_H__
