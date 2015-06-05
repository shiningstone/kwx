
#include <stdlib.h>

#include "cocos2d.h"
USING_NS_CC;

#include "./../protocol/CommonMsg.h"
#include "./../protocol/KwxMessenger.h"

#include "CardCollection.h"
#include "Player.h"
#include "RaceLayer.h"
#include "RoundManager.h"
#include "NetRoundManager.h"
#include "StrategyRm.h"


NetRoundManager::NetRoundManager(RaceLayer *uiManager)
:RoundManager(uiManager) {
    _MODE = NETWORK_GAME;

    _uiManager = uiManager;
    StrategyRm::destroyInstance();
    _strategy  = StrategyRm::getInstance(this);

    _lastWin.winner = INVALID_DIR;
    _lastWin.giver  = INVALID_DIR;

	_gRiver = new CardList;
	for(int i=0;i<TOTAL_CARD_NUM;i++) {
		_unDistributedCards[i] = CARD_UNKNOWN;
	}
    _distributedNum = 0;

    for(int i=0;i<PLAYER_NUM;i++) {
        _players[i] = NULL;
    }

    _HandoutNotify = false;
    _waiting = REQ_INVALID;

    _messenger = new KwxMessenger();
    _logger = LOGGER_REGISTER("NetRoundManager");
}

NetRoundManager::~NetRoundManager() {
    delete _gRiver;
    for(int i=0;i<PLAYER_NUM;i++) {
        delete _players[i];
    }

    delete _messenger;
    LOGGER_DEREGISTER(_logger);
}

/***********************************************
        player information
***********************************************/
#include "Player.h"

void NetRoundManager::InitPlayers() {
	_players[0] = new Player(LEFT);
	_players[1] = new Player(MIDDLE);
	_players[2] = new Player(RIGHT);

    UserProfile_t profile[PLAYER_NUM] = {{0}};
    _strategy->load_profiles(profile);

    for(int dir=0;dir<PLAYER_NUM;dir++) {   
        _players[dir]->Set(&profile[dir]);
    }
}

/****************************************
       networks
****************************************/
void NetRoundManager::HandleMsg(void * aMsg) {
    auto di = static_cast<DsInstruction *>(aMsg);

    switch(di->request) {
        case REQ_GAME_SEND_START:
            _DiRecv((GameStartResponse *)di);
            break;
        case REQ_GAME_RECV_START:
            _DiRecv((GameStartNotif *)di);
            break;
        case REQ_GAME_DIST_BEGINCARDS:
            _DiRecv((FirstDistZhuang *)di);
            break;
        case REQ_GAME_DIST_BEGINCARDS_OTHER:
            _DiRecv((FirstDistNonZhuang *)di);
            break;
        case REQ_GAME_DIST_CARD_TOOTHER:
            _DiRecv((DistCardNotif *)di);
            break;
        case REQ_GAME_DIST_CARD:
            _DiRecv((DistCardInfo *)di);
            break;
        case REQ_GAME_SEND_SHOWCARD:
            _DiRecv((ShowCardResponse *)di);
            break;
        case REQ_GAME_RECV_SHOWCARD:
            _DiRecv((ShowCardNotif *)di);
            break;
        case REQ_GAME_DIST_REMIND:
            _DiRecv((RemindInfo *)di);
            break;
        case REQ_GAME_SEND_ACTION:
            _DiRecv((ActionResponse *)di);
            break;
        case REQ_GAME_RECV_ACTION:
            _DiRecv((ActionNotif *)di);
            break;
        case REQ_GAME_DIST_DECISION:
            _DiRecv((DecisionNotif *)di);
            break;
        case REQ_GAME_DIST_HU_CALCULATE:
            _DiRecv((HuInfoNotif *)di);
            break;

        /*********************************************
            �ƾ��޹�REQUEST
        *********************************************/
        case REQ_GAME_SEND_ENTER:
            _DiRecv((EnterRoomResponse *)di);
            break;
        case REQ_GAME_RECV_ENTER:
            _DiRecv((EnterRoomNotif *)di);
            break;
        case REQ_GAME_DIST_DAOJISHI:
            _DiRecv((CounterNotif *)di);
            break;
        case REQ_GAME_SEND_CALSCORE:
            _DiRecv((ScoreNotif *)di);
            break;
        default:
            LOGGER_WRITE("%s undefined request code %d\n",__FUNCTION__,di->request);
            break;
    }
}

void NetRoundManager::UpdateCards(PlayerDir_t dir,ARRAY_ACTION action,Card_t actKind) {
    if(dir==MIDDLE) {
        RoundManager::UpdateCards(dir,action,actKind);
    } else {
        if(action==aPENG) {
            _isNewDistributed = false;
        }
        
        if(_actCtrl.decision==aAN_GANG) {
            _players[dir]->others_action(_isNewDistributed,aAN_GANG,actKind);
        } else if(_actCtrl.decision==aSHOU_GANG) {
            _players[dir]->others_action(_isNewDistributed,aSHOU_GANG,actKind);
        } else {
            _players[dir]->others_action(_isNewDistributed,(ActionId_t)action,actKind);
        }
    }
}

void NetRoundManager::ServerWaitForMyAction() {
    _uiManager->ShowActionButtons(_actCtrl.choices);

	if(_actCtrl.choices!=0) {
		_actCtrl.decision = aNULL;
	}

	if(_isNewDistributed) {
        _players[MIDDLE]->_cards->_IncludingOthersCard = false;
	}
}

void NetRoundManager::ServerDistributeTo(PlayerDir_t dir,Card_t card) {
    if(_distributedNum<TOTAL_CARD_NUM+1) {
        _isNewDistributed  = true;

        DistributeInfo_t distInfo;
        
        distInfo.target   = dir;
        distInfo.cardsLen = _players[dir]->_cards->size();
        distInfo.newCard  = card;
        distInfo.remain   = TOTAL_CARD_NUM - _distributedNum;
        
        _uiManager->_DistributeEvent(DISTRIBUTE_DONE_EVENT_TYPE,&distInfo);
    } else {
		_uiManager->_DistributeEvent(NOONE_WIN_EVENT_TYPE,NULL);
    }
}

/****************************************
       main interface
****************************************/
void NetRoundManager::CreateRace(RaceLayer *uiManager) {
    _uiManager = uiManager;
    InitPlayers();
	_isGameStart=false;

    SeatInfo *seat = SeatInfo::getInstance();
    seat->Set(1,0x10010000,1,1);

    _messenger->StartReceiving();
    
    /**********************************************/
    /* enter room should be called somewhere else */
    RequestEnterRoom aReq;
    aReq.Set();
    _messenger->Send(aReq);
    /**********************************************/
}

void NetRoundManager::StartGame() {
	_isGameStart=false;
    
    RequestGameStart aReq;
    aReq.Set();
    _messenger->Send(aReq);
}

Card_t NetRoundManager::RecvPeng(PlayerDir_t dir) {
    Card_t kind = LastHandout();
    bool actionPermit = false;
    
    if(dir==MIDDLE) {
        _SendAction(aPENG,kind);
        actionPermit = Wait(REQ_GAME_SEND_ACTION);
    }

    if( dir==MIDDLE && !actionPermit ) {
        _uiManager->hide_action_tip(aPENG);
        return CARD_UNKNOWN;
    } else {
        _players[dir]->_cards->push_back(kind);
        _players[dir]->_cards->_IncludingOthersCard = true;
        return RoundManager::RecvPeng(dir);
    }
}

void NetRoundManager::RecvHu(PlayerDir_t dir) {
    if(dir==MIDDLE) {
        _SendAction(aHU);
    }
}

Card_t NetRoundManager::RecvGangConfirm(PlayerDir_t dir) {
    CardInHand *cards   = _players[dir]->_cards;
    int*        gangIdx = new int[4];
    ActionId_t  action  = aNULL;
    Card_t      card;
    bool        actionPermit = false;

    card = cards->_alter->get_activated_cards(gangIdx,&action);
    cards->_alter->clear();

    SetDecision(dir,action);

    if(dir==MIDDLE) {
        _SendAction(_actCtrl.decision,card);
        actionPermit = Wait(REQ_GAME_SEND_ACTION);
    }

    if( dir==MIDDLE && !actionPermit ) {
        _uiManager->hide_action_tip(aGANG);
        return CARD_UNKNOWN;
    } else {
        if(_actCtrl.decision==aAN_GANG || _actCtrl.decision==aSHOU_GANG) {
            
            if( !IsMing(dir) ) {
                SetEffectCard(card,sAN_GANG);
            }
        
            _uiManager->GangEffect(dir,card,gangIdx);
        } else {
            PlayerDir_t prevPlayer = (PlayerDir_t)_curPlayer;
        
            if(!_isNewDistributed) {
                _players[_curPlayer]->_river->pop_back();
        
                RecordOutCard(card);
                RecordOutCard(card);
                RecordOutCard(card);
        
                TurnTo(dir);
            }else {
                RecordOutCard(card);
            }
        
            _uiManager->GangEffect(dir,card,gangIdx,false,prevPlayer);
        }
        
        return card;
    }
}

void NetRoundManager::RecvQi() {
    _SendAction(aQi);
    
    SetDecision(MIDDLE,aQi);
    _uiManager->QiEffect();
}

void NetRoundManager::_NotifyHandout() {
    if(_HandoutNotify) {
        _HandoutNotify = false;
        
        if(_actCtrl.choices && _actCtrl.decision==aQi) {
            _SendAction(aQi);
            Wait(REQ_GAME_DIST_DAOJISHI);
        }
        
        RequestShowCard aReq;
        aReq.Set(LastHandout());
        _messenger->Send(aReq);
    }
}

void NetRoundManager::RecvHandout(int chosen,Vec2 touch,int mode) {
    _actCtrl.handoutAllow = false;

    _HandoutNotify = true;
    
	if(_isMingTime) {
		_isMingTime      = false;
        _actCtrl.choices = 0;
	} else {
        if(_actCtrl.decision==aMING) {
            _actCtrl.decision = aQi;
        }
    }

	_actCtrl.decision = aQi;

    RecordOutCard(_players[MIDDLE]->_cards->get_kind(chosen));
    //_players[MIDDLE]->hand_out(chosen);

    bool turnToMing = false;
	if(_actCtrl.decision==aMING && !IsMing(_curPlayer) ) {
        _players[_curPlayer]->_cards->set_ming(chosen);

        turnToMing = true;
    }

    _uiManager->MyHandoutEffect(chosen,touch,mode,turnToMing);
}

void NetRoundManager::RecvKouCancel() {
    _players[MIDDLE]->_cards->_alter->clear();

    _SendAction(aKOU_CANCEL);

    _uiManager->KouCancelEffect(aKOU,_players[MIDDLE]->_cards);
}

void NetRoundManager::RecvKouConfirm() {
    CardInHand *cards = _players[MIDDLE]->_cards;

    for(int i=cards->FreeStart;i<cards->size();i++) {
        if(cards->get_status(i)==sKOU_ENABLE)
            cards->set_status(i,sFREE);
    }   
    
    UpdateCards(MIDDLE,a_KOU);
    _players[MIDDLE]->_strategy->scan_ming(_gRiver);

    Card_t kinds[4];
    int    kindNum = _players[MIDDLE]->_cards->_alter->get_activated_kinds(kinds);
    _SendAction(aKOU,kindNum,kinds);

    _uiManager->KouConfirmEffect();
}

void NetRoundManager::RecvMingCancel() {
    _SendAction(aMING_CANCEL);

    UpdateCards(MIDDLE,a_KOU_CANCEL);/*BUG HERE???*/
    
    _isMingTime=false;
    _actCtrl.decision = aQi;
    
    _players[MIDDLE]->_cards->cancel_ming();
    /*!!!BUG MAYBE HERE : should clear MingInfo_t of cardsInHand*/

    _uiManager->MingCancelEffect();
}

void NetRoundManager::RecvMing(bool isFromKouStatus) {
    _actCtrl.decision = aMING;
    _players[MIDDLE]->_strategy->scan_ming();

    _isMingTime=true;
        
    if(!isFromKouStatus) {
        _players[MIDDLE]->_strategy->scan_kou();
        
        if(_players[MIDDLE]->_cards->_alter->group_num()>0) {
            _uiManager->QueryKouCards();
        } else {
            _SendAction(aMING);
            
            UpdateCards(MIDDLE,a_MING);
            _uiManager->QueryMingOutCard();
        }
    } else {
        _SendAction(aMING);
        
        UpdateCards(MIDDLE,a_MING);
        _uiManager->QueryMingOutCard();
    }
}

void NetRoundManager::WaitForMyAction() {
}

void NetRoundManager::WaitForOthersAction(PlayerDir_t dir) {
}

void NetRoundManager::WaitForOthersChoose() {
}

void NetRoundManager::WaitForResponse(PlayerDir_t dir) {
	_NotifyHandout();
}

void NetRoundManager::DistributeTo(PlayerDir_t dir,Card_t card) {
}

void NetRoundManager::ActionAfterGang(PlayerDir_t dir) {
}

void NetRoundManager::WaitForFirstAction(PlayerDir_t zhuang) {
    _isGameStart = true;

    _curPlayer = zhuang;
    if(zhuang==MIDDLE) {
        ServerWaitForMyAction();
        _actCtrl.handoutAllow = true;
    } else {
        WaitForOthersAction((PlayerDir_t)zhuang);
    }
}


/*************************************
        response wait
*************************************/
void NetRoundManager::Resume(DsInstruction *di) {
    if(_waiting==di->request) {
        HandleMsg(di);
        _waiting  = REQ_INVALID;
    }
}

bool NetRoundManager::Wait(RequestId_t req) {
    _waiting      = req;
    _permited     = false;
    
    while(_waiting!=REQ_INVALID) {
        _delay(100);
    }
    
    return _permited;
}

/*************************************
        server requests handlers
*************************************/
int NetRoundManager::_SendAction(ActionId_t action,Card_t card) {
    RequestSendAction aReq;
    aReq.Set(action,card);
    return _messenger->Send(aReq);
}

int NetRoundManager::_SendAction(ActionId_t code,int kindNum,Card_t kinds[]) {
    RequestSendAction aReq;
    aReq.Set(code,kindNum,kinds);
    return _messenger->Send(aReq);
}

void NetRoundManager::_order_small_to_big(Card_t kinds[],int num) {
    for(int i=0;i<num-1;i++) {
        for(int j=i+1;j<num;j++) {
            if(kinds[i]>kinds[j]) {
                Card_t temp = kinds[i];
                kinds[i] = kinds[j];
                kinds[j] = temp;
            }
        }
    }
}

void NetRoundManager::_restoreRemindInfo(const Reminds_t &remind) {
    _serverReminds = remind;

    _order_small_to_big(_serverReminds.kouCard,_serverReminds.kouKindNum);
    _order_small_to_big(_serverReminds.gangCard,_serverReminds.gangKindNum);
    
    _players[_curPlayer]->_cards->_alter->load_kou_info(remind.kouCard,remind.kouKindNum);
    _players[_curPlayer]->_cards->_alter->load_gang_info(remind.actions,remind.gangCard,remind.gangKindNum);
    _players[_curPlayer]->_cards->load_ming_info(remind.ming);
}

void NetRoundManager::_DiRecv(GameStartResponse *info) {
    _players[MIDDLE]->_isReady = true;
    _uiManager->GuiShowReady(MIDDLE);
    LOGGER_WRITE("NOTE: Player%d's score should set to %d\n",MIDDLE,info->score);
    delete info;
}

void NetRoundManager::_DiRecv(GameStartNotif *info) {
    if(_players[info->seat]->_isReady==false) {
        _players[info->seat]->_isReady = true;
        _uiManager->GuiShowReady(info->seat);
    }
    LOGGER_WRITE("NOTE: Player%d's score should set to %d\n",info->seat,info->score);
    delete info;
}

void NetRoundManager::_DiRecv(FirstDistZhuang *info) {
    _uiManager->GuiHideReady();
    
    Shuffle();

    Card_t cards[14];
    for(int i=0;i<14;i++) {
        cards[i] = info->cards[i];
    }
    PlayerDir_t dir = (PlayerDir_t)info->seat;
    INT8U timer     = info->timer;
    
    _actCtrl.choices = info->remind.actions;
    
    _players[_curPlayer]->init(cards,14,aim[MIDDLE]);//玩家手牌初始�?
    _players[(_curPlayer+1)%3]->init(&(_unDistributedCards[14]),13,aim[(MIDDLE+1)%3]);
    _players[(_curPlayer+2)%3]->init(&(_unDistributedCards[27]),13,aim[(MIDDLE+2)%3]);

    _restoreRemindInfo(info->remind);

    delete info;

	_uiManager->FirstRoundDistributeEffect(MIDDLE);//牌局开始发牌效果�?
    _uiManager->start_timer(timer,MIDDLE);
}

void NetRoundManager::_DiRecv(FirstDistNonZhuang *info) {
    _uiManager->GuiHideReady();

    Card_t cards[13];
    for(int i=0;i<13;i++) {
        cards[i] = info->cards[i];
    }
    PlayerDir_t me     = (PlayerDir_t)info->seat;
    PlayerDir_t zhuang = (PlayerDir_t)info->zhuang;
    INT8U timer        = info->timer;
    delete info;

    _curPlayer = zhuang;
    PlayerDir_t other  = (zhuang==LEFT)?RIGHT:LEFT;
    
    _players[zhuang]->init(&(_unDistributedCards[0]),14,aim[zhuang]);//玩家手牌初始�?
    _players[MIDDLE]->init(cards,13,aim[MIDDLE]);
    _players[other]->init(&(_unDistributedCards[27]),13,aim[other]);

	_uiManager->FirstRoundDistributeEffect(zhuang);//牌局开始发牌效果�?
    _uiManager->start_timer(timer,(PlayerDir_t)_curPlayer);
}

void NetRoundManager::_DiRecv(DistCardNotif *info) {
    PlayerDir_t target = (PlayerDir_t)info->seat;
    Card_t card        = (Card_t)info->kind;
    INT8U timer        = info->timer;
    _distributedNum    = info->remain;
    delete info;

    ServerDistributeTo(target,card);
    _players[target]->hand_in(card,false,IsMing(target),(_distributedNum==TOTAL_CARD_NUM));
    _uiManager->start_timer(timer,target);
}

void NetRoundManager::_DiRecv(DistCardInfo *info) {
    PlayerDir_t target = (PlayerDir_t)info->seat;
    Card_t card        = info->kind;
    INT8U timer        = info->timer;
    _distributedNum    = info->remain;

    PlayerDir_t prev  = (PlayerDir_t)_curPlayer;
    _curPlayer        = MIDDLE;
    _isNewDistributed = true;
    
    _actCtrl.choices = info->remind.actions;
    _actCtrl.target  = info->kind;

    _restoreRemindInfo(info->remind);

    delete info;

    ServerDistributeTo(target,card);
    _players[MIDDLE]->hand_in(card,false,IsMing(prev),(_distributedNum==TOTAL_CARD_NUM));
    ServerWaitForMyAction();
    _actCtrl.handoutAllow = true;

    _uiManager->start_timer(timer,target);
    _uiManager->ListenToCardTouch();
}

void NetRoundManager::_DiRecv(ShowCardResponse *info) {
    LOGGER_WRITE("%s handout ret = %d",__FUNCTION__,info->status);

    if(info->ting.kindNum>0) {

    }
    
    delete info;

    _actCtrl.choices = 0;
}

void NetRoundManager::_DiRecv(ShowCardNotif *info) {
    PlayerDir_t dir    = (PlayerDir_t)info->seat;
    Card_t card        = (Card_t)info->kind;

    if(info->ting.kindNum>0) {

    }
    
    delete info;

    RecordOutCard(card);
    
    _actCtrl.decision = aQi;
    
    _curPlayer = dir;

    _players[dir]->_cards->pop_back();
    _players[dir]->_river->push_back(card);
        
	_isNewDistributed = false;

    _uiManager->OthersHandoutEffect(dir,false);
}

void NetRoundManager::_DiRecv(RemindInfo *info) {
    PlayerDir_t dir = (PlayerDir_t)info->seat;
    PlayerDir_t whoGive = (PlayerDir_t)info->whoGive;
    INT8U timer     = info->timer;
    Card_t kind     = info->kind;
    
    _actCtrl.choices = info->remind.actions;
    _actCtrl.target  = info->kind;
    
    _restoreRemindInfo(info->remind);

    delete info;

    if(whoGive==SERVER || whoGive==dir) {
        _isNewDistributed = true;
    } else {
        _isNewDistributed = false;
    }

    ServerWaitForMyAction();
}

void NetRoundManager::_DiRecv(ActionResponse *info) {
    PlayerDir_t dir  = (PlayerDir_t)info->seat;
    PlayerDir_t wait = (PlayerDir_t)info->waitSeat;
    delete info;

    if(wait!=RIGHT && wait!=LEFT) {
        _permited = true;
    } else {
        _permited = false;
    }
}

void NetRoundManager::_DiRecv(ActionNotif *info) {
    PlayerDir_t dir     = (PlayerDir_t)info->seat;
    PlayerDir_t whoGive = (PlayerDir_t)info->whoGive;
    Card_t card         = info->card[0].kind;
    
    _actCtrl.target   = card;
    _actCtrl.decision = (ActionId_t)info->actions;
    _actCtrl.choices  = _actCtrl.decision;

    if(_actCtrl.decision==aMING) {
        _curPlayer = dir;
        
        CardInHand *cards = _players[dir]->_cards;
        cards->clear();
        
        for(int i=0;i<info->cardNum;i++) {
            CardNode_t *node = new CardNode_t;
            *node = info->card[i];
            cards->push_back( node );
        }

        cards->IsMing = true;

        _uiManager->_CardInHandUpdateEffect(dir);
    }
    
    delete info;

    switch(_actCtrl.decision) {
        case aPENG:
            _curPlayer = whoGive;
            RecvPeng(dir);
            break;
        case aMING_GANG:
            {
                if(!_isNewDistributed) {
                    _curPlayer = whoGive;
                    
                    _players[whoGive]->_river->pop_back();
			        RecordOutCard(card);
			        RecordOutCard(card);
			        RecordOutCard(card);

                    _players[dir]->_cards->push_back(LastHandout());
                } else {
                    _curPlayer = dir;
                }

                SetDecision(dir,aGANG);
                _uiManager->_MingGangEffect(dir,whoGive,card);
            }
            break;
		case aSHOU_GANG:
        case aAN_GANG:
            {
                SetDecision(dir,aGANG);
                _uiManager->_AnGangEffect(dir,card);
            }
            break;
        case aHU:

            
            break;
    }
}

void NetRoundManager::_UpdateWin(HuInfo_t *player) {
    int winnerNum = 0;
    int winner = 0;
    int loser = 0;
    
    for(int i=0;i<PLAYER_NUM;i++) {
        if( player[i].status==ZI_MO ) {
            winner = i;
            winnerNum = 1;
        } else if( player[i].status==HU_PAI ) {
            winner = i;
            winnerNum++;
        } else if( player[i].status==DIAN_PAO ) {
            loser = i;
        }
    }

    if(winnerNum==1) {
        SetWin(SINGLE_WIN,winner);
    } else {
        SetWin(DOUBLE_WIN,loser);
    }
}

void NetRoundManager::_DiRecv(HuInfoNotif *info) {
    _UpdateWin(info->hu);

    for(int i=0;i<PLAYER_NUM;i++) {
        _players[i]->refresh(info->hu[i].card, info->hu[i].cardNum, info->hu[i].fan);
    }

    delete info;

    _uiManager->HuEffect(_lastWin, _isQiangGangAsking);
}

void NetRoundManager::_DiRecv(EnterRoomResponse *info) {
    LOGGER_WRITE("NOTE: profile of players should be updated here\n");
    LOGGER_WRITE("Start heartbeats\n");
    KwxHeart::getInstance();
}

void NetRoundManager::_DiRecv(EnterRoomNotif *info) {
    LOGGER_WRITE("NOTE: profile of players should be updated here\n");
}

void NetRoundManager::_DiRecv(CounterNotif *info) {
    _uiManager->start_timer(info->count,(PlayerDir_t)info->seat);

    if(info->seat==MIDDLE) {
        WaitForMyChoose();
        _permited = true;
    }
}

void NetRoundManager::_DiRecv(ScoreNotif *info) {
    UpdateGold((int *)info->val);
}

/*************************************
        singleton
*************************************/
RoundManager *NetRoundManager::getInstance() {
    if (_instance==NULL) {
        _instance = new NetRoundManager(NULL);
    }

    return _instance;
}

void NetRoundManager::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

