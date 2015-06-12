
#include <stdlib.h>

#include "CardCollection.h"
#include "Player.h"
#include "RaceLayer.h"
#include "RoundManager.h"
#include "StrategyRm.h"

#include "Player.h"

/***********************************************
        initialization
***********************************************/
RoundManager::RoundManager(RaceLayer *uiManager) {
    _MODE = LOCAL_GAME;
    TIME_LIMIT = 20;

    _uiManager = uiManager;
    _strategy  = StrategyRm::getInstance(this);

    _lastWin.winner = INVALID_DIR;
    _lastWin.giver  = INVALID_DIR;

	_gRiver = new CardList;
    _distributedNum = 0;

    for(int i=0;i<PLAYER_NUM;i++) {
        _players[i] = NULL;
    }

    _logger = LOGGER_REGISTER("RoundManager");
}

RoundManager::~RoundManager() {
    delete _gRiver;
    for(int i=0;i<PLAYER_NUM;i++) {
        delete _players[i];
    }
    
    LOGGER_DEREGISTER(_logger);
}

void RoundManager::InitPlayers() {
	_players[LEFT]   = new Player(LEFT);
	_players[MIDDLE] = new Player(MIDDLE);
	_players[RIGHT]  = new Player(RIGHT);

    UserProfile_t profile[PLAYER_NUM] = {{0}};
    _strategy->load_profiles(profile);

    for(int dir=0;dir<PLAYER_NUM;dir++) {   
        _players[dir]->_isExist = true;
        _players[dir]->Set(&profile[dir]);
    }
}

int RoundManager::Shuffle() {
#ifndef USE_TEST_ROUND
    _strategy->load_card_sequence(_unDistributedCards);
#else
    load_test_round(1,_unDistributedCards);
#endif

    _gRiver->clear();
    _distributedNum = 0;
    _LogRoundCards();
    
    _isQiangGangAsking = false;
    _isDoubleHuAsking = false;
    _isNewDistributed = true;
    _firstMingNo = INVALID;
    _qiangGangTargetNo = INVALID;
    _otherOneForDouble = INVALID;
    _isGangHua = false;
    _actCtrl.handoutAllow = false;
    _isTuoGuan = false;
    
    CancelEffectCard();
    _isMingTime = false;

	_continue_gang_times=0;
    _lastActionWithGold = aQi;
    _lastActionSource = INVALID;

    _curPlayer = GetLastWinner();

    return 0;
}


/****************************************
       main interface
****************************************/
void RoundManager::CreateRace(RaceLayer *uiManager) {
	_isGameStart=false;
    _uiManager = uiManager;
    InitPlayers();
}

void RoundManager::StartGame() {
	_isGameStart=false;
    
    _players[MIDDLE]->_isReady = true;
	_uiManager->GuiShowReady(MIDDLE);

    Shuffle();

    int lastWinner = GetLastWinner();
    _prevPlayer = (PlayerDir_t)lastWinner;
    _actCtrl.choices = _players[(lastWinner)%3]->init(&(_unDistributedCards[0]),14,aim[lastWinner]);//鐜╁鎵嬬墝鍒濆鍖?
	_players[(lastWinner+1)%3]->init(&(_unDistributedCards[14]),13,aim[(lastWinner+1)%3]);
	_players[(lastWinner+2)%3]->init(&(_unDistributedCards[27]),13,aim[(lastWinner+2)%3]);

	_uiManager->FirstRoundDistributeEffect((PlayerDir_t)lastWinner);//鐗屽眬寮�濮嬪彂鐗屾晥鏋溿�?
}

/***********************************************************
    玩家动作处理
***********************************************************/
Card_t RoundManager::RecvPeng(PlayerDir_t dir) {
    SetDecision(dir,aPENG);

    if(dir==MIDDLE) {
        _actCtrl.handoutAllow = true;
    }
    
    Card_t pengCard = _players[_curPlayer]->_river->get_kind(_players[_curPlayer]->_river->last());
    _players[_curPlayer]->_river->pop_back();

    RecordOutCard(pengCard);
    RecordOutCard(pengCard);

    TurnTo(dir);

    _uiManager->PengEffect((PlayerDir_t)_curPlayer,_prevPlayer,pengCard);

	return pengCard;
}

void RoundManager::RecvHu(PlayerDir_t dir) {
    SetDecision(dir,aHU);
    
    if(_isDoubleHuAsking) {
        SetWin(DOUBLE_WIN,_curPlayer);
    } else {
        SetWin(SINGLE_WIN,dir);
    }

    _uiManager->HuEffect(_lastWin, _isQiangGangAsking);
}

Card_t RoundManager::RecvGangConfirm(PlayerDir_t dir) {
    CardInHand *cards   = _players[dir]->_cards;
    int*        gangIdx = new int[4];
    ActionId_t  action  = aNULL;
    Card_t      card;

    card = cards->_alter->get_activated_cards(gangIdx,&action);
    cards->_alter->clear();

    SetDecision(dir,action);

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

Card_t RoundManager::RecvGang(PlayerDir_t dir) {
    if(_players[dir]->_cards->_alter->group_num()>1 && dir==MIDDLE) {
        _uiManager->QueryGangCards();
        return CARD_UNKNOWN;
    } else {
        _players[dir]->_cards->_alter->switch_status(0);
        return RecvGangConfirm(dir);
    }
}

void RoundManager::RecvQi() {
    if(_players[MIDDLE]->_cards->_IncludingOthersCard) {
        _players[MIDDLE]->_cards->pop_back();
    }

    _uiManager->QiEffect();
}

void RoundManager::RecvHandout(int idx,Vec2 touch,int mode) {
    _actCtrl.handoutAllow = false;

	if(_isMingTime) {
		_isMingTime=false;
	} else {
        if(_actCtrl.decision==aMING) {
            _actCtrl.decision = aQi;
        }
    }
    
    bool turnToMing = false;
	if(_actCtrl.decision==aMING && !IsMing(_curPlayer) ) {
        _players[_curPlayer]->_cards->set_ming(idx);
        turnToMing = true;
    }

    RecordOutCard(_players[_curPlayer]->_cards->get_kind(idx));
    //_players[_curPlayer]->hand_out(idx);

    _uiManager->MyHandoutEffect(idx,touch,mode,turnToMing);
}

void RoundManager::RecvKouCancel() {
    _players[MIDDLE]->_cards->_alter->clear();
    _uiManager->KouCancelEffect(aKOU,_players[MIDDLE]->_cards);
}

void RoundManager::RecvKouConfirm() {
    CardInHand *cards = _players[MIDDLE]->_cards;

    for(int i=cards->FreeStart;i<cards->size();i++) {
        if(cards->get_status(i)==sKOU_ENABLE)
            cards->set_status(i,sFREE);
    }   
    
    UpdateCards(MIDDLE,a_KOU);
    _players[MIDDLE]->_strategy->scan_ming(_gRiver);

    _uiManager->KouConfirmEffect();
}

void RoundManager::RecvMingCancel() {
    _isMingTime=false;
    
    UpdateCards(MIDDLE,a_KOU_CANCEL);
    _actCtrl.decision = aQi;
    
    _players[MIDDLE]->_cards->cancel_ming();
    /*!!!BUG MAYBE HERE : should clear MingInfo_t of cardsInHand*/

    _uiManager->MingCancelEffect();
}

void RoundManager::RecvMing(bool isFromKouStatus) {
	_actCtrl.decision = aMING;

    if(!isFromKouStatus) {
        _players[_curPlayer]->_strategy->scan_ming(_gRiver);
    }

    if(_curPlayer==MIDDLE) {
        if(!isFromKouStatus) {
            _players[MIDDLE]->_strategy->scan_kou();

            if(_players[MIDDLE]->_cards->_alter->group_num()>0) {
                _uiManager->QueryKouCards();
            } else {
                _isMingTime=true;
                UpdateCards(MIDDLE,a_MING);
                
                _uiManager->QueryMingOutCard();
            }
        } else {
            _isMingTime=true;
            UpdateCards(MIDDLE,a_MING);
            
            _uiManager->QueryMingOutCard();
        }
    } else {
        WaitForOthersChoose();
    }
}

void RoundManager::ForceHandout() {
    if(_players[MIDDLE]->_cards->_IncludingOthersCard) {
        RecvQi();
    } else {
        int last = _players[MIDDLE]->_cards->last();
        
        Vec2 location = _uiManager->GetCardPositionInHand(last);
        RecvHandout(last,location,2);
        
        _prevPlayer = MIDDLE;
    }
}

/*****************************************************
    对当前玩家的杠牌，其他玩家可以判断是否抢杠胡
*****************************************************/
void RoundManager::QiangGangHuJudge(PlayerDir_t target,Card_t kind) {
    _qiangGangTargetNo = target;

	PlayerDir_t  no1 = RightOf(target);
    ActionMask_t action1 = _players[no1]->hand_in(kind,false,_players[target]->_cards->IsMing,false);

	PlayerDir_t  no2 = RightOf(no1);
	ActionMask_t action2 = _players[no2]->hand_in(kind,false,_players[target]->_cards->IsMing,false);

	if((action1&a_HU)&&(action2&a_HU)) {
        if(target!=MIDDLE) {
            _actCtrl.choices   = aHU;
            _otherOneForDouble = (no1==MIDDLE) ? no2 : no1;
        }

        _isDoubleHuAsking  = true;
        _lastActionWithGold = aQIANG_GANG;

        WinInfo_t win;
        win.kind  = DOUBLE_WIN;
        win.giver = target;
        _uiManager->DoubleWin(win);
	} else if(action1&a_HU||action2&a_HU) {
        _actCtrl.choices   = aHU;

        _isQiangGangAsking = true;
        _lastActionWithGold = aQIANG_GANG;
        
        WinInfo_t win;
        win.kind   = SINGLE_WIN;
        win.winner = (action1&a_HU) ? no1 : no2;
        win.giver  = target;
        _uiManager->SingleWin(win);
	} else {
        _uiManager->GangGoldEffect(target,target);
	}
}

void RoundManager::WaitForFirstAction(PlayerDir_t zhuang) {
    _isGameStart = true;

    _curPlayer = zhuang;
    //_actCtrl.choices = _players[_curPlayer]->judge_action_again();

    _isNewDistributed = true;
    if(zhuang==MIDDLE) {
        WaitForMyAction();
    } else {
        WaitForOthersAction((PlayerDir_t)zhuang);
    }
}

void RoundManager::WaitForMyAction() {
    _uiManager->start_timer(TIME_LIMIT,MIDDLE);

	if(_actCtrl.choices!=0) {
        _uiManager->ShowActionButtons(_actCtrl.choices);
	}

	if(_isNewDistributed) {
		WaitForMyChoose();
	}
}

void RoundManager::WaitForMyChoose() {
	if(_isNewDistributed) {/* is this judgement neccessary??? */
		if( (_isTuoGuan) || (IsMing(MIDDLE) && !player_can_gang()) ) {
            ForceHandout();
		} else {
			_actCtrl.handoutAllow = true;
        }
	}
}

void RoundManager::WaitForOthersAction(PlayerDir_t dir) {
    _uiManager->start_timer(TIME_LIMIT,dir);

    if(_actCtrl.choices&aHU) {
        RecvHu(dir);
    } else if(player_can_gang()) {
        RecvGang(dir);
    } else if(_actCtrl.choices&aMING) {
        RecvMing();
    } else if(_actCtrl.choices&aPENG) {
        RecvPeng(dir);
    } else {
        WaitForOthersChoose();
    }
}

void RoundManager::WaitForOthersChoose() {
    int index    = INVALID;
    bool canKou  = false;

    index = _players[_curPlayer]->_strategy->choose_card(_actCtrl.decision,canKou);

    if(_actCtrl.decision==aMING) {
        UpdateCards((PlayerDir_t)_curPlayer,(ARRAY_ACTION)aMING);
    }
    
    if ( canKou ) {
        Card_t handingout = _players[_curPlayer]->_cards->get_kind(index);
        _players[_curPlayer]->_cards->_alter->active_all(handingout);
    }

    RecordOutCard(_players[_curPlayer]->_cards->get_kind(index));
	_players[_curPlayer]->hand_out(index);

    if( canKou ) {
        /* it is dangerous to raise these lines to upper, since the following will change the card list*/
        if(_players[_curPlayer]->_cards->_alter->group_num()>0)
            UpdateCards((PlayerDir_t)_curPlayer,a_KOU);
    }

    _uiManager->TingHintBarOfOthers(_curPlayer,index);

	_isNewDistributed = false;

    _uiManager->OthersHandoutEffect((PlayerDir_t)_curPlayer,canKou);
}

void RoundManager::WaitForResponse(PlayerDir_t dir) {
    if(_isNewDistributed) {
        _HandleCardNewDistributed(dir);
    } else {
        _HandleCardFrom(dir);
    }
}

void RoundManager::WaitForTuoGuanHandle() {
    if(_actCtrl.choices!=0) {
        if(!_isNewDistributed) {
            _players[MIDDLE]->_cards->pop_back();

            if(_isQiangGangAsking) {
                _isQiangGangAsking=false;
                _strategy->update_gold((PlayerDir_t)_qiangGangTargetNo,MING_GANG,(PlayerDir_t)_curPlayer);
                _qiangGangTargetNo=INVALID;
                DistributeTo((PlayerDir_t)_curPlayer,(Card_t)(_unDistributedCards[_distributedNum++]));
            } else if(_isDoubleHuAsking) {
                _isDoubleHuAsking = false;
                RecvHu(MIDDLE);
            } else {
                DistributeTo(TurnTo(NEXT),(Card_t)(_unDistributedCards[_distributedNum++]));
            }
        } else {
            WaitForMyChoose();
        }
    } else {
        if(_actCtrl.handoutAllow) {
            SetDecision(MIDDLE,aQi);
            WaitForMyChoose();
        }
    }
}

ActionMask_t RoundManager::GetPlayerChoices(PlayerDir_t dir,bool prevMing) {
    ActionMask_t actions = 
        _players[dir]->hand_in(
            LastHandout(),
            _isNewDistributed,
            prevMing,
            (_distributedNum==TOTAL_CARD_NUM)
        );
    
    if(dir==MIDDLE&&_isTuoGuan) {
        if(IsMing(MIDDLE)&&(actions&a_HU)) {
            actions=a_HU;
        } else {
            actions=a_JUMP;
        }
    } else if(dir!=MIDDLE) {
        if(_players[dir]->_cards->is_aim_limit(actions,LastHandout())) {
            actions = a_JUMP;
        }
    }

    if(actions==a_JUMP) {
        _players[dir]->_cards->pop_back();
		_players[dir]->_cards->_IncludingOthersCard = false;
    }

    return actions;
}

bool RoundManager::player_can_gang() {
    if(_actCtrl.choices&a_AN_GANG  || _actCtrl.choices&a_MING_GANG || _actCtrl.choices&a_SHOU_GANG) {
        return true;
    } else {
        return false;
    }
}

void RoundManager::_HandleCardNewDistributed(PlayerDir_t dir) {
    _isGangHua=false;
    
    if(_lastActionSource==dir&&_continue_gang_times!=0) {
        _isGangHua=true;
    } else {
        _actCtrl.choices  = 0;
        _actCtrl.decision = aQi;
        _continue_gang_times=0;
    }
    
    _actCtrl.choices = 
        _players[dir]->hand_in(
            NewDistribute(),
            _isNewDistributed,
            _players[_prevPlayer]->_cards->IsMing,
            (_distributedNum==TOTAL_CARD_NUM)
        );
    LOGGER_WRITE("%s handin %s",DescPlayer(dir),DescCard(NewDistribute()));
    
    if((PlayerDir_t)dir==MIDDLE) {
        if(IsMing(MIDDLE)&&(_actCtrl.choices&a_HU)){
            RecvHu(MIDDLE);
        }else{
            WaitForMyAction();
        }
    } else {
        if(_players[dir]->_cards->is_aim_limit(_actCtrl.choices,LastHandout())) {
            _actCtrl.decision = aQi;
        }
        WaitForOthersAction(dir);
    }
}

void RoundManager::_HandleCardFrom(PlayerDir_t dir) {
    bool prevMingStatus = IsMing(dir);
    
    PlayerDir_t  no1 = RightOf(dir);
    ActionMask_t action1 = GetPlayerChoices(no1,prevMingStatus);
    
    PlayerDir_t  no2 = RightOf(no1);
    ActionMask_t action2 = GetPlayerChoices(no2,prevMingStatus);
    
    if((action1&a_HU)&&(action2&a_HU))
    {
        _uiManager->HideClock();
        
        if((no1!=MIDDLE&&no2!=MIDDLE) || (no1==MIDDLE||no2==MIDDLE&&IsMing(MIDDLE))){ 
            SetWin(DOUBLE_WIN,(PlayerDir_t)_curPlayer);
            _uiManager->_HuEffect(_lastWin);
            _uiManager->_DistributeEvent(DOUBLE_HU_WITH_ME,NULL);
        } else if((no1==MIDDLE||no2==MIDDLE) && !IsMing(MIDDLE)) {
            _isDoubleHuAsking = true;
            if(no1==1) {
                _actCtrl.choices = action1;
                _otherOneForDouble = no2;
            } else {
                _actCtrl.choices = action2;
                _otherOneForDouble = no1;
            }                   
            WaitForMyAction();
            return;
        }
    } else if(action1&a_HU||action2&a_HU)//鐐圭偖
    {
        _uiManager->HideClock();
        
        if((no1==1&&(action1&a_HU))||(no2==1&&(action2&a_HU))) {
            if(_players[1]->_cards->IsMing) {
                RecvHu(MIDDLE);
            } else {
                if(no1==1)
                    _actCtrl.choices=action1;
                else
                    _actCtrl.choices=action2;
                WaitForMyAction();
                return;
            }
        }
        else if(no1!=1&&(action1&a_HU)) {
            RecvHu((PlayerDir_t)no1);
        }
        else if(no2!=1&&(action2&a_HU)) {
            RecvHu((PlayerDir_t)no2);
        }
    }
    else if(action1!=a_JUMP)//涓嬪
    {
        _actCtrl.choices=action1;
        if(no1==1)
        {
            WaitForMyAction();
            return;
        }
        else
        {
            WaitForOthersAction(no1);
            return;
        }
    }
    else if(action2!=a_JUMP)//涓婂
    {
        _actCtrl.choices=action2;
        if(no2==1)
        {
            WaitForMyAction();
            return;
        }
        else
        {
            WaitForOthersAction(no2);
            return;
        }
    }
    else if(action1==action2&&action1==a_JUMP)
    {
        _actCtrl.decision = aQi;

        DistributeTo(TurnTo(NEXT),(Card_t)(_unDistributedCards[_distributedNum++]));
    }
}

void RoundManager::DistributeTo(PlayerDir_t dir,Card_t card) {
    if(_distributedNum<TOTAL_CARD_NUM+1) {
        _uiManager->start_timer(TIME_LIMIT,dir);

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

void RoundManager::ActionAfterGang(PlayerDir_t dir) {
    if(_isNewDistributed) {
        QiangGangHuJudge(dir,LastHandout());
    } else {
        DistributeTo(TurnTo(dir),_unDistributedCards[_distributedNum++]);
    }
}

void RoundManager::UpdateCards(PlayerDir_t dir,ARRAY_ACTION action,Card_t actKind) {
    if(action==a_AN_GANG) {
        _players[dir]->action(_isNewDistributed,aAN_GANG,actKind);
    } else if(action==a_SHOU_GANG) {
        _players[dir]->action(_isNewDistributed,aSHOU_GANG);
    } else {
        _players[dir]->action(_isNewDistributed,(ActionId_t)action,actKind);
    }
}

void RoundManager::SetDecision(PlayerDir_t dir,ActionId_t act) {
    if(act==aGANG) {
        if(_actCtrl.choices & aAN_GANG) {
            _actCtrl.decision = aAN_GANG;
        } else if(_actCtrl.choices & aSHOU_GANG) {
            _actCtrl.decision = aSHOU_GANG;
        } else {
            _actCtrl.decision = aMING_GANG;
        }

        _continue_gang_times++;
        _lastActionWithGold = _actCtrl.decision;
    } else if(act==aHU) {
        _actCtrl.decision = act;

        if(_isQiangGangAsking) {
            _lastActionWithGold = aQIANG_GANG;
        }
    } else if(act==aMING_GANG || act==aAN_GANG || act==aSHOU_GANG) {
        _actCtrl.decision = act;
        _continue_gang_times++;
        _lastActionWithGold = _actCtrl.decision;
    } else {
        _actCtrl.decision = act;
        _continue_gang_times = 0;
        _lastActionWithGold = aQi;
    }

    _lastActionSource = dir;

    _actCtrl.choices = 0;
}

void RoundManager::UpdateGold(int gold[PLAYER_NUM]) {
    for(int i=0;i<PLAYER_NUM;i++) {
        Database *database = Database::getInstance();
        int total = _players[i]->UpdateProperty(gold[i]);
        database->SetProperty(_players[i]->_profile.id, total);
    }

    for(int id=0;id<PLAYER_NUM;id++) {
        _uiManager->GuiUpdateScore(id,_players[id]->_profile.property);
        _uiManager->GuiJinBiShow((PlayerDir_t)id,gold[id]);        
    }
}

void RoundManager::update_gold(PlayerDir_t GoldWinner,GoldKind_t Gold_kind,PlayerDir_t whoGive) {
    _strategy->update_gold(GoldWinner,Gold_kind,whoGive);
}

void RoundManager::get_ending_gold(int gold[PLAYER_NUM]) {
    _strategy->get_ending_gold(gold);
}

/*****************************
    context
*****************************/
Card_t RoundManager::LastHandout() const {
    return _gRiver->get_kind(_gRiver->last());
}

Card_t RoundManager::NewDistribute() const {
    return (Card_t)_unDistributedCards[_distributedNum-1];
}

/***********************************************
        winner information
***********************************************/
PlayerDir_t RoundManager::GetLastWinner() {
    if( _lastWin.winner==INVALID_DIR ) {
        LOGGER_WRITE("NETWORK: Request(last winner) not defined");
        _lastWin.winner = MIDDLE;
    }
    return _lastWin.winner;
}

void RoundManager::SetWin(WinKind_t kind,int player) {
    _lastWin.kind       = kind;

    if(kind==DOUBLE_WIN) {
        _lastWin.winner = INVALID_DIR;
        _lastWin.giver  = (PlayerDir_t)player;
    } else {
        _lastWin.winner = (PlayerDir_t)player;
        _lastWin.giver  = (PlayerDir_t)_curPlayer;
    }
}

const WinInfo_t &RoundManager::GetWin() {
    return _lastWin;
}

bool RoundManager::IsWinner(int no) {
	if((_lastWin.kind==SINGLE_WIN&&
                ((_lastWin.winner==_curPlayer && _lastWin.winner!=no)
                ||(_lastWin.winner!=_curPlayer && no!=_lastWin.winner && no!=_curPlayer)))
        ||(_lastWin.kind==NONE_WIN && _firstMingNo!=INVALID && no!=_firstMingNo)) {
        return true;
    } else {
        return false;
    }
}

/***********************************************
        general operations
***********************************************/
bool RoundManager::IsMing(int id) const{
    return _players[id]->_cards->IsMing;
}

PlayerDir_t RoundManager::RightOf(PlayerDir_t dir) const {
    return (PlayerDir_t)((dir+1)%PLAYER_NUM);
}

PlayerDir_t RoundManager::TurnTo(PlayerDir_t dir) {
    _prevPlayer = (PlayerDir_t)_curPlayer;

    if(dir==NEXT) {
        _lastActionSource = INVALID;
        _curPlayer  = RightOf(_prevPlayer);
    } else {
        _curPlayer  = dir;
    }

    return (PlayerDir_t)_curPlayer;
}

/***********************************************
        river information
***********************************************/
void RoundManager::RecordOutCard( Card_t kind ) {
    _gRiver->push_back(kind);
    _gRiver->show();
}

/****************************************
        before start
****************************************/
void RoundManager::set_aims_sequence(const int p_aim[]) {
    LOGGER_WRITE("%s",__FUNCTION__);
	for(int i=0;i<3;i++)
		aim[i]=p_aim[i];
}

/****************************************
        effect card
****************************************/
void RoundManager::CancelEffectCard() {
    _curEffectCard.kind = ck_NOT_DEFINED;
    _curEffectCard.status = c_NOT_DEFINDED;
}

void RoundManager::SetEffectCard(int kind,int status) {
    _curEffectCard.kind = (CARD_KIND)kind;
    _curEffectCard.status = (CARD_STATUS)status;
}

bool RoundManager::IsCurEffectCard(const CardNode_t *card) {
    if(card->kind==_curEffectCard.kind && card->status==_curEffectCard.status) {
        return true;
    } else {
        return false;
    }
}

/*************************************
        singleton
*************************************/
RoundManager* RoundManager::_instance = NULL;

RoundManager *RoundManager::getInstance() {
    if (_instance==NULL) {
        _instance = new RoundManager(NULL);
    }

    return _instance;
}

void RoundManager::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

/*************************************
        support
*************************************/
void RoundManager::_LogRoundCards() {
    char p[TOTAL_CARD_NUM] = {0};

    for(int i=0;i<TOTAL_CARD_NUM;i++) {
        p[i] = (_unDistributedCards[i]);
    }

    LOGGER_WRITE("new card sequence:\n");
    LOGGER_WRITE_ARRAY(p,TOTAL_CARD_NUM);
}


