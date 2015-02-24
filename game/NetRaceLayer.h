#ifndef __NET_RACE_LAYER_H__
#define __NET_RACE_LAYER_H__

#include "NetRaceRound.h"
#include "ui/CocosGUI.h"
#include "account.h"
#include "Role.h"
#include "Resources.h"
using namespace CocosDenshion;

#include "RoundManager.h"
#include "./../utils/LogManager.h"

class NetRaceLayer : public cocos2d::Layer
{
public:
	NetRaceLayer();
	virtual ~NetRaceLayer();
protected:
    RoundManager  *_roundManager;
private:
    Logger *_logger;
    
    CARD_KIND RototHandOutIndex;
	float s_scale;
	int s_no;
	volatile CARD_KIND g_show_card;
	float residue_no_x,residue_no_y;
	int last_winner_no;
	int winner_no;
	volatile int cur_player; //0 left,1 me,2 right, 3 server
	int OtherOneForDouble;
	int premiumLeast;//��
	/************************�Լ�����ȫ�ֱ���**************************/
	/************************�Լ�����ȫ�ֱ���**************************/
	bool ifMyActionHint;
	unsigned char temp_actionToDo;
	CARD_KIND cur_effect_cardKind;
	CARD_STATUS cur_effect_cardStatus;
	int MyCardChoosedNum;
	bool ifInsertCardsTime;
	Vec2 distributeCardPos;
	bool ifInsertStopped;
	bool ifEndGameChoose;//�Ƿ��˳���Ϸ
	bool ifMyShowCardTime;//���ƽ׶�
	bool ifEffectTime;
	bool ifUpdateDuringEffect;
	bool ifTuoGuan;
	bool ifMingTime;
	bool ifMingMybeError;
	//bool ifMyTime;
	bool ifGangAsk;
	bool ifGameStart;
	bool ifQiangGangAsk;//˫�����Լ��Ƿ�ѡ���
	bool ifResourcePrepared;
	bool is_last_one;
	int QiangGangTarget;
	bool ifDoubleAsk;
	//bool ifTriangleHintEnable;
	bool ifTingSignBarVisible;
	int FirstMingNo;
	bool ifChosed;
	//ifChosed=true;
	int cardInList;
	Role *player[3];
	outCardList *river_list;
	bool ready_status[3];
	int total_reserved_card_num;
	unsigned char last_action;
	unsigned char last_action_WithGold;//����
	int last_actionSource;
	bool isGangHua;
	unsigned char action_todo;
	unsigned int continue_gang_times;
	int g_server;
	cocos2d::Point base_point[3];
	cocos2d::Point river_point[3];
	cocos2d::Point lastwinner_point[3];
	cocos2d::Sprite *last_winner;
	cocos2d::Sprite *residue_card_bkg;
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
	int card_seq[TOTAL_CARD_NUM];
	CARD_KIND dist_card;
private:
	void AccountShows(LayerColor* BarOfPlayer,int no);
	void AccountHuKind(LayerColor* BarOfPlayer,int num);
	//void show_win_card(account *lastLayer,int no,cocos2d::Vec2 pos,CARD_ARRAY list);
	bool resource_prepare();//����׼��
	void race_begin_prepare();//�ƾֿ�ʼЧ��
	Spawn* simple_tip_effect(Vec2 v,std::string act_name);//���������ܺ�Ч��=-=
	void display_callback(cocos2d::Ref* pSender);//����--�����գ�
	void start_callback();//����--��ʼ
	void restart_touchCallBack(Ref* pSender,ui::Widget::TouchEventType type);
	void start_touchCallBack(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void back_callback(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//����--����
	void robot_callback(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);//����--������
	/*######################
	diretion:
				0 left
				1 middle(me)
				2 right
	#######################*/
	void update_score(int direction,int score);//���·���
	void update_nickname(int direction,std::string str_Nick);//�����ǳ�
	void update_headimage(int direction,std::string head_photo);//����ͷ��
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
	std::string playerSex[3];
	std::string playerLanguage[3];
	virtual void init_race_sequence(){};
	/*##############################
	   everytime when enter game,first call init_role(player) 
	   function,then create_race();
	##############################*/
	void init_role(Role *s_p[],Raction *p_parter[]);//��Ҷ����ʼ��
	void create_race();//������������
	void update_residue_cards(int no);//����ʣ������
	void effect_Distribute_Card(int zhuang);//����Ч��
	/***********callback function***********************/
	void peng_callback(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--����
	void hu_callback(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--����
	void qi_callback(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--����
	void gang_callback(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--����
	void ming_callback(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//me--����
	/***********callback function***********************/
	/*########################
	diretion:
		0 show left indicator
		1 show middle indicator(me)
		2 show right indicator
		-1 show none indicator
	#########################*/
	void update_clock(bool visible,int time,int direction);//���Ӹ���
	void delete_act_tip();//ɾ��Ч������
	void delete_ActionRemind();
	void delete_ActionEffect();
	void angang_update(Node *psender);
	void minggang_update(Node *psender);
	void peng_update(Node *psender);
	void peng_tip_effect(Node *psender);//me--����Ч��
	void an_gang_tip_effect(Node *psender);//me--����Ч��
	void ming_gang_tip_effect(Node *psender);//me--����Ч��
	void ming_tip_effect(Node *psender);//me--����Ч��
	void hu_tip_effect(Node *psender);//me--����Ч��
	void qi_tip_effect(Node *psender);//me--����Ч��
	void startParticleSystem(float delta);//me--����Ч��
	void card_list_update(int no);
	void update_card_list(Node *psender);//�����������
	void update_card_in_river_list(Node* sender);
	void GoldNumInsert(int GoldWinner,int Gold_kind,int who_give);//��Ҽ���
	void showall();
	void ready_indicate(int direction);
	void set_user_default();
	bool get_ready_status(int direction);
	int get_winner_no();
	void set_winner_no(int no);
	int get_last_winner_no();
	//int get_cur_player_no();
	//void set_cur_player_no(int no);
	void raceAccount(float delta);
	void set_reserved_num(int no);
	int get_reserved_num();
	void set_cards_sequence(const int list[]);
	void set_aims_sequence(const int p_aim[]);

	void ming_kou_Choose(int no);
	void KouCancle(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void KouEnsure(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void MingCancle(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void KouCardsCheck(int no);
	void updatePropertyImmediate(int GoldNum[3]);
	void update_UserDefault(int no);
	void get_user_default();
	void ming_winCards_Hint(Point curPosition);
	void update_residue_TingCards(int no);
	void tingHintCreate(Point curPos,int CardPlace);
	void OtherTingHintBar(int curNo,int CardPlace);
	virtual bool init();
	int Hu_cardOut_place;
	void BackChoose();
	void BackChooseEnsure(Ref* pSender,ui::Widget::TouchEventType type);
	void BackChooseCancel(Ref* pSender,ui::Widget::TouchEventType type);
	void tuoGuanCancelFunc(Ref* pSender,ui::Widget::TouchEventType type);
	void QiangGangHuJudge();
	void TingHintBarListener();
	void delete_NnnecessaryResource();

	int dist_card_no;
	Role *race_role[3];
	Raction *race_action[3];
    void distribute_card_event();
    void race_start_again();
public:
	CREATE_FUNC(NetRaceLayer);
};
#endif //	 __RACE_LAYER_H__
