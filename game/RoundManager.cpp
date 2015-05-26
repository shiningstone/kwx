
#include <stdlib.h>

#include "CardCollection.h"
#include "Player.h"
#include "RaceLayer.h"
#include "RoundManager.h"
#include "StrategyRm.h"

#include "Player.h"
#include "PlayerOthers.h"

RoundManager::RoundManager(RaceLayer *uiManager) {
    _MODE = LOCAL_GAME;

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

/***********************************************
        player information
***********************************************/
void RoundManager::InitPlayers() {
	_players[0] = new PlayerOthers(LEFT);
	_players[1] = new Player(MIDDLE);
	_players[2] = new PlayerOthers(RIGHT);

    Database *database = Database::getInstance();

    int  ids[3] = {0};
    _GenerateIds(ids);

    for(int dir=0;dir<3;dir++) {   
        UserProfile_t profile = {0};
        database->GetUserProfile(ids[dir],profile);
        _players[dir]->Set(&profile);
    }
}

void RoundManager::_GenerateIds(int ids[]) {
    ids[0] = rand()%16;
    ids[1] = 17;
    
    do {
        ids[2]=rand()%16;
    } while( ids[2]==ids[0] );
}

int RoundManager::Shuffle() {
#if 1
    _LoadRandomCardSequence();
#else
    load_test_round(1,_unDistributedCards);
#endif
    _distributedNum = 0;

    char p[TOTAL_CARD_NUM] = {0};
    for(int i=0;i<TOTAL_CARD_NUM;i++) {
        p[i] = (_unDistributedCards[i]);
    }
    LOGGER_WRITE("new card sequence:\n");
    LOGGER_WRITE_ARRAY(p,TOTAL_CARD_NUM);
    
    _isGangAsking = false;
    _isQiangGangAsking = false;
    _isDoubleHuAsking = false;
    _isNewDistributed = true;
    _firstMingNo = INVALID;
    _qiangGangTargetNo = INVALID;
    _otherOneForDouble = INVALID;
    _isWaitForMyDecision = false;
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
void RoundManager::CreateRace(Scene *scene) {
    _uiManager = RaceLayer::create();
    scene->addChild(_uiManager);

    _uiManager->Assign(this);

    InitPlayers();
	_isGameStart=false;
    
    _uiManager->CreateRace();
}

void RoundManager::StartGame() {
	_isGameStart=false;
    
    _players[MIDDLE]->_isReady = true;
	_uiManager->GuiShowReady(MIDDLE);

    _gRiver->clear();
    
    Shuffle();

    int lastWinner = GetLastWinner();
    _actCtrl.choices = _players[(lastWinner)%3]->init(&(_unDistributedCards[0]),14,aim[lastWinner]);//鐜╁鎵嬬墝鍒濆鍖?
	if(_actCtrl.choices!=a_TIMEOUT) {
		_players[(lastWinner+1)%3]->init(&(_unDistributedCards[14]),13,aim[(lastWinner+1)%3]);
		_players[(lastWinner+2)%3]->init(&(_unDistributedCards[27]),13,aim[(lastWinner+2)%3]);
		_uiManager->FirstRoundDistributeEffect((PlayerDir_t)lastWinner);//鐗屽眬寮�濮嬪彂鐗屾晥鏋溿�?
	}
}

Card_t RoundManager::RecvPeng(PlayerDir_t dir) {
    SetDecision(dir,aPENG);
    _actCtrl.handoutAllow = true;
    
    Card_t pengCard = _players[_curPlayer]->_river->get_kind(_players[_curPlayer]->_river->last());
    _players[_curPlayer]->_river->pop_back();

    RecordOutCard(pengCard);
    RecordOutCard(pengCard);

    TurnTo(dir);

    _uiManager->PengEffect((PlayerDir_t)_curPlayer,_prevPlayer,pengCard);

	return pengCard;
}

void RoundManager::RecvHu(PlayerDir_t dir) {
    if(_isWaitForMyDecision) {
        _isWaitForMyDecision = false;
        _actCtrl.decision = aHU;
    }

    if(_isQiangGangAsking) {
        _lastActionWithGold = aQIANG_GANG;
    }

    if(_isDoubleHuAsking) {
        SetWin(DOUBLE_WIN,_curPlayer);
    } else {
        SetWin(SINGLE_WIN,dir);
    }

    _uiManager->HuEffect(_lastWin, _isQiangGangAsking);
}

Card_t RoundManager::RecvGang(PlayerDir_t dir) {
    SetDecision(dir,aGANG);

	if(_actCtrl.decision==aAN_GANG || _actCtrl.decision==aSHOU_GANG) {
        int*   gangIdx = new int[4];
        Card_t card = _players[dir]->_cards->find_an_gang_cards(gangIdx);
        
		if( !IsMing(dir) ) {
			SetEffectCard(card,c_AN_GANG);
		}

        _uiManager->GangEffect(dir,card,gangIdx);
        return card;
	} else {
		PlayerDir_t prevPlayer = (PlayerDir_t)_curPlayer;

        CardInHand *cards = _players[dir]->_cards;
        Card_t card;
        
		if(!_isNewDistributed) {
            card = _players[_curPlayer]->_river->get_kind(_players[_curPlayer]->_river->last());
            _players[_curPlayer]->_river->pop_back();
    
			RecordOutCard(card);
			RecordOutCard(card);
			RecordOutCard(card);
            
			_curPlayer=dir;
		}else {
		    card = cards->get_kind(cards->last());/*BUG??? 如果我有之前没杠的*/
			RecordOutCard(card);
		}

        int* gangIdx = new int[4];
        card = cards->find_ming_gang_cards(gangIdx,card);

        _uiManager->GangEffect(dir,card,gangIdx,false,prevPlayer);
        return card;
	}
}

void RoundManager::RecvQi() {
    if(!_isNewDistributed && _lastActionSource!=MIDDLE) {
        _players[MIDDLE]->_cards->pop_back();
    }

    _uiManager->QiEffect();
}

void RoundManager::RecvHandout(int idx,Vec2 touch,int mode) {
    _actCtrl.handoutAllow = false;

    if(_isGangAsking) {
        _isGangAsking = false;
    }
    
	if(_isMingTime) {
		_isMingTime=false;
	} else {
        if(_actCtrl.decision==aMING) {
            _actCtrl.decision = aQi;
        }
    }
    
	if(_isWaitForMyDecision) {
		_isWaitForMyDecision=false;
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

void RoundManager::QiangGangHuJudge(PlayerDir_t dir) {
    LOGGER_WRITE("%s",__FUNCTION__);

    _qiangGangTargetNo = dir;

	_isNewDistributed=false;

	int no1=(_curPlayer+1)%3;
    unsigned char action1=_players[no1]->hand_in(
        LastHandout(),
        _isNewDistributed,
        _players[_curPlayer]->_cards->IsMing,
        false,
        aQIANG_GANG,
        _continue_gang_times,
        _isGangHua
    );

	int no2=(_curPlayer+2)%3;
	unsigned char action2=_players[no2]->hand_in(
        LastHandout(),
        _isNewDistributed,
        _players[_curPlayer]->_cards->IsMing,
        false,
        aQIANG_GANG,
        _continue_gang_times,
        _isGangHua
    );

	if((action1&a_HU)&&(action2&a_HU)) {
        WinInfo_t win;
        win.kind  = DOUBLE_WIN;
        win.giver = (PlayerDir_t)_curPlayer;
        
        if(no1==1) {
            _actCtrl.decision = (ActionId_t)action1;
            _otherOneForDouble = no2;
        } else {
            _actCtrl.decision = (ActionId_t)action2;
            _otherOneForDouble = no1;
        }

        _isDoubleHuAsking = true;
        _lastActionWithGold = aQIANG_GANG;

        _uiManager->DoubleWin(win);
	} else if(action1&a_HU||action2&a_HU) {
        WinInfo_t win;
        win.kind = SINGLE_WIN;
        win.winner = (PlayerDir_t)((action1&a_HU) ? no1 : no2);
        win.giver = (PlayerDir_t)_curPlayer;

        if(no1==1)
            _actCtrl.decision = (ActionId_t)action1;
        else
            _actCtrl.decision = (ActionId_t)action2;

        _isQiangGangAsking=true;
        _lastActionWithGold = aQIANG_GANG;
        
        _uiManager->SingleWin(win);
	} else {
		_isNewDistributed=true;

        _uiManager->GangGoldEffect(_qiangGangTargetNo,_curPlayer);
	}
}

void RoundManager::RecvKouCancel() {
    _players[MIDDLE]->_cards->clear_kou_choices();
    _uiManager->KouCancelEffect(_players[MIDDLE]->_cards);
}

void RoundManager::RecvKouConfirm() {
    CardInHand *cards = _players[MIDDLE]->_cards;

    for(int i=cards->FreeStart;i<cards->size();i++) {
        if(cards->get_status(i)==c_KOU_ENABLE)
            cards->set_status(i,sFREE);
    }   
    
    UpdateCards(MIDDLE,a_KOU);
    cards->collect_ming_info(_gRiver);

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
        _players[_curPlayer]->_cards->collect_ming_info(_gRiver);
    }

    if(_curPlayer==MIDDLE) {
        if(!isFromKouStatus) {
            _players[MIDDLE]->_cards->scan_kou_cards();

            if(_players[MIDDLE]->_cards->kou_group_num()>0) {
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

void RoundManager::WaitForFirstAction(PlayerDir_t zhuang) {
    _isGameStart = true;

    _curPlayer = zhuang;
    _actCtrl.choices = _players[_curPlayer]->judge_action_again();

    _isNewDistributed = true;
    if(zhuang==MIDDLE) {
        WaitForMyAction();
    } else {
        WaitForOthersAction((PlayerDir_t)zhuang);
    }
}

void RoundManager::WaitForMyAction() {
    _uiManager->ShowActionButtons(_actCtrl.choices);

	if(_actCtrl.choices!=a_JUMP) {
		_isWaitForMyDecision = true;
        
        if(_actCtrl.choices&a_AN_GANG  || _actCtrl.choices&a_MING_GANG || _actCtrl.choices&a_SHOU_GANG) {
            _isGangAsking = true;
        }
	}

	if(_isNewDistributed) {
		WaitForMyChoose();
	}
}

void RoundManager::WaitForMyChoose() {
	if(_isNewDistributed) {/* is this judgement neccessary??? */
		if( (_isTuoGuan) || (IsMing(_curPlayer) && !_isGangAsking) ) {
            int last = _players[MIDDLE]->_cards->last();
            
            Vec2 location = _uiManager->GetCardPositionInHand(last);
            RecvHandout(last,location,2);

            _prevPlayer = MIDDLE;
		} else {
			_actCtrl.handoutAllow = true;
        }
	}
}

void RoundManager::WaitForOthersAction(PlayerDir_t dir) {
    LOGGER_WRITE("%s (%d) perform action %d",__FUNCTION__,dir,_actCtrl.choices);

    if(_actCtrl.choices&aHU) {
        RecvHu(dir);
    } else if(_actCtrl.choices&aAN_GANG||_actCtrl.choices&aSHOU_GANG||_actCtrl.choices&aMING_GANG) {
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

	PlayerOthers *player = static_cast<PlayerOthers *>(_players[_curPlayer]);
    index = player->choose_card(*this,_actCtrl.decision,canKou);

    if(_actCtrl.decision==aMING) {
        UpdateCards((PlayerDir_t)_curPlayer,(ARRAY_ACTION)aMING);
    }
    
    if ( canKou ) {
        Card_t handingout = _players[_curPlayer]->_cards->get_kind(index);
        _players[_curPlayer]->_cards->choose_all_kou_cards(handingout);
    }

    RecordOutCard(_players[_curPlayer]->_cards->get_kind(index));
	_players[_curPlayer]->hand_out(index);

    if( canKou ) {
        /* it is dangerous to raise these lines to upper, since the following will change the card list*/
        if(_players[_curPlayer]->_cards->kou_group_num()>0)
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
    if(_isWaitForMyDecision) {
        _isWaitForMyDecision=false;
        
        if(_actCtrl.choices!=0 && !_isNewDistributed) {
            _players[MIDDLE]->_cards->pop_back();
        }
        
        if(!_isNewDistributed) {
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

unsigned int RoundManager::_GetPlayerReaction(PlayerDir_t dir,bool prevMing) {
    ActionMask_t actions = 
        _players[dir]->hand_in(
            LastHandout(),
            _isNewDistributed,
            prevMing,
            (_distributedNum==TOTAL_CARD_NUM),
            _lastActionWithGold,
            _continue_gang_times,
            _isGangHua
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
    }

    return actions;
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
            (_distributedNum==TOTAL_CARD_NUM),
            _lastActionWithGold,
            _continue_gang_times,
            _isGangHua
        );
    
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
    
    int no1=((PlayerDir_t)dir+1)%3;
    unsigned char action1 = _GetPlayerReaction((PlayerDir_t)no1,prevMingStatus);
    
    int no2=((PlayerDir_t)dir+2)%3;
    unsigned char action2 = _GetPlayerReaction((PlayerDir_t)no2,prevMingStatus);
    
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
            _uiManager->UpdateClock(0,no1);
            WaitForMyAction();
            return;
        }
        else
        {
            _uiManager->UpdateClock(0,no1);
            WaitForOthersAction((PlayerDir_t)no1);
            return;
        }
    }
    else if(action2!=a_JUMP)//涓婂
    {
        _actCtrl.choices=action2;
        if(no2==1)
        {
            _uiManager->UpdateClock(0,no2);
            WaitForMyAction();
            return;
        }
        else
        {
            _uiManager->UpdateClock(0,no2);
            WaitForOthersAction((PlayerDir_t)no2);
            return;
        }
    }
    else if(action1==action2&&action1==a_JUMP)
    {
        _actCtrl.decision = aQi;

        _curPlayer = TurnTo(NEXT);
        _uiManager->UpdateClock(0,_curPlayer);
        DistributeTo((PlayerDir_t)_curPlayer,(Card_t)(_unDistributedCards[_distributedNum++]));
    }
}

void RoundManager::DistributeTo(PlayerDir_t dir,Card_t card) {
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

void RoundManager::ActionAfterGang(PlayerDir_t dir) {
    if(_isNewDistributed) {
        QiangGangHuJudge(dir);
    } else {
        DistributeTo(dir,(Card_t)(_unDistributedCards[_distributedNum++]));
    }
}

void RoundManager::UpdateCards(PlayerDir_t dir,ARRAY_ACTION action,Card_t actKind) {
    if(action==a_AN_GANG) {
        _players[dir]->action(_isNewDistributed,aAN_GANG);
    } else if(action==a_SHOU_GANG) {
        _players[dir]->action(_isNewDistributed,aSHOU_GANG);
    } else {
        _players[dir]->action(_isNewDistributed,(ActionId_t)action);
    }
}

void RoundManager::SetDecision(PlayerDir_t dir,ActionId_t act) {
    if(_isWaitForMyDecision) {
        _isWaitForMyDecision = false;
    }

    if(_isGangAsking) {//is this judgement neccessary?
        _isGangAsking = false;
    }
    
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
    } else {
        _actCtrl.decision = act;
        _continue_gang_times = 0;
        _lastActionWithGold = aQi;
    }
    
    _lastActionSource = dir;
}

void RoundManager::_LoadRandomCardSequence() {
    int cards[TOTAL_CARD_NUM]; 

	for(int i=0;i<TOTAL_CARD_NUM;i++) {
		cards[i]=i;
	}

	for(int j=0;j<2;j++) {//伪随机数列生成
		for(int i=0;i<TOTAL_CARD_NUM;i++) {
			int tmp = cards[i];
			int cur = rand()%TOTAL_CARD_NUM;
			cards[i] = cards[cur];
			cards[cur] = tmp;
		}
	}

    for(int i=0;i<TOTAL_CARD_NUM;i++) {
        _unDistributedCards[i] = cards[i]/4;
    }
}

void RoundManager::update_gold(int gold[3]) {
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

PlayerDir_t RoundManager::TurnTo(PlayerDir_t dir) {
    _prevPlayer = (PlayerDir_t)_curPlayer;

    if(dir==INVALID_DIR) {
        _curPlayer  = (PlayerDir_t)(_prevPlayer+1)%PLAYER_NUM;
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

