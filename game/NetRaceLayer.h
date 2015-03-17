#ifndef __NET_RACE_LAYER_H__
#define __NET_RACE_LAYER_H__

#include "ui/CocosGUI.h"
using namespace CocosDenshion;

#include "NetRaceRound.h"
#include "account.h"
#include "Role.h"
#include "Resources.h"

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"
#include "./../multimedia/VoiceEffect.h"
#include "./../multimedia/GameLayout.h"
#include "./../multimedia/GraphicObject.h"
#include "./../multimedia/GraphicEffect.h"
#include "DataBase.h"

class RoundManager;
class NetRaceLayer : public cocos2d::Layer
{
public:
	NetRaceLayer();
	virtual ~NetRaceLayer();
protected:
    RoundManager   *_roundManager;
    VoiceEffect    *_voice;
    GameLayout     *_layout;
    GraphicEffect  *_effect;
    GObjectFactory *_object;
private:
    Logger *_logger;

    void create_residue_cards();
    void refresh_residue_cards();
    std::string _NumToString( int number );
    void _CalcAnGangGold(int winner,int goldOfPlayer[3]);
    void _CalcMingGangGold(int winner,int loser,int goldOfPlayer[3]);
    void _CalcSingleWinGold(int goldOfPlayer[3], int winner);
    void _CalcDoubleWinGold(int goldOfPlayer[3], int loser);
    void _CalcNoneWinGold(int goldOfPlayer[3], int loser);
    void _CalcHuGold(int goldOfPlayer[3]);
    void CalculateGoldNum(int goldOfPlayer[3],int GoldWinner,int Gold_kind,int who_give);

    void DispatchAction(Node *psender,ARRAY_ACTION action);
    Sprite *_GetEffectCardInHand(Node *myframe, int i,CARD_KIND value );
    void _UpdateGouldAccount(int id,int gold);
    void UpdateGoldAccounts(int goldOfPlayer[3]);
    void GuiJinBiShow(int dir, int gold);
    void GoldNumInsert(int GoldWinner,int Gold_kind,int who_give);
    void _GuiUpdateScore(LayerColor *layer,int score);

    void _ClockAddTime( Sprite *clock, int time );
    void _Remove(Node *parent, int childTag);
    bool _IsClickedOn(Node* button,Touch* touch);
    void _Show(Node *parent, int childTag,bool flag);

    /*Ming*/
    Node *_NonKouMask(Sprite *card);
    void    MaskNonKouCards();
    void    ListenToKou(int no);
    TargetedAction *ShowBigMing(Node *myframe);
    bool _CardTouchBegan(Touch* touch, Event* event);
    void _CardTouchMove(Touch* touch, Event* event);
    void _CardTouchEnd(Touch* touch, Event* event);
    void ListenToCardTouch();
    void _CollectResouce(HAH *res) ;

    void _CreateGangCardsMotion(TargetedAction *motions[4]);
    void _CreateGangCardInHandMotion(TargetedAction *motions[4],int cardInHand[4],CARD_KIND kind);

    void _AttachKindTexture(Sprite *parent,CARD_KIND kind);
    void _AttachKindTextureToFreeCard(Sprite *parent,CARD_KIND kind);
    void _CreateMingGangCardsMotion(TargetedAction *mostions[4],CARD_KIND kind);
    void _CreateMingGangCardInHandMotion(TargetedAction *motions[3], int idxInHand[3], CARD_KIND kind);
    void _CreateBackgroundElementMotion(TargetedAction *motions[5],int gangType);
    Sequence *_HideReminder(int reminderTag, double lastingTime, double shadeScale);
    Sequence *_HideQiReminder();
    void _CreateFreeCard(Sprite *cards[3], int idxInHand[3], CARD_KIND kind);

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
	/*if use*/
	//int Angang[4];


	bool ifGameStart;
	bool ifResourcePrepared;

	bool ifTuoGuan;
	int  touched;  //only used in waitfor_MyTouchShowCard
	bool ifChosed;   //only used in waitfor_MyTouchShowCard
	int MyCardChoosedNum;

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

	Vec2 distributeCardPos;
    
	cocos2d::Sprite *_zhuangSign;
	cocos2d::Sprite *residue_card_bkg;
    
	cocos2d::Sprite *left_player_bkg;
	cocos2d::Sprite *right_player_bkg;
	cocos2d::Sprite *my_bkg;

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
    Vec2 _getLastCardPosition(PlayerDir_t dir) ;
	void call_distribute_card();
	void distribute_card_effect();
	void ListenToDistributeCard();
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

    void _kouCardsRecordClear();
    void _kouCardsRecordAdd(CARD_KIND kind, int idx[]);
    bool _kouCardsRecordIncludes(CARD_KIND kind);
    int  _findCards(int idx[],CARD_ARRAY *list,CARD_KIND kind);

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
    void _4CardsDistribute(int sequenceNo, float delayRef);
	void effect_Distribute_Card(int zhuang);//发牌效果
	/***********callback function***********************/
    void PengPressed(Button *button,PlayerDir_t prevPlayer,Card_t card);
	void BtnPengHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--碰牌
    void HuPressed(Button *button, bool qiangGang, bool doubleHu);
	void BtnHuHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--胡牌
    void GangPressed(Button *button, Card_t card, int gangCardIdx[], bool isAnGang=true, PlayerDir_t prevPlayer=MIDDLE);
	void BtnGangHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--杠牌
	void BtnQiHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--弃牌
	void MingPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--名牌
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
    
	void angang_dispatch(Node *psender);
	void minggang_dispatch(Node *psender);
	void peng_dispatch(Node *psender);

    void PengEffect(PlayerDir_t dir, PlayerDir_t prevDir, Card_t card);
	void an_gang_tip_effect(int no,Card_t card,int gang[]);//me--暗杠效果
	void ming_gang_tip_effect(int no,PlayerDir_t prevDir, Card_t card,int gang[]);//me--明杠效果
	void ming_tip_effect(Node *psender);//me--名牌效果
	void hu_tip_effect(Node *psender);//me--胡牌效果
	void qi_tip_effect(Node *psender);//me--弃牌效果
	void startParticleSystem(float delta);//me--例子效果
	void card_list_update(int no);
	void update_card_list(Node *psender);//更新玩家手牌
	void update_card_in_river_list(Node* sender);
	void showall();
	void GuiShowReady(int direction);
	//int get_cur_player_no();
	//void set_cur_player_no(int no);
	void raceAccount(float delta);
	void set_aims_sequence(const int p_aim[]);

	void ming_kou_Choose(int no);
	void KouCancelPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void KouConfirmPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void MingCancelPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
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
	void ListenToTingButton();

    void distribute_card_event();
    void race_start_again();
public:
	CREATE_FUNC(NetRaceLayer);
};
#endif //	 __RACE_LAYER_H__
