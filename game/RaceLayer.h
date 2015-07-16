#ifndef __RACE_LAYER_H__
#define __RACE_LAYER_H__

#include "ui/CocosGUI.h"
using namespace CocosDenshion;

#include "./../account.h"
#include "./../Resources.h"

#include "GameType.h"

#include "./../utils/LogManager.h"
#include "./../multimedia/VoiceEffect.h"
#include "./../multimedia/GameLayout.h"
#include "./../multimedia/GraphicObject.h"
#include "./../multimedia/GraphicEffect.h"
#include "DataBase.h"
#include "CardCollection.h"

typedef struct {
    PlayerDir_t target;
    int         cardsLen;
    Card_t      newCard;
    int         remain;
}DistributeInfo_t;

class RoundManager;
class RaceLayer : public cocos2d::Layer
{
public:
	RaceLayer();
	virtual ~RaceLayer();
    
    void DoubleWin(const WinInfo_t &win);
    void SingleWin(const WinInfo_t &win);
    void GangGoldEffect(int winner,int whoGive);
    void KouCancelEffect(ActionId_t action,CardInHand *cards);
    void KouConfirmEffect();
    void MingCancelEffect();
	void QueryKouCards();
	void QueryMingOutCard();
    void _SwitchCancelBtn(int tag);

    void QueryGangCards();
	void ListenToGangChoose(int no);
	bool _GangChooseBegan(Touch* touch, Event* event);
	void _GangChooseEnded(Touch* touch, Event* event);
	void _GangChooseUpdate();

    void HideActionButtons();
    void ShowActionButtons(int mask);
    Vec2 GetCardPositionInHand(int idx);
    void OthersHandoutEffect(PlayerDir_t dir,bool canKou);//其它玩家出牌效果
protected:
    RoundManager   *_roundManager;
    VoiceEffect    *_voice;
    GameLayout     *_layout;
    GraphicEffect  *_effect;
    GObjectFactory *_object;
private:
    
    typedef struct {
        PlayerDir_t curPlayer;
        int         count;
    }TimerInfo_t;

    TimerInfo_t    _timer;

    float _AddBtnQi(const Vec2 &ref);
    float _AddBtnHu(const Vec2 &ref);
    float _AddBtnMing(const Vec2 &ref);
    float _AddBtnGang(const Vec2 &ref);
    float _AddBtnPeng(const Vec2 &ref);
    
    const static int DIST_BATCHES     = 4;   /*card number of distribute batch*/
    const static int DIST_BATCH_CARDS = 4;   /*card number of distribute batch*/

    Logger *_logger;

    LayerColor* myframe;

    Sprite *_GetCardInHand(PlayerDir_t dir,int idx);
    void    _ReOrderCardsInHand(int droppedCard,CardInHand *cards);
    int     _FindCard(int start,int end,Touch *touch);
    TargetedAction *_OthersShowCardEffect(PlayerDir_t dir,Card_t outCard,bool canKou);//其它玩家出牌的效果
    Spawn *_CreateHuBackgroundEffect(PlayerDir_t dir);
    
    void _OthersMingGangEffect(PlayerDir_t dir,PlayerDir_t prevDir,bool isNewDistributed,Card_t card = CARD_UNKNOWN);
    void _UpdateNonChosenCards(const CardInHand *cards, int chosen);
    void _CancelChosenCardInHand();
    void _UpdateCardsInHand(const CardInHand *cards, int chosen);
    void _ShowMenuButtons();
    void _CreateMeneButtons();
    void _CreateHeadImage();
    void _ShowZhuang(PlayerDir_t dir);
    bool _ResourcePrepare();
    void _StartParticleSystem(float delta);
    void ListenToDoubleHu();
    LabelAtlas * _CreateNumberSign(int number);
    Sprite* _GetCardOnTingSignBar(PlayerDir_t dir,int cardIdx);
    CardStatus_t _GetAppearance(const CardList *cards,int idx);
    Sprite *_CreateCardInHand(PlayerDir_t dir,int idx,CardList *cards,bool isTing,const Vec2 &refer);
    float _YofNextCard(PlayerDir_t dir,int idx,CardList *cards,bool isTing,float refY);

    void _CreateResidueCards();
    void _InitResidueCards();
    std::string _NumToString( int number );

    void DispatchAction(Node *psender,ARRAY_ACTION action);
    Sprite *_CreateEffectCard(int i,CARD_KIND value );
    void _GuiUpdateScore(LayerColor *layer,int score);

    void _ClockAddTime( Sprite *clock, int time );
    void _Remove(Node *parent, int childTag);
    void _Add(Node *parent, Node *child, int childTag,int zOrder);
    bool _IsClickedOn(Node* button,Touch* touch);
    void _Show(Node *parent, int childTag,bool flag);

    /*IsMing*/
    bool _KouTouchBegan(Touch* touch, Event* event);
    int _FindChosenGroup(Touch *touch,Sprite *cardsInHand[]);
    void _KouTouchEnded(Touch* touch, Event* event);
    void MaskNon(CardStatus_t status);
    void ListenToKou(int no);
    TargetedAction *_ShowBigMing(Node *myframe);
    
    bool _CardTouchBegan(Touch* touch, Event* event);
    void _CardTouchMove(Touch* touch, Event* event);
    void _CardTouchEnd(Touch* touch, Event* event);
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
    void ShowPlayerMes(PlayerDir_t dir,UserProfile_t newPlayer);
    void GuiPlayerShow(PlayerDir_t dir);		//显示玩家信息
	void HidePlayerMes(PlayerDir_t dir);			//删除玩家信息
	void BtnPlayerReady(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//游戏准备//ceshi_yusi
	//void GuiShowReady(int direction);         //已有的准备状态显示


    void GuiJinBiShow(PlayerDir_t dir, int gold);

	void start_timer(int time,PlayerDir_t direction);
    void UpdateClock(float dt);
    void HideClock();
    void ListenToCardTouch();
    
private:
	float s_scale;
	int s_no;
	float residue_no_x,residue_no_y;
	const int PREMIUM_LEAST;   //底
	/************************自己手牌全局变量**************************/
	/************************自己手牌全局变量**************************/
	/*if use*/
	//int Angang[4];


	bool ifGameStart;
	bool _isResoucePrepared;

	bool ifTuoGuan;
    
	bool ifChosed;   //only used in ListenToCardTouch
	int  _myTouchedCard;  //only used in ListenToCardTouch
    int  _myChosenCard;

	CARD_KIND cur_effect_cardKind;
	CARD_STATUS cur_effect_cardStatus;

	bool ifInsertCardsTime;
	bool _isCardInHandUpdated;
	bool ifEndGameChoose;//是否退出游戏
	bool ifEffectTime;
	bool ifUpdateDuringEffect;
	//bool ifPengAction;
	//bool ifGangAction;
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
	CARD_KIND dist_card;
private:
    Label *_CreateName(const char *name);
    Sprite *_CreateHeadImage(const char *file);
    Sprite *_CreateGoldImage();
    LabelAtlas *_CreatePropertyNumber(int number,Sprite *gold);
    Sprite *_CreatePropertyUnit(int number,LabelAtlas *label);
    Sprite *_CreateHu();
    Sprite *_CreateZiMo();
    Sprite *_CreateFangPao();
    Sprite *_CreateBaozhuang();
    void _CreateAccountPanel(const UserProfile_t &profile, Node *parent);
    Sprite *_CreateSymbol(PlayerDir_t dir,int gold,LayerColor *parent);
    LabelAtlas *_CreatePropertyChange(PlayerDir_t dir,int gold,LayerColor *parent);
    void _ExposeCards(PlayerDir_t dir,const WinInfo_t &win,LayerColor *parent);
	void AccountShows(LayerColor* BarOfPlayer,int no);
	void AccountHuKind(LayerColor* BarOfPlayer,int num);
	//void show_win_card(account *lastLayer,int no,cocos2d::Vec2 pos,CARD_ARRAY list);
	void _RaceBeginPrepare();//牌局开始效果
	Spawn* simple_tip_effect(Vec2 v,std::string act_name);//机器人碰杠胡效果=-=
	void display_callback(cocos2d::Ref* pSender);//功能--看（空）
	void BtnRestartHandler(Ref* pSender,ui::Widget::TouchEventType type);
	void BtnStartHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void BtnBackHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);//功能--返回
	void BtnTuoGuanHandler(cocos2d::Ref* pSender,ui::Widget::TouchEventType type);//功能--机器人
	/*######################
	diretion:
				0 left
				1 middle(me)
				2 right
	#######################*/
	void _UpdateNickName(int direction,std::string str_Nick);//更新昵称
	void _UpdateHeadImage(int direction,std::string head_photo);//更新头像
    /*###################################*/
	void _DeleteStartDealCards();
	BezierTo* BizerMove1(CardList* river,Vec2 location);
	BezierTo* BizerMove2(CardList* river,Vec2 location,int time);
	BezierTo* OthersBizerMove(int no,CardList* river);
    Vec2 _GetLastCardPosition(PlayerDir_t dir,int cardLen) ;
	void ListenToDistributeCard();
	void WaitForFirstAction(PlayerDir_t zhuang);
	void _MyHandoutEffect(Card_t outCard,Vec2 location,int time,bool turnToMing=false);
public:
	void StartGame();//功能--开始
	void GuiUpdateScore(int direction,int score);//更新分数
	void _DistributeEvent(const std::string event_type,void* val);
    void _PengEffect(PlayerDir_t dir, PlayerDir_t prevDir, Card_t card);
	void _HuEffect(const WinInfo_t &win);
	void _DistributeCard(const DistributeInfo_t &distribute);
	void MyHandoutEffect(int cardInList,Vec2 touch,int time,bool turnToMing);
	int GoldAccountImmediate[3];

	unsigned int VoiceId;
	virtual void init_race_sequence(){};
	/*##############################
	   everytime when enter game,first call init_role(player) 
	   function,then CreateRace();
	##############################*/
	void CreateRace(GameMode_t mode);//创建比赛场景
    void reload();
	void _UpdateResidueCards(int no);//更新剩余牌数
    void _RightBatchDistribute(int batchIdx, float delayRef, int cardLen=13);
    void _LeftBatchDistribute(int batchIdx, float delayRef, int cardLen=13);
	void FirstRoundDistributeEffect(PlayerDir_t zhuang);//发牌效果
    Sequence *_FisrtRoundResidueUpdate();
	/***********callback function***********************/
    void QiEffect();
    void PengEffect(PlayerDir_t dir, PlayerDir_t prevDir, Card_t card);
    void HuEffect(const WinInfo_t &win,bool qiangGang);
    void GangEffect(PlayerDir_t winner,Card_t card, int gangCardIdx[], bool isAnGang=true, PlayerDir_t prevPlayer=MIDDLE);
	void BtnMingHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
    
	void BtnPengHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void BtnHuHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void BtnGangHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void BtnQiHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	
	/***********callback function***********************/
	/*########################
	diretion:
		0 show left indicator
		1 show middle indicator(me)
		2 show right indicator
		-1 show none indicator
	#########################*/
	void _DeleteActionTip();//删除效果残留
	void _DeleteActionReminder();
	void _DeleteActionEffect();
    void hide_action_tip(ActionId_t action);

    TargetedAction* _MingAnimation();
	void _AnGangEffect(PlayerDir_t dir,Card_t card,int gang[]=NULL);//me--暗杠效果
	void _MingGangEffect(PlayerDir_t dir,PlayerDir_t prevDir, Card_t card,int gang[]=NULL);//me--明杠效果
	void _QiEffect(PlayerDir_t dir);//me--弃牌效果
	void _CardInHandUpdateEffect(PlayerDir_t dir);
	void _CardRiverUpdateEffect(PlayerDir_t dir);
	void showall();
    void GuiHideReady();
	void GuiShowReady(int direction);
	//int get_cur_player_no();
	//void set_cur_player_no(int no);
	void raceAccount(float delta);

    void BtnGangCancelHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
    void BtnGangConfirmHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);

	void BtnKouCancelHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void BtnKouConfirmHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void MingCancelHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type);
	void GuiUpdateGoldAccounts(int GoldNum[3]);
	void ming_winCards_Hint(Point curPosition);
	void _UpdateTingNum(PlayerDir_t dir);
	void _TingHintCreate(Point curPos,int CardPlace);
	void TingHintBarOfOthers(int curNo,int CardPlace);
	virtual bool init();
	int Hu_cardOut_place;
	void Back();
	void BtnBackConfirmHandler(Ref* pSender,ui::Widget::TouchEventType type);
	void BtnBackCancelHandler(Ref* pSender,ui::Widget::TouchEventType type);
	void BtnTuoGuanCancelHandler(Ref* pSender,ui::Widget::TouchEventType type);
	void ListenToTingButton();

public:
	CREATE_FUNC(RaceLayer);
};
#endif //	 __RACE_LAYER_H__
