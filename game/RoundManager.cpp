
#include <stdlib.h>

#include "RaceType.h"

#include "CardCollection.h"
#include "Player.h"
#include "RaceLayer.h"
#include "RoundManager.h"

RoundManager::RoundManager(RaceLayer *uiManager) {
    _MODE = LOCAL_GAME;

    _uiManager = uiManager;

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

void RoundManager::GetWin(WinInfo_t &info) {
    info.kind   = _lastWin.kind;
    info.winner = _lastWin.winner;
    info.giver  = _lastWin.giver;
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
bool RoundManager::IsTing(int id) const{
    return _players[id]->_cards->IsMing;
}

PlayerDir_t RoundManager::TurnToNext() {
    _curPlayer = (_curPlayer+1)%PLAYER_NUM;
    return (PlayerDir_t)_curPlayer;
}

/***********************************************
        river information
***********************************************/
void RoundManager::RecordOutCard( Card_t kind ) {
    _gRiver->push_back(kind);
    _gRiver->show();
}

void RoundManager::RenewOutCard() {
    delete _gRiver;
	_gRiver = new CardList;
}

/***********************************************
        player information
***********************************************/
#include "Player.h"
#include "PlayerOthers.h"

void RoundManager::InitPlayers() {
	_players[0] = new PlayerOthers();
	_players[1] = new Player();
	_players[2] = new PlayerOthers();

    Database *database = Database::getInstance();

    int  ids[3] = {0};
    _GenerateIds(ids);

    for(int dir=0;dir<3;dir++)
    {   
        UserProfile_t profile = {0};
        database->GetUserProfile(ids[dir],profile);
        _players[dir]->Set(&profile);
    }
}

void RoundManager::_GenerateIds(int ids[]) {
    ids[0]=rand()%16;
    ids[1]=17;
    
    do {
        ids[2]=rand()%16;
    } while( ids[2]==ids[0] );
}

int RoundManager::Shuffle() {
#if 0
    _LoadRandomCardSequence();
#else
    load_test_round(1,_unDistributedCards);
#endif
    _distributedNum = 0;

    char p[TOTAL_CARD_NUM] = {0};
    for(int i=0;i<TOTAL_CARD_NUM;i++) {
        p[i] = (_unDistributedCards[i]);
    }
    LOGGER_WRITE_ARRAY(p,TOTAL_CARD_NUM);
    
    _isGangAsking = false;
    _isQiangGangAsking = false;
    _isDoubleHuAsking = false;
    _isNewDistributed = true;
    _firstMingNo = INVALID;
    _qiangGangTargetNo = INVALID;
    _otherOneForDouble = INVALID;
    _isWaitDecision = false;
    _isGangHua = false;
    _isMyShowTime = false;
    _isTuoGuan = false;
    _otherHandedOut = CARD_UNKNOWN;
    
    CancelEffectCard();
    _isMingTime = false;

	_actionToDo=a_JUMP;
	_continue_gang_times=0;
    _lastAction = INVALID;
    _lastActionWithGold = aQi;
    _lastActionSource = INVALID;

    _curPlayer = GetLastWinner();

    return 0;
}


/****************************************
        before start
****************************************/
bool RoundManager::GetReadyStatus(PlayerDir_t dir) {
    LOGGER_WRITE("NETWORK : %s %d",__FUNCTION__,dir);
    return true;
}

bool RoundManager::WaitUntilAllReady() {
    LOGGER_WRITE("NETWORK : %s",__FUNCTION__);
    while( !GetReadyStatus(LEFT) || !GetReadyStatus(RIGHT) ) {
        //delay
    }

    return true;
}

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
    WaitUntilAllReady();

    RenewOutCard();
    Shuffle();

    int lastWinner = GetLastWinner();
    _actionToDo = _players[(lastWinner)%3]->init(&(_unDistributedCards[0]),14,aim[lastWinner]);//鐜╁鎵嬬墝鍒濆鍖?
	if(_actionToDo!=a_TIMEOUT) {
		_players[(lastWinner+1)%3]->init(&(_unDistributedCards[14]),13,aim[(lastWinner+1)%3]);
		_players[(lastWinner+2)%3]->init(&(_unDistributedCards[27]),13,aim[(lastWinner+2)%3]);
		_uiManager->FirstRoundDistributeEffect((PlayerDir_t)lastWinner);//鐗屽眬寮�濮嬪彂鐗屾晥鏋溿�?
	}
}

void RoundManager::RecvPeng(PlayerDir_t dir) {
    PlayerDir_t prevPlayer;
    
    if(_isWaitDecision) {
        _isWaitDecision = false;
        _actionToDo = _tempActionToDo;
        _tempActionToDo = a_JUMP;
    }

    _continue_gang_times = 0;
    _lastAction=a_PENG;
    
    Card_t pengCard = _players[_curPlayer]->_river->get_kind(_players[_curPlayer]->_river->last());
    _players[_curPlayer]->_river->pop_back();

    RecordOutCard(pengCard);
    RecordOutCard(pengCard);

    if(dir==MIDDLE) {
        _isMyShowTime = true;
    }
    
    prevPlayer = (PlayerDir_t)_curPlayer;
    _curPlayer = dir;

    _uiManager->PengEffect(dir,prevPlayer,pengCard);
}

void RoundManager::RecvHu(PlayerDir_t dir) {
    if(_isWaitDecision) {
        _isWaitDecision = false;
        _actionToDo = _tempActionToDo;
        _tempActionToDo = a_JUMP;
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

void RoundManager::RecvGang(PlayerDir_t dir) {
    if(_isGangAsking)//is this judgement neccessary?
        _isGangAsking = false;
    
    if(_isWaitDecision) {
        _isWaitDecision=false;
        _actionToDo = _tempActionToDo;
        _tempActionToDo = a_JUMP;
    }

	_continue_gang_times++;

    int* gangCardIdx=new int[4];
    Card_t card;
    
    CardInHand *cards = _players[dir]->_cards;

	if( _actionToDo & a_AN_GANG || _actionToDo & a_SHOU_GANG ) {
		_lastActionSource = dir;
        
		if(_actionToDo&a_AN_GANG) {
			_actionToDo=a_AN_GANG;
			_lastAction=a_AN_GANG;
			_lastActionWithGold = aAN_GANG;
		} else if(_actionToDo&a_SHOU_GANG) {
			_actionToDo=a_SHOU_GANG;
			_lastAction=a_SHOU_GANG;
			_lastActionWithGold = aSHOU_GANG;
		}
        
        card = cards->find_an_gang_cards(gangCardIdx);
        
		if( !IsTing(_curPlayer) ) {
			SetEffectCard(card,c_AN_GANG);
		}

        _uiManager->GangEffect(dir,card,gangCardIdx);
	}
	else if( _actionToDo & a_MING_GANG ) {
		_lastActionSource=dir;
		_actionToDo=a_MING_GANG;
		_lastAction=a_MING_GANG;
		_lastActionWithGold = aMING_GANG;

		Card_t GangCard;
		PlayerDir_t prevPlayer = (PlayerDir_t)_curPlayer;
        
		if(!_isNewDistributed) {
            card = _players[_curPlayer]->_river->get_kind(_players[_curPlayer]->_river->last());
            _players[_curPlayer]->_river->pop_back();
    
			RecordOutCard(card);
			RecordOutCard(card);
			RecordOutCard(card);
            
			_curPlayer=dir;
		}else {
		    card = cards->get_kind(cards->last());
			RecordOutCard(card);
		}

        card = cards->find_ming_gang_cards(gangCardIdx,card);
        _uiManager->GangEffect(dir,card,gangCardIdx,false,prevPlayer);
	}
}

void RoundManager::RecvQi() {
	if(_lastAction==a_JUMP) {
		_continue_gang_times=0;
    }

	_lastAction=a_JUMP;
	_actionToDo=a_JUMP;

	if(_isWaitDecision) {
		_isWaitDecision=false;
		_tempActionToDo=a_JUMP;
	}

    if(!_isNewDistributed) {
        _players[MIDDLE]->_cards->pop_back();
    }

    _uiManager->QiEffect();
}

void RoundManager::RecvHandout(int idx,Vec2 touch,int mode) {
    if(_isGangAsking) {
        _isGangAsking = false;
    }
    
	if(_isMingTime) {
		_isMingTime=false;
	} else {
        if(_actionToDo&a_MING) {
            _actionToDo = a_JUMP;
        }
    }
    
	if(_isWaitDecision) {
		_isWaitDecision=false;
		_tempActionToDo=a_JUMP;
	}

    RecordOutCard(_players[_curPlayer]->_cards->get_kind(idx));
    _lastHandedOutCard = _players[_curPlayer]->hand_out(idx);
    _players[_curPlayer]->_river->push_back(_lastHandedOutCard);

    bool turnToMing = false;
	if(_actionToDo==a_MING && 
        !IsTing(_curPlayer) ) {
        _players[_curPlayer]->_cards->set_ming(idx);

        turnToMing = true;
    }

    _uiManager->MyHandoutEffect(idx,touch,mode,turnToMing);
}

void RoundManager::QiangGangHuJudge(PlayerDir_t dir) {
    LOGGER_WRITE("%s",__FUNCTION__);

    _qiangGangTargetNo = dir;

	_isNewDistributed=false;

	int no1=(_curPlayer+1)%3;
    unsigned char action1=_players[no1]->hand_in(
        _lastHandedOutCard,
        _isNewDistributed,
        _players[_curPlayer]->_cards->IsMing,
        false,
        aQIANG_GANG,
        _continue_gang_times,
        _isGangHua
    );

	int no2=(_curPlayer+2)%3;
	unsigned char action2=_players[no2]->hand_in(
        _lastHandedOutCard,
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
            _actionToDo=action1;
            _otherOneForDouble = no2;
        } else {
            _actionToDo=action2;
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
            _actionToDo=action1;
        else
            _actionToDo=action2;

        _isQiangGangAsking=true;
        _lastActionWithGold = aQIANG_GANG;
        
        _uiManager->SingleWin(win);
	} else {
		_isNewDistributed=true;

        _uiManager->GangGoldEffect(_qiangGangTargetNo,_curPlayer);
	}
}

int RoundManager::_GroupIdx(int idx,CARD_ARRAY *cards) {
    if((cards->data[idx].kind==cards->data[idx+1].kind)&&(cards->data[idx].kind==cards->data[idx+2].kind)&&(cards->data[idx].kind==cards->data[idx+3].kind)&&(cards->data[idx].kind!=cards->data[idx+4].kind))
        return 1;
    else if((cards->data[idx].kind==cards->data[idx+1].kind)&&(cards->data[idx].kind==cards->data[idx+2].kind)&&(cards->data[idx].kind!=cards->data[idx+3].kind))
        return 2;
    else if(cards->data[idx].kind==cards->data[idx+1].kind&&cards->data[idx].kind!=cards->data[idx+2].kind)
        return 3;
    else if(cards->data[idx].kind!=cards->data[idx+1].kind)
        return 4;
}

CartApperance_t RoundManager::GetCardApperance(PlayerDir_t dir,int idx) {
    CardStatus_t status = _players[dir]->_cards->get_status(idx);

    bool isTing       = IsTing(dir);
    bool isMiddleTing = IsTing(MIDDLE);
    
    if(status==sFREE) {
        if(isTing) {
            return LAYDOWN_SHOW ;
        } else if (isMiddleTing) {
            return LAYDOWN_HIDE ;
        }
    } else if(status==sPENG || status==sMING_GANG) {
        return LAYDOWN_SHOW ;
    } else if(status==sAN_GANG) {
        int groupIdx = _players[dir]->_cards->get_idx_in_group(idx);
        
        if((dir==LEFT&&groupIdx==3) || (dir==RIGHT&&groupIdx==2)) {
            if(!isTing && isMiddleTing) {/* here must be a bug */
                if(isTing) {
                    return LAYDOWN_SHOW;
                } else if(!isTing&&isMiddleTing) {
                    return LAYDOWN_HIDE;
                }
            }
        }
    }

    return NORMAL_APPERANCE;
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
    _actionToDo=a_JUMP;
    
    _players[MIDDLE]->_cards->cancel_ming();
    /*!!!BUG MAYBE HERE : should clear MingInfo_t of cardsInHand*/

    _uiManager->MingCancelEffect();
}

void RoundManager::RecvMing(bool isFromKouStatus) {
	_actionToDo=a_MING;

    if(!isFromKouStatus) {
        _players[_curPlayer]->_cards->collect_ming_info(_gRiver);
    }

    if(_curPlayer==MIDDLE) {
        _players[MIDDLE]->_cards->scan_kou_cards();
        if(_players[MIDDLE]->_cards->kou_group_num()>0) {
            _uiManager->QueryKouCards();
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
    if(_MODE==LOCAL_GAME) {
        _actionToDo = _players[_curPlayer]->ActiontodoCheckAgain();
    }

    _isNewDistributed = true;
    if(zhuang==MIDDLE) {
        WaitForMyAction();
    } else {
        WaitForOthersAction((PlayerDir_t)zhuang);
    }
}

void RoundManager::WaitForMyAction() {
    _uiManager->ShowActionButtons(_actionToDo);

	if(_actionToDo!=a_JUMP) {
		_isWaitDecision = true;
		_tempActionToDo=_actionToDo;
		_actionToDo=a_JUMP;
	}

	if(_actionToDo&a_AN_GANG 
        || _actionToDo&a_MING_GANG
        ||_actionToDo&a_SHOU_GANG) {
        _isGangAsking = true;
	}

	if(_isNewDistributed) {
		if(_lastAction==a_JUMP&&!(_lastActionSource==1&&_continue_gang_times!=0)) {
			_continue_gang_times=0;
        }

		_lastAction=a_JUMP;
		WaitForMyChoose();
	}
}

void RoundManager::WaitForMyChoose() {
	if(_isNewDistributed) {/* is this judgement neccessary??? */
		if( _isTuoGuan ||
                (IsTing(_curPlayer) && !_isGangAsking) ) {
            int last = _players[MIDDLE]->_cards->last();
            
            Vec2 location = _uiManager->GetCardPositionInHand(last);
            RecvHandout(last,location,2);
		} else {
			_isMyShowTime = true;
        }
	}
}

void RoundManager::WaitForOthersAction(PlayerDir_t dir) {
    LOGGER_WRITE("%s (%d) perform action %d",__FUNCTION__,dir,_actionToDo);

    CardInHand *cards = _players[dir]->_cards;

    if(_actionToDo&a_HU) {
        RecvHu(dir);
    } else if(_actionToDo&a_AN_GANG||_actionToDo&a_SHOU_GANG) {
        _continue_gang_times++;
        _lastActionSource = dir;
        
        if(_actionToDo&a_AN_GANG) {
            _actionToDo=a_AN_GANG;
            _lastAction=a_AN_GANG;
            _lastActionWithGold = aAN_GANG;
        } else if(_actionToDo&a_SHOU_GANG) {
            _actionToDo=a_SHOU_GANG;
            _lastAction=a_SHOU_GANG;
            _lastActionWithGold = aSHOU_GANG;
        }

        int* gangIdx=new int[4];
        Card_t card = cards->find_an_gang_cards(gangIdx);

        if( !IsTing(dir) ) {
            SetEffectCard(card,c_AN_GANG);
        }

        _uiManager->_AnGangEffect(dir,card,gangIdx);
    } else if(_actionToDo&a_MING_GANG) {
        _lastActionSource=dir;
        _actionToDo=a_MING_GANG;
        _lastAction=a_MING_GANG;
        _lastActionWithGold = aMING_GANG;

        Card_t GangCard;
        PlayerDir_t prevPlayer = (PlayerDir_t)dir;
        if(!_isNewDistributed) {
            CardNode_t * last = _players[_curPlayer]->_river->back();
            GangCard = last->kind;
            _players[_curPlayer]->_river->pop_back();

            RecordOutCard(GangCard);
            RecordOutCard(GangCard);
            RecordOutCard(GangCard);
            
            _curPlayer=dir;
        }else {
            GangCard = cards->get_kind(cards->last());/*BUG : what if I have gang cards but do not act gang*/
            RecordOutCard(GangCard);
        }

        int* gangIdx=new int[4];
        GangCard = cards->find_ming_gang_cards(gangIdx,GangCard);
        _uiManager->_MingGangEffect(dir,prevPlayer,GangCard,gangIdx);
    }
    else if(_actionToDo&a_MING) {
        RecvMing();
    } else if(_actionToDo&a_PENG) {
        RecvPeng(dir);
    } else if(_actionToDo==a_JUMP) {
        if(_lastAction==a_JUMP) {
            _continue_gang_times=0;
        }
        _lastAction=a_JUMP;
        
        WaitForOthersChoose();
    }
}

void RoundManager::WaitForOthersChoose() {
    bool canKou = false;

	PlayerOthers *player = static_cast<PlayerOthers *>(_players[_curPlayer]);
	int index = player->choose_worst(*this,canKou);
    
    if ( canKou ) {
        _otherHandedOut = _players[_curPlayer]->_cards->get_kind(index);
        _players[_curPlayer]->_cards->choose_all_kou_cards(_otherHandedOut);
    }

    RecordOutCard(_players[_curPlayer]->_cards->get_kind(index));
	_lastHandedOutCard = _players[_curPlayer]->hand_out(index);
    _players[_curPlayer]->_river->push_back(_lastHandedOutCard);

    if(canKou) {
        /* it is dangerous to raise these lines to upper, since the following will change the card list*/
        if(_players[_curPlayer]->_cards->kou_group_num()>0)
            UpdateCards((PlayerDir_t)_curPlayer,a_KOU);
    }

    if(_actionToDo==aMING) {
        UpdateCards((PlayerDir_t)_curPlayer,a_MING);
        _players[_curPlayer]->_cards->set_ming(index);
    }
    
    _uiManager->TingHintBarOfOthers(_curPlayer,index);

	_isNewDistributed = false;

    _uiManager->OthersHandoutEffect((PlayerDir_t)_curPlayer,canKou);
}

unsigned int RoundManager::_GetPlayerReaction(PlayerDir_t dir,bool prevTingStatus) {
    ActionMask_t actions = 
        _players[dir]->hand_in(
            _lastHandedOutCard,
            _isNewDistributed,
            prevTingStatus,
            (_distributedNum==TOTAL_CARD_NUM),
            _lastActionWithGold,
            _continue_gang_times,
            _isGangHua
        );
    
    if(dir==MIDDLE&&_isTuoGuan) {
        if(IsTing(MIDDLE)&&(actions&a_HU)) {
            actions=a_HU;
        } else {
            actions=a_JUMP;
        }
    } else if(dir!=MIDDLE) {
        if(_players[dir]->_cards->is_aim_limit(actions,_lastHandedOutCard)) {
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
    
    if(_lastActionSource==dir&&_continue_gang_times!=0)
        _isGangHua=true;
    else
        _continue_gang_times=0;
    
    _actionToDo = 
        _players[dir]->hand_in(
            _lastHandedOutCard,
            _isNewDistributed,
            _players[dir]->_cards->IsMing,
            (_distributedNum==TOTAL_CARD_NUM),
            _lastActionWithGold,
            _continue_gang_times,
            _isGangHua
        );
    
    if((PlayerDir_t)dir==MIDDLE) {
        if(IsTing(MIDDLE)&&(_actionToDo&a_HU)){
            RecvHu(MIDDLE);
        }else{
            if(_isTuoGuan)
                _actionToDo=a_JUMP;
    
            WaitForMyAction();
        }
    }else{
        if(_players[dir]->_cards->is_aim_limit(_actionToDo,_lastHandedOutCard)) {
            _actionToDo = a_JUMP;
        }
        WaitForOthersAction(dir);
    }
}

void RoundManager::_HandleCardFrom(PlayerDir_t dir) {
    bool prevTingStatus = IsTing(dir);
    
    int no1=((PlayerDir_t)dir+1)%3;
    unsigned char action1 = _GetPlayerReaction((PlayerDir_t)no1,prevTingStatus);
    
    int no2=((PlayerDir_t)dir+2)%3;
    unsigned char action2 = _GetPlayerReaction((PlayerDir_t)no2,prevTingStatus);
    
    if((action1&a_HU)&&(action2&a_HU))
    {
        _uiManager->HideClock();
        
        if((no1!=MIDDLE&&no2!=MIDDLE) || (no1==MIDDLE||no2==MIDDLE&&IsTing(MIDDLE))){ 
            SetWin(DOUBLE_WIN,(PlayerDir_t)_curPlayer);
            _uiManager->_HuEffect(_lastWin);
            _uiManager->_DistributeEvent(DOUBLE_HU_WITH_ME,NULL);
        } else if((no1==MIDDLE||no2==MIDDLE) && !IsTing(MIDDLE)) {
            _isDoubleHuAsking = true;
            if(no1==1) {
                _otherOneForDouble = no2;
                _actionToDo=action1;
            } else {
                _otherOneForDouble = no1;
                _actionToDo=action2;
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
                    _actionToDo=action1;
                else
                    _actionToDo=action2;
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
        _actionToDo=action1;
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
        _actionToDo=action2;
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
        _actionToDo=action1;
        _curPlayer=(_curPlayer+1)%3;
        _uiManager->UpdateClock(0,_curPlayer);
        DistributeTo((PlayerDir_t)_curPlayer,(Card_t)(_unDistributedCards[_distributedNum++]));
    }
}

void RoundManager::WaitForResponse(PlayerDir_t dir) {
    if(_isNewDistributed) {
        _HandleCardNewDistributed(dir);
    } else {
        _HandleCardFrom(dir);
    }
}

void RoundManager::DistributeTo(PlayerDir_t dir,Card_t card) {
    if(_distributedNum<TOTAL_CARD_NUM+1) {
		_lastHandedOutCard = card;
        _isNewDistributed  = true;

        DistributeInfo_t distInfo;
        
        distInfo.target = dir;
        distInfo.card   = card;
        distInfo.distNum = _distributedNum;
        
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
    if(_actionToDo&a_AN_GANG) {
        _players[dir]->action(_isNewDistributed,aAN_GANG);
    } else if(_actionToDo&a_SHOU_GANG) {
        _players[dir]->action(_isNewDistributed,aSHOU_GANG);
    } else {
        _players[dir]->action(_isNewDistributed,(ActionId_t)action);
    }
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

